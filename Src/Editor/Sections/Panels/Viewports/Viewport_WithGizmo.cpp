// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Viewport_WithGizmo.h"

#include "CyredModule_Render.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Physics.h"
#include "CyredModule_Input.h"
#include "CyredModule_Time.h"

#include "../../../Utils/EditorEvents.h"


using namespace CYRED;


Viewport_WithGizmo::Viewport_WithGizmo( int panelIndex )
	: Panel_Viewport( panelIndex )
	, _selectedGO( NULL )
	, _gizmoGrid( NULL )
	, _gizmoAxis( NULL )
	, _gizmoBackground( NULL )
	, _gizmoPointLight( NULL )
	, _gizmoDirLight( NULL )
	, _gizmoSpotLight( NULL )
	, _gizmoOrthoCamera( NULL )
	, _gizmoPerspCamera( NULL )
	, _gizmoPivot( NULL )
{
}


void Viewport_WithGizmo::LoadGizmo()
{
	FiniteString gizmoGrid			( GIZMO_GRID );
	FiniteString gizmoAxis			( GIZMO_AXIS );
	FiniteString gizmoBackground	( GIZMO_BACKGROUND );
	FiniteString gizmoPointLight	( GIZMO_POINT_LIGHT );
	FiniteString gizmoDirLight		( GIZMO_DIR_LIGHT );
	FiniteString gizmoSpotLight		( GIZMO_SPOT_LIGHT );
	FiniteString gizmoOrthoCamera	( GIZMO_ORTHO_CAMERA );
	FiniteString gizmoPerspCamera	( GIZMO_PERSP_CAMERA );
	FiniteString gizmoPivot			( GIZMO_PIVOT );
	FiniteString gizmoCollBox		( GIZMO_COLL_BOX );
	FiniteString gizmoCollSphere	( GIZMO_COLL_SPHERE );
	FiniteString gizmoDebugText		( GIZMO_DEBUG_TEXT );

	// parse prefabs and find gizmo grid
	for ( int i = 0; i < AssetManager::Singleton()->GetPrefabCount(); i++ ) {
		Prefab* prefab = AssetManager::Singleton()->GetPrefabAt( i );

		// check name
		if ( gizmoGrid == prefab->GetName() ) {
			_gizmoGrid = new GameObject();
			prefab->GetRoot()->Clone( _gizmoGrid );
		}
		else if ( gizmoAxis == prefab->GetName() ) {
			_gizmoAxis = new GameObject();
			prefab->GetRoot()->Clone( _gizmoAxis );
		}
		else if ( gizmoBackground == prefab->GetName() ) {
			_gizmoBackground = new GameObject();
			prefab->GetRoot()->Clone( _gizmoBackground );
		}
		else if ( gizmoPointLight == prefab->GetName() ) {
			_gizmoPointLight = new GameObject();
			prefab->GetRoot()->Clone( _gizmoPointLight );
		}
		else if ( gizmoDirLight == prefab->GetName() ) {
			_gizmoDirLight = new GameObject();
			prefab->GetRoot()->Clone( _gizmoDirLight );
		}
		else if ( gizmoSpotLight == prefab->GetName() ) {
			_gizmoSpotLight = new GameObject();
			prefab->GetRoot()->Clone( _gizmoSpotLight );
		}
		else if ( gizmoOrthoCamera == prefab->GetName() ) {
			_gizmoOrthoCamera = new GameObject();
			prefab->GetRoot()->Clone( _gizmoOrthoCamera );
		}
		else if ( gizmoPerspCamera == prefab->GetName() ) {
			_gizmoPerspCamera = new GameObject();
			prefab->GetRoot()->Clone( _gizmoPerspCamera );
		}
		else if ( gizmoPivot == prefab->GetName() ) {
			_gizmoPivot = new GameObject();
			prefab->GetRoot()->Clone( _gizmoPivot );
		}
		else if ( gizmoCollBox == prefab->GetName() ) {
			_gizmoCollBox = new GameObject();
			prefab->GetRoot()->Clone( _gizmoCollBox );
		}
		else if ( gizmoCollSphere == prefab->GetName() ) {
			_gizmoCollSphere = new GameObject();
			prefab->GetRoot()->Clone( _gizmoCollSphere );
		}
		else if ( gizmoDebugText == prefab->GetName() ) {
			_gizmoDebugText = new GameObject();
			prefab->GetRoot()->Clone( _gizmoDebugText );
		}
	}
}


