// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Viewport_Game.h"

#include "CyredModule_Render.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Input.h"
#include "CyredModule_Physics.h"
#include "CyredModule_Script.h"


using namespace CYRED;


Viewport_Game::Viewport_Game( int panelIndex )
	: Panel_Viewport( panelIndex )
{
}


cchar* Viewport_Game::_GetPanelTitle()
{
	return PANEL_TITLE;
}


void Viewport_Game::_OnUpdate( bool isRuntime )
{
	RenderManager* renderMngr = RenderManager::Singleton();

	// check window resize
	if ( _mustResize ) {
		renderMngr->OnResize( _canvasSlot );
		_mustResize = FALSE;
	}

	if ( _isFirstUpdate ) {
		// create renderers
		renderMngr->SwitchCanvas( _canvasSlot );
		renderMngr->CreateRenderer( RendererType::GL_FORWARD );
	}

	// set renderer
	renderMngr->SwitchCanvas( _canvasSlot );
	renderMngr->SwitchRenderer( RendererType::GL_FORWARD );
	// clear screen
	renderMngr->ClearScreen( 0, 0, 0 );

	// render scenes
	if ( SceneManager::Singleton()->CountLoadedScenes() > 0 ) {
		// get first scene's root
		Node* sceneRoot = SceneManager::Singleton()->GetScene()->GetRoot();

		// get main camera
		GameObject* cameraGO = _RecFindMainCamera( sceneRoot );
		if ( cameraGO == NULL ) {
			// finish
			renderMngr->SwapBuffers();
			return;
		}

		_currCamera		= cameraGO->GetComponent<Camera>();
		_currCameraTran	= cameraGO->GetComponent<Transform>();
		if ( _currCamera == NULL || _currCameraTran == NULL ) {
			// finish
			renderMngr->SwapBuffers();
			return;
		}

		// all good

		// set camera
		renderMngr->SwitchCamera( _currCameraTran, _currCamera );

		// update camera size
		{
			bool emitEvents = _currCamera->DoesEmitEvents();
			_currCamera->SetEmitEvents( FALSE );
			{
				float aspectRatio = CAST_S( float, _qtWindow->width() ) / _qtWindow->height();
				float height = _currCamera->GetOrthoSize().y;
				_currCamera->SetAspectRatio( aspectRatio );
				_currCamera->SetOrthoWidth( aspectRatio * height );
			}
			_currCamera->SetEmitEvents( emitEvents );
		}


		// apply mouse callbacks
		if ( isRuntime ) {
			_TestMouseInput();
		}
		

		// collect layers
		DataArray<int> layers;
		for ( int i = 0; i < sceneRoot->GetChildNodeCount(); i++ ) {
			_RecCollectLayers( CAST_S(GameObject*, sceneRoot->GetChildNodeAt(i)), layers );
		}

		// collect lights
		DataArray<GameObject*> lightsGO;
		_RecCollectLights( sceneRoot, lightsGO );

		// render by layers
		for ( int i = 0; i < layers.Size(); i++ ) {
			// render meshes
			renderMngr->Render( layers[i], ComponentType::MESH_RENDERING, sceneRoot, lightsGO );
			// render morphs
			renderMngr->Render( layers[i], ComponentType::MORPH_RENDERING, sceneRoot, lightsGO );
			// render text 3d
			renderMngr->Render( layers[i], ComponentType::TEXT_3D, sceneRoot, lightsGO );
			// render particles
			renderMngr->Render( layers[i], ComponentType::PARTICLE_EMITTER, sceneRoot, lightsGO );
		
			// reset depth
			renderMngr->ResetDepth();
		}
	}

	// finish
	renderMngr->SwapBuffers();
}


GameObject* Viewport_Game::_RecFindMainCamera( Node* root )
{
	if ( root == NULL ) {
		return NULL;
	}

	// check if gameobject
	GameObject* gameObject = CAST_D( GameObject*, root );
	if ( gameObject != NULL ) {
		// check for camera component
		Camera* camera = gameObject->GetComponent<Camera>();
		if ( camera != NULL && camera->IsEnabled() ) {
			return gameObject;
		}
	}

	// parse children
	for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
		GameObject* found = _RecFindMainCamera( root->GetChildNodeAt(i) );
		if ( found ) {
			return found;
		}
	}

	return NULL;
}


