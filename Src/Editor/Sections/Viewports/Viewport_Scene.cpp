// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Viewport_Scene.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "../Settings/EditorSkin.h"

#include "QtWidgets\QComboBox"
#include "QtWidgets\QHBoxLayout"


using namespace CYRED;


Viewport_Scene::Viewport_Scene( int panelIndex )
	: Viewport_WithGizmo( panelIndex )
{
}


void Viewport_Scene::OnEvent( EventType eType, void* eData )
{
	switch ( eType ) {
		case EventType::SELECT_GAMEOBJECT:
		{
			if ( SceneManager::Singleton()->CountLoadedScenes() > 0 ) {
				GameObject* gameObject = CAST_S( GameObject*, eData );

				// get first scene's root
				Node* sceneRoot = SceneManager::Singleton()->GetScene()->GetRoot();

				// check if is displayed
				_selectedGO = NULL;
				for ( int i = 0; i < sceneRoot->GetChildNodeCount(); i++ ) {
					if ( gameObject == sceneRoot->GetChildNodeAt( i ) ) {
						// found
						_selectedGO = gameObject;
					}
				}
			}
			break;
		}
	}
}


void Viewport_Scene::_OnInitialize()
{
	_qtCameraDropdown = Memory::Alloc<QComboBox>();
	_qtTopBarLayout->addWidget( _qtCameraDropdown );

	// register events
	EventManager::Singleton()->RegisterListener( EventType::SELECT_GAMEOBJECT, this );
}


void Viewport_Scene::_OnFinalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::SELECT_GAMEOBJECT, this );
}


void Viewport_Scene::_OnUpdate()
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

		// render gizmo before
		_RenderGizmoBefore();

		// collect lights
		DataArray<GameObject*> lightsGO;
		for ( int i = 0; i < sceneRoot->GetChildNodeCount(); i++ ) {
			_RecCollectLights( CAST_S(GameObject*, sceneRoot->GetChildNodeAt(i)), lightsGO );
		}

		// render meshes
		renderMngr->Render( ComponentType::MESH_RENDERING, sceneRoot, _cameraGO, lightsGO );
		// render morphs
		renderMngr->Render( ComponentType::MORPH_RENDERING, sceneRoot, _cameraGO, lightsGO );
		// render particles
		renderMngr->Render( ComponentType::PARTICLE_EMITTER, sceneRoot, _cameraGO, lightsGO );

		// render gizmo after
		_RenderGizmoAfter();
	}

	// finish
	renderMngr->SwapBuffers();
}