void Viewport_WithGizmo::SetCamera( GameObject* cameraGO )
{
	if ( cameraGO != NULL ) {
		_currCameraTran = cameraGO->GetComponent<Transform>();
		_currCamera		= cameraGO->GetComponent<Camera>();
	}
	else {
		_currCameraTran = NULL;
		_currCamera		= NULL;
	}
}


bool Viewport_WithGizmo::_IsRenderingReady()
{
	RenderManager* renderMngr = RenderManager::Singleton();

	if ( _mustResize ) {
		renderMngr->OnResize( _canvasSlot );
		_mustResize = FALSE;
	}

	if ( _isFirstUpdate ) {
		// create renderers
		renderMngr->SwitchCanvas( _canvasSlot );
		renderMngr->CreateRenderer( RendererType::GL_PICKING );
		renderMngr->CreateRenderer( RendererType::GL_FORWARD );
	}

	// set renderer
	renderMngr->SwitchCanvas( _canvasSlot );
	renderMngr->SwitchRenderer( RendererType::GL_FORWARD );
	// clear screen
	renderMngr->ClearScreen( 0.3f, 0.3f, 0.3f );

	// exit if no camera
	if ( _currCameraTran == NULL || _currCamera == NULL ) {
		// finish
		renderMngr->SwapBuffers();
		return FALSE;
	}

	// set camera
	renderMngr->SwitchCamera( _currCameraTran, _currCamera );

	// update camera size
	bool emitEvents = _currCamera->DoesEmitEvents();
	_currCamera->SetEmitEvents( FALSE );
	{
		// update camera
		float aspectRatio = CAST_S( float, _qtWindow->width() ) / _qtWindow->height();
		float height = _currCamera->GetOrthoSize().y;
		_currCamera->SetAspectRatio( aspectRatio );
		_currCamera->SetOrthoWidth( aspectRatio * height );
	}
	_currCamera->SetEmitEvents( emitEvents );

	return TRUE;
}


void Viewport_WithGizmo::_RecCollectLights( GameObject* root, DataArray<GameObject*>& lightsGO )
{
	if ( root == NULL ) {
		return;
	}

	// check for light component
	Light* light = root->GetComponent<Light>();
	if ( light != NULL ) {
		lightsGO.Add( root );
	}

	// parse children
	for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
		_RecCollectLights( CAST_S(GameObject*, root->GetChildNodeAt(i)), lightsGO );
	}
}