void Viewport_Game::_RecCollectLights( Node* root, DataArray<GameObject*>& lightsGO )
{
	if ( root == NULL ) {
		return;
	}

	// check if gameobject
	GameObject* gameObject = CAST_D( GameObject*, root );
	if ( gameObject != NULL ) {
		// check for light component
		Light* light = gameObject->GetComponent<Light>();
		if ( light != NULL ) {
			lightsGO.Add( gameObject );
		}
	}

	// parse children
	for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
		_RecCollectLights(root->GetChildNodeAt(i), lightsGO );
	}
}


void Viewport_Game::_TestMouseInput()
{
	InputManager* inputMngr		= InputManager::Singleton();

	// check target window
	int targetWindow = inputMngr->GetWindowForMouse();
	if ( targetWindow == _panelIndex ) {
		// get mouse buttons
		bool mouseLeftDown	 = inputMngr->KeyDownFirstTime( KeyCode::MOUSE_LEFT );
		bool mouseLeftUp	 = inputMngr->KeyUpFirstTime( KeyCode::MOUSE_LEFT );
		bool mouseMiddleDown = inputMngr->KeyDownFirstTime( KeyCode::MOUSE_MIDDLE );
		bool mouseMiddleUp	 = inputMngr->KeyUpFirstTime( KeyCode::MOUSE_MIDDLE );
		bool mouseRightDown	 = inputMngr->KeyDownFirstTime( KeyCode::MOUSE_RIGHT );
		bool mouseRightUp	 = inputMngr->KeyUpFirstTime( KeyCode::MOUSE_RIGHT );

		// at least one must be active
		if ( mouseLeftDown || mouseLeftUp || 
			 mouseMiddleDown || mouseMiddleUp || 
			 mouseRightDown || mouseRightUp )
		{
			// get mouse pos
			Vector2 mousePos = inputMngr->MousePosition();

			// create ray
			Ray ray;

			// transform viewport coords to world coords
			Vector4 rayStartNDC = Vector4(
				( mousePos.x / _qtWindow->width() - 0.5f ) * 2.0f,
				- ( mousePos.y / _qtWindow->height() - 0.5f ) * 2.0f,
				-1.0f, // The near plane maps to Z=-1 in Normalized Device Coordinates
				1.0f
			);
			Vector4 rayEndNDC = Vector4(
				( mousePos.x / _qtWindow->width() - 0.5f ) * 2.0f,
				- ( mousePos.y / _qtWindow->height() - 0.5f ) * 2.0f,
				0.0f,
				1.0f
			);
			const Matrix4& projMat = _currCamera->GetProjectionMatrix();
			const Matrix4& viewMat = _currCameraTran->GetViewMatrix();
			const Matrix4& mat = Matrix4::Inverse( projMat * viewMat );
			Vector4 rayStartWorld = mat * rayStartNDC; 
			rayStartWorld /= rayStartWorld.w;
			Vector4 rayEndWorld = mat * rayEndNDC ;
			rayEndWorld /= rayEndWorld.w;

			// calculate ray
			Vector4 diff = rayEndWorld - rayStartWorld;
			Vector3 dir = Vector3::Normalize( Vector3( diff.x, diff.y, diff.z ) );
			float far = _currCamera->GetFarClipping();
			float near = _currCamera->GetNearClipping();
			ray.start	= Vector3( rayStartWorld.x, rayStartWorld.y, rayStartWorld.z );
			ray.end		= ray.start + dir * (far - near);
			// do raycasting
			Scripter* scripter = PhysicsManager::Singleton()->RaycastFirstTarget( ray );

			// call script API
			if ( scripter != NULL ) {
				if ( mouseLeftDown ) {
					scripter->OnMouseDown( KeyCode::MOUSE_LEFT );
				}

				if ( mouseLeftUp ) {
					scripter->OnMouseUp( KeyCode::MOUSE_LEFT );
				}

				if ( mouseMiddleDown ) {
					scripter->OnMouseDown( KeyCode::MOUSE_MIDDLE );
				}

				if ( mouseMiddleUp ) {
					scripter->OnMouseUp( KeyCode::MOUSE_MIDDLE );
				}

				if ( mouseRightDown ) {
					scripter->OnMouseDown( KeyCode::MOUSE_RIGHT );
				}

				if ( mouseRightUp ) {
					scripter->OnMouseUp( KeyCode::MOUSE_RIGHT );
				}
			}
		}
	}
}
