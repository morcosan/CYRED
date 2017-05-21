// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "SceneViewport.h"
#include "CyredModule_Event.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "../Settings/EditorSkin.h"

#include "QtWidgets\QComboBox"
#include "QtWidgets\QHBoxLayout"
#include "QtWidgets\qpushbutton.h"


using namespace CYRED;



DataMap<TechniqueType, int> SceneViewport::_techSlots;


SceneViewport::SceneViewport( int panelIndex )
	: Panel_Viewport( panelIndex )
	, _selectedGO( NULL )
{
}


void SceneViewport::OnEvent( EventType eType, void* eData )
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


void SceneViewport::LoadGizmo()
{
	FiniteString gizmoGrid( GIZMO_GRID );
	FiniteString gizmoPointLight( GIZMO_POINT_LIGHT );

	// parse prefabs and find gizmo grid
	for ( int i = 0; i < AssetManager::Singleton()->GetPrefabCount(); i++ ) {
		Prefab* prefab = AssetManager::Singleton()->GetPrefabAt( i );

		// check name
		if ( gizmoGrid == prefab->GetName() ) {
			_gizmoGrid = prefab;
		}
		else if ( gizmoPointLight == prefab->GetName() ) {
			_gizmoPointLight = prefab;
		}
	}
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

	// register events
	EventManager::Singleton()->RegisterListener( EventType::SELECT_GAMEOBJECT, this );
}


void SceneViewport::_OnFinalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::SELECT_GAMEOBJECT, this );
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

		// empty lights list
		DataArray<GameObject*> noLightsGO;

		// render gizmo grid
		if ( _gizmoGrid != NULL ) {
			renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoGrid->GetRoot(), 
								_cameraGO, noLightsGO );
		}

		// render selected object gizmo
		if ( _selectedGO != NULL ) {
			Light*		light		= _selectedGO->GetComponent<Light>();
			Transform*	transform	= _selectedGO->GetComponent<Transform>();

			// gizmo point light
			if ( _gizmoPointLight != NULL && light != NULL ) {
				if ( light->GetLightType() == LightType::POINT ) {
					// update transform
					Node* root = _gizmoPointLight->GetRoot();
					for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
						GameObject* childGO = CAST_S( GameObject*, root->GetChildNodeAt(i) );
						Transform* childTran = childGO->GetComponent<Transform>();
						childTran->SetEmitEvents( FALSE );
						childTran->SetPositionWorld( transform->GetPositionWorld() );
						childTran->SetRotationWorld( transform->GetRotationWorld() );
						float range = light->GetRange();
						childTran->SetScaleWorld( Vector3( range, range, range ) );
						childTran->SetEmitEvents( TRUE );
					}

					renderMngr->Render( ComponentType::MESH_RENDERING, _gizmoPointLight->GetRoot(), 
										_cameraGO, noLightsGO );
				}
			}
		}

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

