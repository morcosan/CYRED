// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "PrefabViewport.h"
#include "CyredModule_Event.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "../Settings/EditorSkin.h"

#include "QtWidgets\QComboBox"
#include "QtWidgets\QHBoxLayout"
#include "QtWidgets\qpushbutton.h"


using namespace CYRED;



DataMap<TechniqueType, int> PrefabViewport::_techSlots;


PrefabViewport::PrefabViewport( int panelIndex )
	: Panel_Viewport( panelIndex )
	, _targetPrefab( NULL )
	, _selectedGO( NULL )
{
}


void PrefabViewport::OnEvent( EventType eType, void* eData )
{
	switch ( eType ) {
		case EventType::OPEN_PREFAB:
			_targetPrefab = CAST_S( Prefab*, eData );
			break;

		case EventType::CLOSE_PREFAB:
			if ( _targetPrefab == eData ) {
				_targetPrefab = NULL;
			}
			break;

		case EventType::SELECT_GAMEOBJECT:
		{
			if ( _targetPrefab != NULL ) {
				GameObject* gameObject = CAST_S( GameObject*, eData );
				// check if is displayed
				_selectedGO = NULL;
				for ( int i = 0; i < _targetPrefab->GetRoot()->GetChildNodeCount(); i++ ) {
					if ( gameObject == _targetPrefab->GetRoot()->GetChildNodeAt( i ) ) {
						// found
						_selectedGO = gameObject;
					}
				}
			}
			break;
		}
	}
}


void PrefabViewport::LoadGizmo()
{
	FiniteString gizmoGrid( GIZMO_GRID );
	FiniteString gizmoBackground( GIZMO_BACKGROUND );
	FiniteString gizmoPointLight( GIZMO_POINT_LIGHT );

	// parse prefabs and find gizmo grid
	for ( int i = 0; i < AssetManager::Singleton()->GetPrefabCount(); i++ ) {
		Prefab* prefab = AssetManager::Singleton()->GetPrefabAt( i );

		// check name
		if ( gizmoGrid == prefab->GetName() ) {
			_gizmoGrid = prefab;
		}
		else if ( gizmoBackground == prefab->GetName() ) {
			_gizmoBackground = prefab;
		}
		else if ( gizmoPointLight == prefab->GetName() ) {
			_gizmoPointLight = prefab;
		}
	}
}


void PrefabViewport::SetCamera( GameObject* cameraGO )
{
	_cameraGO = cameraGO;
}


const char* PrefabViewport::_GetPanelTitle()
{
	return PANEL_TITLE;
}


Vector2 PrefabViewport::_GetPanelMinSize()
{
	return MIN_SIZE;
}


void PrefabViewport::_OnInitialize()
{
	// register events
	EventManager::Singleton()->RegisterListener( EventType::OPEN_PREFAB, this );
	EventManager::Singleton()->RegisterListener( EventType::CLOSE_PREFAB, this );
	EventManager::Singleton()->RegisterListener( EventType::SELECT_GAMEOBJECT, this );
}


void PrefabViewport::_OnFinalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::OPEN_PREFAB, this );
	EventManager::Singleton()->UnregisterListener( EventType::CLOSE_PREFAB, this );
	EventManager::Singleton()->UnregisterListener( EventType::SELECT_GAMEOBJECT, this );
}


void PrefabViewport::_OnUpdate()
{
	RenderManager* renderMngr = RenderManager::Singleton();

	if ( _mustResize ) {
		renderMngr->OnResize( _canvasSlot );
		_mustResize = FALSE;
	}

	if ( _isFirstUpdate ) {
		// create renderer
		renderMngr->SwitchCanvas( _canvasSlot );
		renderMngr->CreateRenderer( RendererType::GL_FORWARD );
	}

	// set renderer
	renderMngr->SwitchCanvas( _canvasSlot );
	renderMngr->SwitchRenderer( RendererType::GL_FORWARD );
	// clear screen
	renderMngr->ClearScreen();

	// exit if no camera
	if ( _cameraGO == NULL ) {
		// finish
		renderMngr->SwapBuffers();
		return;
	}

	//! update camera size
	Camera* cam = _cameraGO->GetComponent<Camera>();
	float aspectRatio = CAST_S( float, _qtWindow->width() ) / _qtWindow->height();
	float height = cam->GetOrthoSize().y;
	cam->SetAspectRatio( aspectRatio );
	cam->SetOrthoWidth( aspectRatio * height );

	// render prefab
	if ( _targetPrefab != NULL ) {
		// get prefabs root
		Node* prefabRoot = _targetPrefab->GetRoot();

		// empty lights list
		DataArray<GameObject*> noLightsGO;

		// render gizmo background
		if ( _gizmoBackground != NULL ) {
			renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoBackground->GetRoot(), 
								_cameraGO, noLightsGO );
		}

		// render gizmo grid
		if ( _gizmoGrid != NULL ) {
			renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoGrid->GetRoot(), 
								_cameraGO, noLightsGO );
		}

		// render selected object gizmo
		if ( _selectedGO != NULL ) {
			Light*		light		= _selectedGO->GetComponent<Light>();
			Transform*	transform	= _selectedGO->GetComponent<Transform>();

			// gizmo point light
			if ( _gizmoPointLight != NULL && light != NULL ) {
				if ( light->GetLightType() == LightType::POINT ) {
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

					renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoPointLight->GetRoot(), 
										_cameraGO, noLightsGO );
				}
			}
		}

		// collect lights
		DataArray<GameObject*> lightsGO;
		lightsGO.Add( _cameraGO );
		for ( int i = 0; i < prefabRoot->GetChildNodeCount(); i++ ) {
			_RecCollectLights( CAST_S(GameObject*, prefabRoot->GetChildNodeAt(i)), lightsGO );
		}

		// render meshes
		renderMngr->Render( ComponentType::MESH_RENDERING, prefabRoot, _cameraGO, lightsGO );
		// render morphs
		renderMngr->Render( ComponentType::MORPH_RENDERING, prefabRoot, _cameraGO, lightsGO );
		// render particles
		renderMngr->Render( ComponentType::PARTICLE_EMITTER, prefabRoot, _cameraGO, lightsGO );
	}

	// finish
	renderMngr->SwapBuffers();
}


void PrefabViewport::_RecCollectLights( GameObject* gameObject, DataArray<GameObject*>& lightsGO )
{
	// check for light component
	Light* light = gameObject->GetComponent<Light>();
	if ( light != NULL ) {
		lightsGO.Add( gameObject );
	}

	// parse children
	for ( int i = 0; i < gameObject->GetChildNodeCount(); i++ ) {
		_RecCollectLights( CAST_S(GameObject*, gameObject->GetChildNodeAt(i)), lightsGO );
	}
}