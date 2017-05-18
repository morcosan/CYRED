// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "SceneViewport.h"
#include "CyredModule_Event.h"
#include "CyredModule_Scene.h"
#include "../Settings/EditorSkin.h"

#include "QtWidgets\QComboBox"
#include "QtWidgets\QHBoxLayout"
#include "QtWidgets\qpushbutton.h"


using namespace CYRED;



DataMap<TechniqueType, int> SceneViewport::_techSlots;


SceneViewport::SceneViewport( int panelIndex )
	: Panel_Viewport( panelIndex )
{
}


void SceneViewport::SetCamera( GameObject* cameraGO )
{
	_cameraGO = cameraGO;
}


void SceneViewport::A_CameraButton()
{
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, _cameraGO );
}


const char* SceneViewport::_GetPanelTitle()
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


void SceneViewport::_OnFinalize()
{
}


void SceneViewport::_OnUpdate()
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
	
	// render scenes
	if ( SceneManager::Singleton()->CountLoadedScenes() > 0 ) {
		// get first scene's root
		Node* sceneRoot = SceneManager::Singleton()->GetScene()->GetRoot();

		// collect lights
		DataArray<GameObject*> lightsGO;
		for ( int i = 0; i < sceneRoot->GetChildNodeCount(); i++ ) {
			_RecCollectLights( CAST_S(GameObject*, sceneRoot->GetChildNodeAt(i)), lightsGO );
		}

		// render meshes
		renderMngr->Render( ComponentType::MESH_RENDERING, sceneRoot, _cameraGO, lightsGO.Data() );
		// render morphs
		renderMngr->Render( ComponentType::MORPH_RENDERING, sceneRoot, _cameraGO, lightsGO.Data() );
		// render particles
		renderMngr->Render( ComponentType::PARTICLE_EMITTER, sceneRoot, _cameraGO, lightsGO.Data() );
	}

	// finish
	renderMngr->SwapBuffers();
}


void SceneViewport::_RecCollectLights( GameObject* gameObject, DataArray<GameObject*>& lightsGO )
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
