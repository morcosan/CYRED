// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "SceneViewport.h"
#include "CyredModule_Event.h"
#include "CyredModule_Scene.h"
#include "../EditorSkin.h"

#include "QtWidgets\QComboBox"
#include "QtWidgets\QHBoxLayout"
#include "QtWidgets\qpushbutton.h"


using namespace CYRED;



DataMap<TechniqueType, UInt> SceneViewport::_techSlots;


SceneViewport::SceneViewport( UInt panelIndex )
	: ViewportPanel( panelIndex )
{
}


void SceneViewport::A_CameraButton()
{
	EventManager::Singleton()->EmitEvent( EventType::SCENE, EventName::GAMEOBJECT_SELECTED, _cameraGO );
}


UInt SceneViewport::GetSlotForTechnique( TechniqueType type )
{
	if ( _techSlots.Has( type ) )
	{
		return _techSlots.Get( type );
	}

	UInt techSlot = RenderManager::Singleton()->NewTechnique( type );
	_techSlots.Set( type, techSlot );

	return techSlot;
}


const Char* SceneViewport::_GetPanelTitle()
{
	return PANEL_TITLE;
}


Vector2 SceneViewport::_GetPanelMinSize()
{
	return MIN_SIZE;
}


void SceneViewport::_OnInitialize()
{
	_qtCameraDropdown = Memory::Alloc<QComboBox>();

	_qtCameraButton =  Memory::Alloc<QPushButton>();
	_qtCameraButton->setText( "C" );
	_qtCameraButton->setMaximumWidth( 30 );
	_qtCameraButton->setObjectName( EditorSkin::VIEWPORT_BUTTON );
	QObject::connect( _qtCameraButton,	&QPushButton::pressed, this, &SceneViewport::A_CameraButton );

	_qtTopBarLayout->addWidget( _qtCameraDropdown );
	_qtTopBarLayout->addWidget( _qtCameraButton );
}


void SceneViewport::_OnUpdate()
{
	RenderManager* renderMngr = RenderManager::Singleton();

	if ( _mustResize ) 
	{
		renderMngr->OnResize( _canvasSlot );
		_mustResize = false;
	}

	if ( _isFirstUpdate )
	{
		UInt techSlot = GetSlotForTechnique( TechniqueType::FORWARD_BASIC );
		renderMngr->ChangeRenderer( _canvasSlot, RendererType::GL_FORWARD );
		renderMngr->ChangeTechnique( _canvasSlot, techSlot );
	}

	if ( _cameraGO == NULL )
	{
		return;
	}

	//! update camera size
	COMP::Camera* cam = _cameraGO->GetComponent<COMP::Camera>();
	float aspectRatio = (float) _qtWindow->width() / _qtWindow->height();
	float height = cam->GetOrthoSize().y;
	cam->SetAspectRatio( aspectRatio );
	cam->SetOrthoSize( Vector2( aspectRatio * height, height ) );
	

	SceneManager* sceneMngr = SceneManager::Singleton();

	if ( sceneMngr->CountLoadedScenes() > 0 )
	{
		//for ( UInt i = 0; i < sceneMngr->CountLoadedScenes(); ++i )
		{
			Scene* scene = sceneMngr->GetScene();
			renderMngr->RenderScene( _canvasSlot, scene, _cameraGO );
		}
	}
	else
	{
		renderMngr->RenderScene( _canvasSlot, NULL, _cameraGO );
	}
}
