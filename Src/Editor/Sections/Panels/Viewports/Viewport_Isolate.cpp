// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Viewport_Isolate.h"
#include "CyredModule_Render.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Input.h"

#include "../../../Utils/EditorEvents.h"


using namespace CYRED;


Viewport_Isolate::Viewport_Isolate( int panelIndex )
	: Viewport_WithGizmo( panelIndex )
	, _target( NULL )
{
}


void Viewport_Isolate::OnEvent( int eventType, void* eventData )
{
	switch ( eventType ) {
		case EditorEventType::ISOLATE_OPEN_SCENE:
		case EditorEventType::ISOLATE_OPEN_PREFAB:
			// store data
			_target = CAST_S( GameObject*, eventData );
			_openEventType = eventType;
			_selectedGO = NULL;
			break;

		case EventType::GAMEOBJECT_UPDATE:
			// check if target still attacked
			if ( _target != NULL && _target->GetParentNode() == NULL ) {
				_target = NULL;
				_selectedGO = NULL;
			}
			break;

		case EventType::SCENE_CLOSE:
			if ( _openEventType == EditorEventType::ISOLATE_OPEN_SCENE ) {
				_target = NULL;
				_selectedGO = NULL;
			}
			break;

		case EditorEventType::PREFAB_CLOSE:
			if ( _openEventType == EditorEventType::ISOLATE_OPEN_PREFAB ) {
				_target = NULL;
			}
			break;

		case EditorEventType::ASSET_SELECT:
		case EditorEventType::SCENE_SELECT:
		case EditorEventType::PREFAB_SELECT:
		case EditorEventType::GAMEOBJECT_SELECT:
			if ( _target != NULL ) {
				// unselect
				_selectedGO = NULL;
			}
			break;

		case EditorEventType::ISOLATE_SELECT:
			if ( _target != NULL ) {
				GameObject* gameObject = CAST_S( GameObject*, eventData );
				// check if is displayed
				_selectedGO = NULL;

				// search for gameobject
				if ( _RecIsFound( gameObject, _target ) ) {
					// found
					_selectedGO = gameObject;
				}
			}
			break;
	}
}


cchar* Viewport_Isolate::_GetPanelTitle()
{
	return PANEL_TITLE;
}


void Viewport_Isolate::_OnInitialize()
{
	// register events
	EventManager::Singleton()->RegisterListener( this, EventType::ALL );
}


void Viewport_Isolate::_OnFinalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( this, EventType::ALL );
}


void Viewport_Isolate::_OnUpdate( bool isRuntime )
{
	RenderManager* renderMngr = RenderManager::Singleton();

	// prepare rendering
	if ( !_IsRenderingReady() ) {
		return;
	}

	// check picking
	if ( _IsPickingInput( _target, EditorEventType::ISOLATE_SELECT ) ) {
		return;
	}

	// render prefab
	if ( _target != NULL ) {
		// render gizmo
		_RenderGizmo();

		// collect layers
		DataArray<int> layers;
		_RecCollectLayers( _target, layers );

		// collect lights
		DataArray<GameObject*> lightsGO;
		lightsGO.Add( _currCamera->GetGameObject() );
		_RecCollectLights( _target, lightsGO );

		// render by layers
		for ( int i = 0; i < layers.Size(); i++ ) {
			// render meshes
			renderMngr->Render( layers[i], ComponentType::MESH_RENDERING, _target, lightsGO );
			// render morphs
			renderMngr->Render( layers[i], ComponentType::MORPH_RENDERING, _target, lightsGO );
			// render text 3d
			renderMngr->Render( layers[i], ComponentType::TEXT_3D, _target, lightsGO );
			// render particles
			renderMngr->Render( layers[i], ComponentType::PARTICLE_EMITTER, _target, lightsGO );
		
			// reset depth
			renderMngr->ResetDepth();
		}

		// render gizmo after
		_RenderGizmoAfter();
	}

	// finish
	renderMngr->SwapBuffers();
}
