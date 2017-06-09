// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Viewport_Scene.h"

#include "CyredModule_Render.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Input.h"

#include "../../../Utils/EditorEvents.h"

#include "QtWidgets\QComboBox"
#include "QtWidgets\QHBoxLayout"


using namespace CYRED;


Viewport_Scene::Viewport_Scene( int panelIndex )
	: Viewport_WithGizmo( panelIndex )
{
}


void Viewport_Scene::OnEvent( int eventType, void* eventData )
{
	switch ( eventType ) {
		case EditorEventType::GAMEOBJECT_SELECT:
			if ( SceneManager::Singleton()->CountLoadedScenes() > 0 ) {
				GameObject* gameObject = CAST_S( GameObject*, eventData );

				// get first scene's root
				Node* sceneRoot = SceneManager::Singleton()->GetScene()->GetRoot();

				// check if is displayed
				_selectedGO = NULL;

				// search for gameobject
				for ( int i = 0; i < sceneRoot->GetChildNodeCount(); i++ ) {
					GameObject* root = CAST_S( GameObject*, sceneRoot->GetChildNodeAt(i) );
					if ( _RecIsFound( gameObject, root ) ) {
						// found
						_selectedGO = gameObject;
					}
				}
			}
			break;

		case EventType::GAMEOBJECT_DELETE:
		{
			GameObject* gameObject = CAST_S( GameObject*, eventData );
			if ( _selectedGO == gameObject ) {
				// deselect
				_selectedGO = NULL;
			}
			break;
		}

		case EditorEventType::ASSET_SELECT:
		case EditorEventType::PREFAB_SELECT:
		case EditorEventType::SCENE_SELECT:
		case EditorEventType::ISOLATE_SELECT:
			if ( SceneManager::Singleton()->CountLoadedScenes() > 0 ) {
				// unselect
				_selectedGO = NULL;
			}
			break;
	}
}

cchar* Viewport_Scene::_GetPanelTitle()
{
	return PANEL_TITLE;
}


void Viewport_Scene::_OnInitialize()
{
	_qtCameraDropdown = new QComboBox();
	_qtTopBarLayout->addWidget( _qtCameraDropdown );

	// register events
	EventManager::Singleton()->RegisterListener( this, EditorEventType::GAMEOBJECT_SELECT );
	EventManager::Singleton()->RegisterListener( this, EditorEventType::ASSET_SELECT );
	EventManager::Singleton()->RegisterListener( this, EditorEventType::PREFAB_SELECT );
	EventManager::Singleton()->RegisterListener( this, EditorEventType::SCENE_SELECT );
}


void Viewport_Scene::_OnFinalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( this, EditorEventType::GAMEOBJECT_SELECT );
	EventManager::Singleton()->UnregisterListener( this, EditorEventType::ASSET_SELECT );
	EventManager::Singleton()->UnregisterListener( this, EditorEventType::PREFAB_SELECT );
	EventManager::Singleton()->UnregisterListener( this, EditorEventType::SCENE_SELECT );
}


void Viewport_Scene::_OnUpdate( bool isRuntime )
{
	RenderManager* renderMngr = RenderManager::Singleton();

	// prepare rendering
	if ( !_IsRenderingReady() ) {
		return;
	}

	// render scenes
	if ( SceneManager::Singleton()->CountLoadedScenes() > 0 ) {
		// get first scene's root
		Node* sceneRoot = SceneManager::Singleton()->GetScene()->GetRoot();

		// check picking
		if ( _IsPickingInput( sceneRoot, EditorEventType::GAMEOBJECT_SELECT ) ) {
			return;
		}


		// render gizmo
		_RenderGizmo();

		// collect layers
		DataArray<int> layers;
		for ( int i = 0; i < sceneRoot->GetChildNodeCount(); i++ ) {
			_RecCollectLayers( CAST_S(GameObject*, sceneRoot->GetChildNodeAt(i)), layers );
		}

		// collect lights
		DataArray<GameObject*> lightsGO;
		for ( int i = 0; i < sceneRoot->GetChildNodeCount(); i++ ) {
			_RecCollectLights( CAST_S(GameObject*, sceneRoot->GetChildNodeAt(i)), lightsGO );
		}

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

		// render gizmo after
		_RenderGizmoAfter();
	}

	// finish
	renderMngr->SwapBuffers();
}