void Viewport_WithGizmo::_RenderGizmo()
{
	RenderManager* renderMngr = RenderManager::Singleton();

	// render gizmo background
	if ( _gizmoBackground != NULL ) {
		renderMngr->Render( 0, ComponentType::MESH_RENDERING, _gizmoBackground, _noLightsGO );
	}

	// render gizmo grid
	if ( _gizmoGrid != NULL ) {
		renderMngr->Render( 0, ComponentType::MESH_RENDERING, _gizmoGrid, _noLightsGO );
	}

	// render gizmo axis
	if ( _gizmoAxis != NULL ) {
		renderMngr->Render( 0, ComponentType::MESH_RENDERING, _gizmoAxis, _noLightsGO );
	}

	// render selected object gizmo
	if ( _selectedGO != NULL && _selectedGO->IsEnabled() ) {
		Light*		light		= _selectedGO->GetComponent<Light>();
		Camera*		camera		= _selectedGO->GetComponent<Camera>();
		Transform*	transform	= _selectedGO->GetComponent<Transform>();
		RigidBody*	rigidBody	= _selectedGO->GetComponent<RigidBody>();

		if ( transform != NULL && transform->IsEnabled() ) {

			if ( light != NULL && light->IsEnabled() ) {
				// gizmo point light
				if ( _gizmoPointLight != NULL && light->GetLightType() == LightType::POINT ) {
					// update transform
					Transform* rootTran = _gizmoPointLight->GetComponent<Transform>();
					rootTran->SetEmitEvents( FALSE );
					{
						rootTran->SetPositionWorld( transform->GetPositionWorld() );
						rootTran->SetRotationWorld( transform->GetRotationWorld() );
						// assume initial radius is 1
						float range = light->GetRange();
						rootTran->SetScaleWorld( Vector3( range, range, range ) );
					}
					rootTran->SetEmitEvents( TRUE );

					// render gizmo
					renderMngr->Render( 0, ComponentType::MESH_RENDERING, _gizmoPointLight, _noLightsGO );
				}

				// gizmo directional light
				if ( _gizmoDirLight != NULL && light->GetLightType() == LightType::DIRECTIONAL ) {
					// update transform
					Transform* rootTran = _gizmoDirLight->GetComponent<Transform>();
					rootTran->SetEmitEvents( FALSE );
					{
						rootTran->SetPositionWorld( transform->GetPositionWorld() );
						rootTran->SetRotationWorld( transform->GetRotationWorld() );
					}
					rootTran->SetEmitEvents( TRUE );

					// render gizmo
					renderMngr->Render( 0, ComponentType::MESH_RENDERING, _gizmoDirLight, _noLightsGO );
				}

				// gizmo spot light
				if ( _gizmoSpotLight != NULL && light->GetLightType() == LightType::SPOT ) {
					// update transform
					Transform* rootTran = _gizmoSpotLight->GetComponent<Transform>();
					rootTran->SetEmitEvents( FALSE );
					{
						rootTran->SetPositionWorld( transform->GetPositionWorld() );
						rootTran->SetRotationWorld( transform->GetRotationWorld() );
						// assume initial size (1, 1, 1)
						float tan = Math::Tan( Math::ToRadians( light->GetSpotAngle() / 2 ) );
						float range = light->GetRange();
						float scaleBase = range * tan;
						rootTran->SetScaleWorld( Vector3( scaleBase, scaleBase, range ) );
					}
					rootTran->SetEmitEvents( TRUE );

					// render gizmo
					renderMngr->Render( 0, ComponentType::MESH_RENDERING, _gizmoSpotLight, _noLightsGO );
				}
			}

			if ( camera != NULL && camera->IsEnabled() ) {
				// gizmo ortho camera
				if ( _gizmoOrthoCamera != NULL && camera->GetCameraType() == CameraType::ORTHOGRAPHIC ) {
					// update transform
					Transform* rootTran = _gizmoOrthoCamera->GetComponent<Transform>();
					rootTran->SetEmitEvents( FALSE );
					{
						rootTran->SetPositionWorld( transform->GetPositionWorld() );
						rootTran->SetRotationWorld( transform->GetRotationWorld() );

						// assume initial size (1, 1, 1)
						float far = camera->GetFarClipping();
						float near = camera->GetNearClipping();
						Vector2 orthoSize = camera->GetOrthoSize();

						rootTran->SetScaleWorld( Vector3( orthoSize.x, orthoSize.y, far - near ) );
						rootTran->TranslateByLocal( Vector3( 0, 0, - near ) );
					}
					rootTran->SetEmitEvents( TRUE );

					// render gizmo
					renderMngr->Render( 0, ComponentType::MESH_RENDERING, _gizmoOrthoCamera, _noLightsGO );
				}

				// gizmo perspective camera
				if ( _gizmoPerspCamera != NULL && camera->GetCameraType() == CameraType::PERSPECTIVE ) {
					// update transform
					Transform* rootTran = _gizmoPerspCamera->GetComponent<Transform>();
					rootTran->SetEmitEvents( FALSE );
					{
						rootTran->SetPositionWorld( transform->GetPositionWorld() );
						rootTran->SetRotationWorld( transform->GetRotationWorld() );

						// assume initial size (2, 2, 1)
						float near = camera->GetNearClipping();
						float far = camera->GetFarClipping();
						float tan = Math::Tan( Math::ToRadians( camera->GetFovYAngle() / 2 ) );
						float scaleNearY = near * tan;
						float scaleNearX = scaleNearY * camera->GetAspectRatio();
						float scaleFarY = far * tan;
						float scaleFarX = scaleFarY * camera->GetAspectRatio();

						Node* farNode  = _gizmoPerspCamera->GetChildNodeAt( 0 );
						Node* nearNode = _gizmoPerspCamera->GetChildNodeAt( 1 );
						Transform* farTran  = CAST_S( GameObject*, farNode )->GetComponent<Transform>();
						Transform* nearTran = CAST_S( GameObject*, nearNode )->GetComponent<Transform>();
						farTran->SetScaleWorld( Vector3( scaleFarX, scaleFarY, far ) );
						nearTran->SetScaleWorld( Vector3( scaleNearX, scaleNearY, near ) );
					}
					rootTran->SetEmitEvents( TRUE );

					// render gizmo
					renderMngr->Render( 0, ComponentType::MESH_RENDERING, _gizmoPerspCamera, _noLightsGO );
				}
			}

			if ( rigidBody != NULL && rigidBody->IsEnabled() ) {
				// gizmo collision box
				if ( _gizmoCollBox != NULL && rigidBody->GetShapeType() == CollisionShapeType::BOX ) {
					// update transform
					Transform* rootTran = _gizmoCollBox->GetComponent<Transform>();
					rootTran->SetEmitEvents( FALSE );
					{
						rootTran->SetPositionWorld( transform->GetPositionWorld() );
						rootTran->SetRotationWorld( transform->GetRotationWorld() );
						// assume initial size (0.5, 0.5, 0.5)
						Vector3 sizeFactor = rigidBody->GetShapeSize() / Vector3( 0.5f, 0.5f, 0.5f );
						rootTran->SetScaleWorld( transform->GetScaleWorld() * sizeFactor );
					}
					rootTran->SetEmitEvents( TRUE );

					// render gizmo
					renderMngr->Render( 0, ComponentType::MESH_RENDERING, _gizmoCollBox, _noLightsGO );
				}

				// gizmo collision sphere
				if ( _gizmoCollSphere != NULL && rigidBody->GetShapeType() == CollisionShapeType::SPHERE ) {
					// update transform
					Transform* rootTran = _gizmoCollSphere->GetComponent<Transform>();
					rootTran->SetEmitEvents( FALSE );
					{
						rootTran->SetPositionWorld( transform->GetPositionWorld() );
						rootTran->SetRotationWorld( transform->GetRotationWorld() );
						// assume initial size 0.5
						Vector3 scale = transform->GetScaleWorld() * rigidBody->GetShapeSize() / 0.5f;
						// get highest scale value
						float maxScale = (scale.x < scale.y) ? ((scale.y < scale.z) ? scale.z : scale.y) 
															 : ((scale.x < scale.z) ? scale.z : scale.x);
						rootTran->SetScaleWorld( Vector3( maxScale, maxScale, maxScale ) );
					}
					rootTran->SetEmitEvents( TRUE );

					// render gizmo
					renderMngr->Render( 0, ComponentType::MESH_RENDERING, _gizmoCollSphere, _noLightsGO );
				}
			}

		}
	}
}


