// Copyright (c) 2015 Morco (www.morco.ro)
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
{
}


void PrefabViewport::OnEvent( EventType eType, void* eData )
{
	switch ( eType ) {
		case EventType::OPEN_PREFAB:
			_targetPrefab = CAST_S( Prefab*, eData );
			break;
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
}


void PrefabViewport::_OnFinalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::OPEN_PREFAB, this );
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

		// collect lights
		DataArray<GameObject*> lightsGO;
		lightsGO.Add( _cameraGO );

		// render meshes
		renderMngr->Render( ComponentType::MESH_RENDERING, prefabRoot, _cameraGO, lightsGO.Data() );
		// render morphs
		renderMngr->Render( ComponentType::MORPH_RENDERING, prefabRoot, _cameraGO, lightsGO.Data() );
		// render particles
		renderMngr->Render( ComponentType::PARTICLE_EMITTER, prefabRoot, _cameraGO, lightsGO.Data() );
	}

	// finish
	renderMngr->SwapBuffers();
}
