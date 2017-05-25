// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Viewport_WithGizmo.h"
#include "CyredModule_Render.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "../Settings/EditorSkin.h"

#include "QtWidgets\QComboBox"
#include "QtWidgets\QHBoxLayout"
#include "QtWidgets\qpushbutton.h"


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
	FiniteString gizmoGrid( GIZMO_GRID );
	FiniteString gizmoAxis( GIZMO_AXIS );
	FiniteString gizmoBackground( GIZMO_BACKGROUND );
	FiniteString gizmoPointLight( GIZMO_POINT_LIGHT );
	FiniteString gizmoDirLight( GIZMO_DIR_LIGHT );
	FiniteString gizmoSpotLight( GIZMO_SPOT_LIGHT );
	FiniteString gizmoOrthoCamera( GIZMO_ORTHO_CAMERA );
	FiniteString gizmoPerspCamera( GIZMO_PERSP_CAMERA );
	FiniteString gizmoPivot( GIZMO_PIVOT );

	// parse prefabs and find gizmo grid
	for ( int i = 0; i < AssetManager::Singleton()->GetPrefabCount(); i++ ) {
		Prefab* prefab = AssetManager::Singleton()->GetPrefabAt( i );

		// check name
		if ( gizmoGrid == prefab->GetName() ) {
			_gizmoGrid = prefab;
		}
		else if ( gizmoAxis == prefab->GetName() ) {
			_gizmoAxis = prefab;
		}
		else if ( gizmoBackground == prefab->GetName() ) {
			_gizmoBackground = prefab;
		}
		else if ( gizmoPointLight == prefab->GetName() ) {
			_gizmoPointLight = prefab;
		}
		else if ( gizmoDirLight == prefab->GetName() ) {
			_gizmoDirLight = prefab;
		}
		else if ( gizmoSpotLight == prefab->GetName() ) {
			_gizmoSpotLight = prefab;
		}
		else if ( gizmoOrthoCamera == prefab->GetName() ) {
			_gizmoOrthoCamera = prefab;
		}
		else if ( gizmoPerspCamera == prefab->GetName() ) {
			_gizmoPerspCamera = prefab;
		}
		else if ( gizmoPivot == prefab->GetName() ) {
			_gizmoPivot = prefab;
		}
	}
}


void Viewport_WithGizmo::SetCamera( GameObject* cameraGO )
{
	_cameraGO = cameraGO;
}


