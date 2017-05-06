// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "PrefabViewport.h"
#include "CyredModule_Event.h"
#include "CyredModule_Scene.h"
#include "../Settings/EditorSkin.h"

#include "QtWidgets\QComboBox"
#include "QtWidgets\QHBoxLayout"
#include "QtWidgets\qpushbutton.h"


using namespace CYRED;



DataMap<TechniqueType, UInt> PrefabViewport::_techSlots;


PrefabViewport::PrefabViewport( UInt panelIndex )
	: Panel_Viewport( panelIndex )
{
}


void PrefabViewport::SetCamera( GameObject* cameraGO )
{
	_cameraGO = cameraGO;
}


UInt PrefabViewport::GetSlotForTechnique( TechniqueType type )
{
	if ( _techSlots.Has( type ) ) {
		return _techSlots.Get( type );
	}

	UInt techSlot = RenderManager::Singleton()->NewTechnique( type );
	_techSlots.Set( type, techSlot );

	return techSlot;
}


const Char* PrefabViewport::_GetPanelTitle()
{
	return PANEL_TITLE;
}


Vector2 PrefabViewport::_GetPanelMinSize()
{
	return MIN_SIZE;
}


void PrefabViewport::_OnInitialize()
{
}


void PrefabViewport::_OnUpdate()
{
	RenderManager* renderMngr = RenderManager::Singleton();

	if ( _mustResize ) {
		renderMngr->OnResize( _canvasSlot );
		_mustResize = FALSE;
	}

	if ( _isFirstUpdate ) {
		UInt techSlot = GetSlotForTechnique( TechniqueType::FORWARD_BASIC );
		renderMngr->ChangeRenderer( _canvasSlot, RendererType::GL_FORWARD );
		renderMngr->ChangeTechnique( _canvasSlot, techSlot );
	}

	if ( _cameraGO == NULL ) {
		return;
	}

	//! update camera size
	Camera* cam = _cameraGO->GetComponent<Camera>();
	Float aspectRatio = CAST_S( Float, _qtWindow->width() ) / _qtWindow->height();
	Float height = cam->GetOrthoSize().y;
	cam->SetAspectRatio( aspectRatio );
	cam->SetOrthoWidth( aspectRatio * height );

	// render scenes
	SceneManager* sceneMngr = SceneManager::Singleton();
	if ( sceneMngr->CountLoadedScenes() > 0 ) {
		//for ( UInt i = 0; i < sceneMngr->CountLoadedScenes(); ++i )
		{
			Scene* scene = sceneMngr->GetScene();
			renderMngr->Render( _canvasSlot, scene->GetRoot(), _cameraGO );
		}
	}
	else {
		renderMngr->Render( _canvasSlot, NULL, _cameraGO );
	}
}
