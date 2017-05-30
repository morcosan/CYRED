// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Viewport_Game.h"

#include "CyredModule_Render.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"


using namespace CYRED;


Viewport_Game::Viewport_Game( int panelIndex )
	: Panel_Viewport( panelIndex )
{
}


cchar* Viewport_Game::_GetPanelTitle()
{
	return PANEL_TITLE;
}


void Viewport_Game::_OnInitialize()
{
}


void Viewport_Game::_OnFinalize()
{
}


void Viewport_Game::_OnUpdate()
{
	RenderManager* renderMngr = RenderManager::Singleton();

	if ( _mustResize ) {
		renderMngr->OnResize( _canvasSlot );
		_mustResize = FALSE;
	}

	if ( _isFirstUpdate ) {
		// create renderers
		renderMngr->SwitchCanvas( _canvasSlot );
		renderMngr->CreateRenderer( RendererType::GL_FORWARD );
	}

	// set renderer
	renderMngr->SwitchCanvas( _canvasSlot );
	renderMngr->SwitchRenderer( RendererType::GL_FORWARD );
	// clear screen
	renderMngr->ClearScreen( 0, 0, 0 );

	// render scenes
	if ( SceneManager::Singleton()->CountLoadedScenes() > 0 ) {
		// get first scene's root
		Node* sceneRoot = SceneManager::Singleton()->GetScene()->GetRoot();

		// get main camera
		_cameraGO = _RecFindMainCamera( sceneRoot );
		if ( _cameraGO == NULL ) {
			// finish
			renderMngr->SwapBuffers();
			return;
		}

		// update camera size
		{
			Camera* camera = _cameraGO->GetComponent<Camera>();
			// disable events
			bool emitEvents = camera->DoesEmitEvents();
			camera->SetEmitEvents( FALSE );
			// update camera
			float aspectRatio = CAST_S( float, _qtWindow->width() ) / _qtWindow->height();
			float height = camera->GetOrthoSize().y;
			camera->SetAspectRatio( aspectRatio );
			camera->SetOrthoWidth( aspectRatio * height );
			// set back emit events
			camera->SetEmitEvents( emitEvents );
		}
		

		// collect lights
		DataArray<GameObject*> lightsGO;
		_RecCollectLights( sceneRoot, lightsGO );


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


GameObject* Viewport_Game::_RecFindMainCamera( Node* root )
{
	if ( root == NULL ) {
		return NULL;
	}

	// check if gameobject
	GameObject* gameObject = CAST_D( GameObject*, root );
	if ( gameObject != NULL ) {
		// check for camera component
		Camera* camera = gameObject->GetComponent<Camera>();
		if ( camera != NULL && camera->IsEnabled() ) {
			return gameObject;
		}
	}

	// parse children
	for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
		GameObject* found = _RecFindMainCamera( root->GetChildNodeAt(i) );
		if ( found ) {
			return found;
		}
	}

	return NULL;
}


void Viewport_Game::_RecCollectLights( Node* root, DataArray<GameObject*>& lightsGO )
{
	if ( root == NULL ) {
		return;
	}

	// check if gameobject
	GameObject* gameObject = CAST_D( GameObject*, root );
	if ( gameObject != NULL ) {
		// check for light component
		Light* light = gameObject->GetComponent<Light>();
		if ( light != NULL ) {
			lightsGO.Add( gameObject );
		}
	}

	// parse children
	for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
		_RecCollectLights(root->GetChildNodeAt(i), lightsGO );
	}
}
