// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Viewport_Prefab.h"
#include "CyredModule_Render.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Input.h"

#include "../../../Utils/EditorEvents.h"


using namespace CYRED;


Viewport_Prefab::Viewport_Prefab( int panelIndex )
	: Viewport_WithGizmo( panelIndex )
	, _targetPrefab( NULL )
{
}


void Viewport_Prefab::OnEvent( int eventType, void* eventData )
{
	switch ( eventType ) {
		case EditorEventType::PREFAB_OPEN:
			_targetPrefab = CAST_S( Prefab*, eventData );
			_selectedGO = NULL;
			break;

		case EditorEventType::PREFAB_CLOSE:
			if ( _targetPrefab == eventData ) {
				_targetPrefab = NULL;
				_selectedGO = NULL;
			}
			break;

		case EditorEventType::GAMEOBJECT_SELECT:
			if ( _targetPrefab != NULL ) {
				GameObject* gameObject = CAST_S( GameObject*, eventData );
				// check if is displayed
				_selectedGO = NULL;

				// get prefab root
				GameObject* prefabRoot = _targetPrefab->GetRoot();

				// search for gameobject
				if ( _RecIsFound( gameObject, prefabRoot ) ) {
					// found
					_selectedGO = gameObject;
				}
			}
			break;

		case EditorEventType::ASSET_SELECT:
		case EditorEventType::PREFAB_SELECT:
		case EditorEventType::SCENE_SELECT:
		case EditorEventType::ISOLATE_SELECT:
			if ( _targetPrefab != NULL ) {
				// unselect
				_selectedGO = NULL;
			}
			break;
	}
}


cchar* Viewport_Prefab::_GetPanelTitle()
{
	return PANEL_TITLE;
}


void Viewport_Prefab::_OnInitialize()
{
	// register events
	EventManager::Singleton()->RegisterListener( this, EventType::ALL );
}


void Viewport_Prefab::_OnFinalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( this, EventType::ALL );
}


void Viewport_Prefab::_OnUpdate( bool isRuntime )
{
	RenderManager* renderMngr = RenderManager::Singleton();

	// prepare rendering
	if ( !_IsRenderingReady() ) {
		return;
	}

	// render prefab
	if ( _targetPrefab != NULL ) {
		// get prefabs root
		GameObject* prefabRoot = _targetPrefab->GetRoot();

		// check picking
		if ( _IsPickingInput( prefabRoot, EditorEventType::GAMEOBJECT_SELECT ) ) {
			return;
		}
		

		// render gizmo
		_RenderGizmo();

		// collect layers
		DataArray<int> layers;
		_RecCollectLayers( prefabRoot, layers );

		// collect lights
		DataArray<GameObject*> lightsGO;
		lightsGO.Add( _currCamera->GetGameObject() );
		_RecCollectLights( prefabRoot, lightsGO );

		// render by layers
		for ( int i = 0; i < layers.Size(); i++ ) {
			// render meshes
			renderMngr->Render( layers[i], ComponentType::MESH_RENDERING, prefabRoot, lightsGO );
			// render morphs
			renderMngr->Render( layers[i], ComponentType::MORPH_RENDERING, prefabRoot, lightsGO );
			// render text 3d
			renderMngr->Render( layers[i], ComponentType::TEXT_3D, prefabRoot, lightsGO );
			// render particles
			renderMngr->Render( layers[i], ComponentType::PARTICLE_EMITTER, prefabRoot, lightsGO );
		
			// reset depth
			renderMngr->ResetDepth();
		}

		// render gizmo after
		_RenderGizmoAfter();
	}

	// finish
	renderMngr->SwapBuffers();
}
