// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Viewport_Game.h"

#include "CyredModule_Render.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Input.h"
#include "CyredModule_Physics.h"


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
		_cameraGO = _RecFindMainCamera( sceneRoot );
		if ( _cameraGO == NULL ) {
			// finish
			renderMngr->SwapBuffers();
			return;
		}

		Camera* camera			= _cameraGO->GetComponent<Camera>();
		Transform* cameraTran	= _cameraGO->GetComponent<Transform>();
		if ( camera == NULL || cameraTran == NULL ) {
			// finish
			renderMngr->SwapBuffers();
			return;
		}

		// all good

		// update camera size
		{
			// disable events
			bool emitEvents = camera->DoesEmitEvents();
			camera->SetEmitEvents( FALSE );
			// update camera
			float aspectRatio = CAST_S( float, _qtWindow->width() ) / _qtWindow->height();
			float height = camera->GetOrthoSize().y;
			camera->SetAspectRatio( aspectRatio );
			camera->SetOrthoWidth( aspectRatio * height );
			// set back emit events
			camera->SetEmitEvents( emitEvents );
		}


		// apply mouse callbacks
		if ( isRuntime ) {
			_TestMouseInput( cameraTran, camera );
		}
		

		// collect lights
		DataArray<GameObject*> lightsGO;
		_RecCollectLights( sceneRoot, lightsGO );

		// render meshes
		renderMngr->Render( ComponentType::MESH_RENDERING, sceneRoot, _cameraGO, lightsGO );
		// render morphs
		renderMngr->Render( ComponentType::MORPH_RENDERING, sceneRoot, _cameraGO, lightsGO );
		// render particles
		renderMngr->Render( ComponentType::PARTICLE_EMITTER, sceneRoot, _cameraGO, lightsGO );
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


void Viewport_Game::_TestMouseInput( Transform* cameraTran, Camera* camera )
{
	InputManager* inputMngr		= InputManager::Singleton();
	PhysicsManager* physicsMngr = PhysicsManager::Singleton();

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

			// calculate ray
			Ray ray;
			ray.origin = cameraTran->GetPositionWorld();

			// transform viewport coords to world coords
			Vector4 rayStartNDC = Vector4(
				( mousePos.x / _qtWindow->width() - 0.5f ) * 2.0f,
				( mousePos.y / _qtWindow->height() - 0.5f ) * 2.0f,
				-1.0f, // The near plane maps to Z=-1 in Normalized Device Coordinates
				1.0f
			);
			Vector4 rayEndNDC = Vector4(
				( mousePos.x / _qtWindow->width() - 0.5f ) * 2.0f,
				( mousePos.y / _qtWindow->height() - 0.5f ) * 2.0f,
				0.0f,
				1.0f
			);
			const Matrix4& projMat = camera->GetProjectionMatrix();
			const Matrix4& viewMat = cameraTran->GetViewMatrix();
			const Matrix4& mat = Matrix4::Inverse( projMat * viewMat );
			Vector4 rayStartWorld = mat * rayStartNDC; 
			rayStartWorld /= rayStartWorld.w;
			Vector4 rayEndWorld = mat * rayEndNDC ;
			rayEndWorld /= rayEndWorld.w;

			// calculate ray direction
			Vector4 rayDir = rayEndWorld - rayStartWorld;
			ray.direction = Vector3::Normalize( Vector3( rayDir.x, rayDir.y, rayDir.z ) );


			// do raycasting

			if ( mouseLeftDown ) {
				physicsMngr->ApplyMouseDown( KeyCode::MOUSE_LEFT, ray );
			}

			if ( mouseLeftUp ) {
				physicsMngr->ApplyMouseUp( KeyCode::MOUSE_LEFT, ray );
			}

			if ( mouseMiddleDown ) {
				physicsMngr->ApplyMouseDown( KeyCode::MOUSE_MIDDLE, ray );
			}

			if ( mouseMiddleUp ) {
				physicsMngr->ApplyMouseUp( KeyCode::MOUSE_MIDDLE, ray );
			}

			if ( mouseRightDown ) {
				physicsMngr->ApplyMouseDown( KeyCode::MOUSE_RIGHT, ray );
			}

			if ( mouseRightUp ) {
				physicsMngr->ApplyMouseUp( KeyCode::MOUSE_RIGHT, ray );
			}
		}
	}
}
