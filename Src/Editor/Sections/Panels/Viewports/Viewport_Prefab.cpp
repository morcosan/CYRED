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
		{
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
		}

		case EditorEventType::ASSET_SELECT:
		case EditorEventType::PREFAB_SELECT:
		case EditorEventType::SCENE_SELECT:
		{
			if ( _targetPrefab != NULL ) {
				// unselect
				_selectedGO = NULL;
			}
			break;
		}
	}
}


cchar* Viewport_Prefab::_GetPanelTitle()
{
	return PANEL_TITLE;
}


void Viewport_Prefab::_OnInitialize()
{
	// register events
	EventManager::Singleton()->RegisterListener( this, EditorEventType::PREFAB_OPEN );
	EventManager::Singleton()->RegisterListener( this, EditorEventType::PREFAB_CLOSE );
	EventManager::Singleton()->RegisterListener( this, EditorEventType::PREFAB_SELECT );
	EventManager::Singleton()->RegisterListener( this, EditorEventType::SCENE_SELECT );
	EventManager::Singleton()->RegisterListener( this, EditorEventType::ASSET_SELECT );
	EventManager::Singleton()->RegisterListener( this, EditorEventType::GAMEOBJECT_SELECT );
}


void Viewport_Prefab::_OnFinalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( this, EditorEventType::PREFAB_OPEN );
	EventManager::Singleton()->UnregisterListener( this, EditorEventType::PREFAB_CLOSE );
	EventManager::Singleton()->UnregisterListener( this, EditorEventType::PREFAB_SELECT );
	EventManager::Singleton()->UnregisterListener( this, EditorEventType::SCENE_SELECT );
	EventManager::Singleton()->UnregisterListener( this, EditorEventType::ASSET_SELECT );
	EventManager::Singleton()->UnregisterListener( this, EditorEventType::GAMEOBJECT_SELECT );
}


void Viewport_Prefab::_OnUpdate()
{
	RenderManager* renderMngr = RenderManager::Singleton();

	// prepare rendering
	if ( !_IsRenderingReady() ) {
		return;
	}

	// check picking
	if ( _IsPickingInput() ) {
		return;
	}

	// render prefab
	if ( _targetPrefab != NULL ) {
		// get prefabs root
		GameObject* prefabRoot = _targetPrefab->GetRoot();

		// render gizmo before
		_RenderGizmoBefore();

		// collect lights
		DataArray<GameObject*> lightsGO;
		lightsGO.Add( _cameraGO );
		_RecCollectLights( prefabRoot, lightsGO );

		// render meshes
		renderMngr->Render( ComponentType::MESH_RENDERING, prefabRoot, _cameraGO, lightsGO );
		// render morphs
		renderMngr->Render( ComponentType::MORPH_RENDERING, prefabRoot, _cameraGO, lightsGO );
		// render particles
		renderMngr->Render( ComponentType::PARTICLE_EMITTER, prefabRoot, _cameraGO, lightsGO );

		// render gizmo after
		_RenderGizmoAfter();
	}

	// finish
	renderMngr->SwapBuffers();
}


bool Viewport_Prefab::_IsPickingInput()
{
	RenderManager* renderMngr = RenderManager::Singleton();
	InputManager* inputMngr = InputManager::Singleton();

	// check target window
	int targetWindow = inputMngr->GetWindowForCursor();

	// check input for mouse down
	if ( inputMngr->KeyDownFirstTime( KeyCode::MOUSE_LEFT ) && targetWindow == _panelIndex ) {
		// use picking rederer
		renderMngr->SwitchRenderer( RendererType::GL_PICKING );
		// clear screen
		renderMngr->ClearScreen( 0, 0, 0 );

		// render prefab
		if ( _targetPrefab != NULL ) {
			// get prefabs root
			GameObject* prefabRoot = _targetPrefab->GetRoot();

			// render meshes
			renderMngr->Render( ComponentType::MESH_RENDERING, prefabRoot, _cameraGO, _noLightsGO );

			// get pixel from mouse position
			Vector2 mousePos = inputMngr->CursorPosition();
			Vector4 pixel = renderMngr->ReadPixel( mousePos.x, mousePos.y );

			// get object uid
			int uid = pixel.x;
			// find gameobject by uid
			GameObject* gameObject = _RecSearchByUID( uid, prefabRoot );
			// if found
			if ( gameObject != NULL ) {
				// select gameobject
				EventManager::Singleton()->EmitEvent( EditorEventType::GAMEOBJECT_SELECT, gameObject );
			}
		}

		return true;
	}

	return false;
}