void Viewport_WithGizmo::_RenderGizmoAfter()
{
	RenderManager* renderMngr = RenderManager::Singleton();
	TimeManager* timeMngr = TimeManager::Singleton();

	// reset depth
	renderMngr->ResetDepth();
	
	// render selected object gizmo
	if ( _selectedGO != NULL ) {
		Transform* transform = _selectedGO->GetComponent<Transform>();

		// render pivot
		if ( transform != NULL && _gizmoPivot != NULL ) {
			// update transform
			Transform* rootTran = _gizmoPivot->GetComponent<Transform>();
			rootTran->SetEmitEvents( FALSE );
			rootTran->SetPositionWorld( transform->GetPositionWorld() );
			rootTran->SetRotationWorld( transform->GetRotationWorld() );
			rootTran->SetEmitEvents( TRUE );

			// render gizmo
			renderMngr->Render( 0, ComponentType::MESH_RENDERING, _gizmoPivot, _noLightsGO );
		}
	}

	// reset depth
	renderMngr->ResetDepth();

	// render debug text
	if ( _gizmoDebugText != NULL ) {
		// update transform
		Transform* rootTran = _gizmoDebugText->GetComponent<Transform>();
		rootTran->SetEmitEvents( FALSE );
		rootTran->SetPositionWorld( _currCameraTran->GetPositionWorld() );
		rootTran->SetRotationWorld( _currCameraTran->GetRotationWorld() );
		rootTran->TranslateByLocal( Vector3( 0, 5, -10 ) );
		rootTran->SetEmitEvents( TRUE );

		// update text
		Text3D* rootText = _gizmoDebugText->GetComponent<Text3D>();
		FiniteString text( "%.2f", (1.0f / timeMngr->GetRenderDeltaTime()) );
		rootText->SetText( text.GetChar() );

		// render gizmo
		renderMngr->Render( 0, ComponentType::TEXT_3D, _gizmoDebugText, _noLightsGO );
	}
}