Vector2 Viewport_WithGizmo::_GetPanelMinSize()
{
	return MIN_SIZE;
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
	if ( _cameraGO == NULL ) {
		// finish
		renderMngr->SwapBuffers();
		return FALSE;
	}

	//! update camera size
	Camera* cam = _cameraGO->GetComponent<Camera>();
	float aspectRatio = CAST_S( float, _qtWindow->width() ) / _qtWindow->height();
	float height = cam->GetOrthoSize().y;
	cam->SetAspectRatio( aspectRatio );
	cam->SetOrthoWidth( aspectRatio * height );

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


void Viewport_WithGizmo::_RenderGizmoBefore()
{
	RenderManager* renderMngr = RenderManager::Singleton();

	// render gizmo background
	if ( _gizmoBackground != NULL ) {
		renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoBackground->GetRoot(), 
							_cameraGO, _noLightsGO );
	}

	// render gizmo grid
	if ( _gizmoGrid != NULL ) {
		renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoGrid->GetRoot(), 
							_cameraGO, _noLightsGO );
	}

	// render gizmo axis
	if ( _gizmoAxis != NULL ) {
		renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoAxis->GetRoot(), 
							_cameraGO, _noLightsGO );
	}

	// render selected object gizmo
	if ( _selectedGO != NULL ) {
		Light*		light		= _selectedGO->GetComponent<Light>();
		Camera*		camera		= _selectedGO->GetComponent<Camera>();
		Transform*	transform	= _selectedGO->GetComponent<Transform>();

		if ( light != NULL && transform != NULL ) {
			// gizmo point light
			if ( _gizmoPointLight != NULL && light->GetLightType() == LightType::POINT ) {
				// update transform
				Node* root = _gizmoPointLight->GetRoot();
				for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
					GameObject* childGO = CAST_S( GameObject*, root->GetChildNodeAt(i) );
					Transform* childTran = childGO->GetComponent<Transform>();
					childTran->SetEmitEvents( FALSE );

					childTran->SetPositionWorld( transform->GetPositionWorld() );
					childTran->SetRotationWorld( transform->GetRotationWorld() );
					float range = light->GetRange();
					childTran->SetScaleWorld( Vector3( range, range, range ) );

					childTran->SetEmitEvents( TRUE );
				}
				// render gizmo
				renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoPointLight->GetRoot(), 
									_cameraGO, _noLightsGO );
			}

			// gizmo directional light
			if ( _gizmoDirLight != NULL && light->GetLightType() == LightType::DIRECTIONAL ) {
				// update transform
				Node* root = _gizmoDirLight->GetRoot();
				for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
					GameObject* childGO = CAST_S( GameObject*, root->GetChildNodeAt(i) );
					Transform* childTran = childGO->GetComponent<Transform>();
					childTran->SetEmitEvents( FALSE );

					childTran->SetPositionWorld( transform->GetPositionWorld() );
					childTran->SetRotationWorld( transform->GetRotationWorld() );

					childTran->SetEmitEvents( TRUE );
				}
				// render gizmo
				renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoDirLight->GetRoot(), 
									_cameraGO, _noLightsGO );
			}

			// gizmo spot light
			if ( _gizmoSpotLight != NULL && light->GetLightType() == LightType::SPOT ) {
				// update transform
				Node* root = _gizmoSpotLight->GetRoot();
				for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
					GameObject* childGO = CAST_S( GameObject*, root->GetChildNodeAt(i) );
					Transform* childTran = childGO->GetComponent<Transform>();
					childTran->SetEmitEvents( FALSE );

					childTran->SetPositionWorld( transform->GetPositionWorld() );
					childTran->SetRotationWorld( transform->GetRotationWorld() );
					float tan = Math::Tan( Math::ToRadians( light->GetSpotAngle() / 2 ) );
					float range = light->GetRange();
					float scaleBase = range * tan;
					childTran->SetScaleWorld( Vector3( scaleBase, scaleBase, range ) );

					childTran->SetEmitEvents( TRUE );
				}
				// render gizmo
				renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoSpotLight->GetRoot(), 
									_cameraGO, _noLightsGO );
			}
		}

		if ( camera != NULL ) {
			// gizmo ortho camera
			if ( _gizmoOrthoCamera != NULL && camera->GetCameraType() == CameraType::ORTHOGRAPHIC ) {
				// update transform
				Node* root = _gizmoOrthoCamera->GetRoot();
				for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
					GameObject* childGO = CAST_S( GameObject*, root->GetChildNodeAt( i ) );
					Transform* childTran = childGO->GetComponent<Transform>();
					childTran->SetEmitEvents( FALSE );

					childTran->SetPositionWorld( transform->GetPositionWorld() );
					childTran->SetRotationWorld( transform->GetRotationWorld() );

					childTran->SetEmitEvents( TRUE );
				}
				// render gizmo
				renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoOrthoCamera->GetRoot(),
									_cameraGO, _noLightsGO );
			}

			// gizmo perspective camera
			if ( _gizmoPerspCamera != NULL && camera->GetCameraType() == CameraType::PERSPECTIVE ) {
				// update transform
				Node* root = _gizmoPerspCamera->GetRoot();
				for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
					GameObject* childGO = CAST_S( GameObject*, root->GetChildNodeAt( i ) );
					Transform* childTran = childGO->GetComponent<Transform>();
					childTran->SetEmitEvents( FALSE );

					childTran->SetPositionWorld( transform->GetPositionWorld() );
					childTran->SetRotationWorld( transform->GetRotationWorld() );

					childTran->SetEmitEvents( TRUE );
				}
				// render gizmo
				renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoPerspCamera->GetRoot(),
									_cameraGO, _noLightsGO );
			}
		}
	}
}


void Viewport_WithGizmo::_RenderGizmoAfter()
{
	RenderManager* renderMngr = RenderManager::Singleton();

	// reset depth
	renderMngr->ResetDepth();
	
	// render selected object gizmo
	if ( _selectedGO != NULL ) {
		Transform*	transform	= _selectedGO->GetComponent<Transform>();

		// render pivot
		if ( transform != NULL && _gizmoPivot != NULL ) {
			// update transform
			Node* root = _gizmoPivot->GetRoot();
			for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
				GameObject* childGO = CAST_S( GameObject*, root->GetChildNodeAt(i) );
				Transform* childTran = childGO->GetComponent<Transform>();
				childTran->SetEmitEvents( FALSE );

				childTran->SetPositionWorld( transform->GetPositionWorld() );
				childTran->SetRotationWorld( transform->GetRotationWorld() );

				childTran->SetEmitEvents( TRUE );
			}
			// render gizmo
			renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoPivot->GetRoot(), 
								_cameraGO, _noLightsGO );
		}
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

