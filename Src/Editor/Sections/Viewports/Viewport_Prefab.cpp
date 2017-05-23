// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Viewport_Prefab.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"


using namespace CYRED;


Viewport_Prefab::Viewport_Prefab( int panelIndex )
	: Viewport_WithGizmo( panelIndex )
	, _targetPrefab( NULL )
{
}


void Viewport_Prefab::OnEvent( EventType eType, void* eData )
{
	switch ( eType ) {
		case EventType::OPEN_PREFAB:
			_targetPrefab = CAST_S( Prefab*, eData );
			_selectedGO = NULL;
			break;

		case EventType::CLOSE_PREFAB:
			if ( _targetPrefab == eData ) {
				_targetPrefab = NULL;
				_selectedGO = NULL;
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


void Viewport_Prefab::_OnInitialize()
{
	// register events
	EventManager::Singleton()->RegisterListener( EventType::OPEN_PREFAB, this );
	EventManager::Singleton()->RegisterListener( EventType::CLOSE_PREFAB, this );
	EventManager::Singleton()->RegisterListener( EventType::SELECT_GAMEOBJECT, this );
}


void Viewport_Prefab::_OnFinalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::OPEN_PREFAB, this );
	EventManager::Singleton()->UnregisterListener( EventType::CLOSE_PREFAB, this );
	EventManager::Singleton()->UnregisterListener( EventType::SELECT_GAMEOBJECT, this );
}


void Viewport_Prefab::_OnUpdate()
{
	RenderManager* renderMngr = RenderManager::Singleton();

	// prepare rendering
	if ( !_IsRenderingReady() ) {
		return;
	}

	// render prefab
	if ( _targetPrefab != NULL ) {
		// get prefabs root
		Node* prefabRoot = _targetPrefab->GetRoot();

		// render gizmo
		_RenderGizmo();

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