GameObject* Viewport_WithGizmo::_RecSearchByUID( int uid, GameObject* root )
{
	if ( root == NULL ) {
		return NULL;
	}

	// check uid
	if ( root->GetUniqueID() == uid ) {
		return root;
	}
	
	// parse children
	for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
		GameObject* found = _RecSearchByUID( uid, CAST_S(GameObject*, root->GetChildNodeAt(i)) );
		if ( found != NULL ) {
			return found;
		}
	}

	// not found
	return NULL;
}


bool Viewport_WithGizmo::_RecIsFound( GameObject* target, GameObject* root )
{
	if ( root == NULL ) {
		return false;
	}

	// check root
	if ( target == root ) {
		return true;
	}

	// parse children
	for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
		bool found = _RecIsFound( target, CAST_S(GameObject*, root->GetChildNodeAt(i)) );
		if ( found) {
			return true;
		}
	}

	// not found
	return false;
}


bool Viewport_WithGizmo::_IsPickingInput( Node* root, int selectEvent )
{
	RenderManager* renderMngr = RenderManager::Singleton();
	InputManager* inputMngr = InputManager::Singleton();

	// check target window
	int targetWindow = inputMngr->GetWindowForMouse();

	// check input for mouse down
	if ( inputMngr->KeyDownFirstTime( KeyCode::MOUSE_LEFT ) && targetWindow == _panelIndex ) {
		// use picking rederer
		renderMngr->SwitchRenderer( RendererType::GL_PICKING );
		// clear screen
		renderMngr->ClearScreen( 0, 0, 0 );
		// set camera
		renderMngr->SwitchCamera( _currCameraTran, _currCamera );

		// render
		if ( root != NULL ) {
			// check if root is gameobject
			GameObject* rootObject = CAST_S( GameObject*, root );

			// collect layers
			DataArray<int> layers;
			if ( rootObject != NULL ) {
				_RecCollectLayers( rootObject, layers );
			}
			else {
				for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
					_RecCollectLayers( CAST_S(GameObject*, root->GetChildNodeAt(i)), layers );
				}
			}
			

			// render by layers
			for ( int i = 0; i < layers.Size(); i++ ) {
				// render meshes
				renderMngr->Render( layers[i], ComponentType::MESH_RENDERING, root, _noLightsGO );
				// render text 3d
				renderMngr->Render( layers[i], ComponentType::TEXT_3D, root, _noLightsGO );

				// reset depth
				renderMngr->ResetDepth();
			}


			// get pixel from mouse position
			Vector2 mousePos = inputMngr->MousePosition();
			Vector4 pixel = renderMngr->ReadPixel( mousePos.x, mousePos.y );

			// get object uid
			int uid = pixel.x;
			// find gameobject by uid
			GameObject* gameObject = NULL;
			if ( rootObject != NULL ) {
				gameObject = _RecSearchByUID( uid, rootObject );
			}
			else {
				for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
					gameObject = _RecSearchByUID( uid, CAST_S(GameObject*, root->GetChildNodeAt(i)) );
					if ( gameObject != NULL ) {
						// found
						break;
					}
				}
			}
			
			// if found, send event
			if ( gameObject != NULL ) {
				// select gameobject
				EventManager::Singleton()->EmitEvent( selectEvent, gameObject );
			}
		}

		return true;
	}

	return false;
}

