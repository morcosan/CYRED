// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "SceneManagerImpl.h"

#include "../Event/EventManager.h"
#include "../File/FileManager.h"
#include "../Asset/AssetManager.h"
#include "../../2_BuildingBlocks/GameObject.h"
#include "../../2_BuildingBlocks/String/FiniteString.h"
#include "../../2_BuildingBlocks/Random/Random.h"
#include "../../2_BuildingBlocks/Components/Transform.h"
#include "Sections/Scene.h"
#include "../Render/Components/Camera.h" 
#include "../Render/Components/Light.h" 
#include "../Asset/Assets/Prefab.h" 


using namespace CYRED;
using namespace NonAPI;


//! deferred definition of SceneManager
DEFINE_LOCAL_SINGLETON( SceneManager, SceneManagerImpl )
DEFINE_LOCAL_SINGLETON_IMPL( SceneManagerImpl )


void SceneManagerImpl::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	_generatedUID = 0;
	_mainCameraGO = NULL;
}


void SceneManagerImpl::Update( bool isRuntime )
{
	// call update for each scene
	for ( int i = 0; i < _currScenes.Size(); ++i ) {
		_currScenes[i]->OnUpdate( isRuntime );
	}

	// delete objects
	for ( int i = 0; i < _toDestroy.Size(); i++ ) {
		GameObject* gameObject = _toDestroy[i];
		// clear parent
		gameObject->SetParentNode( NULL );
		// send event
		EventManager::Singleton()->PushEvent( EventType::GAMEOBJECT_DELETE, gameObject );
		// clear memory
		PTR_FREE( gameObject );
	}
	// clear list
	_toDestroy.Clear();
}


void SceneManagerImpl::Finalize()
{
	ASSERT( _isInitialized );
}


Scene* SceneManagerImpl::OpenScene( cchar* sceneUID )
{
	ASSERT( _isInitialized );

	CloseAllScenes();

	Scene* scene = AssetManager::Singleton()->GetScene( sceneUID );
	scene->LoadFullFile();
	_currScenes.Add( scene );

	// send event
	EventManager::Singleton()->PushEvent( EventType::SCENE_OPEN, scene );

	return NULL;
}


Scene* SceneManagerImpl::OpenNewScene()
{
	ASSERT( _isInitialized );

	CloseAllScenes();

	Scene* scene = new Scene();
	scene->SetEmitEvents( FALSE );
	scene->SetName( NEW_SCENE );
	scene->SetEmitEvents( TRUE );
	_currScenes.Add( scene );

	// send event
	EventManager::Singleton()->PushEvent( EventType::SCENE_OPEN, scene );

	return scene;
}


Scene* SceneManagerImpl::LoadScene( cchar* sceneUID )
{
	ASSERT( _isInitialized );

	// check if scene exists
	String temp( sceneUID );
	bool found = FALSE;
	for ( int i = 0; i < _currScenes.Size(); ++i ) {
		if ( temp == _currScenes[i]->GetUniqueID() ) {
			found = TRUE;
		}
	}

	// exit if not found
	if ( !found ) {
		return NULL;
	}

	Scene* scene = AssetManager::Singleton()->GetScene( sceneUID );
	scene->LoadFullFile();
	_currScenes.Add( scene );

	// send event
	EventManager::Singleton()->PushEvent( EventType::SCENE_OPEN, scene );
	
	return scene;
}


Scene* SceneManagerImpl::LoadNewScene()
{
	ASSERT( _isInitialized );

	Scene* scene = new Scene();
	scene->SetEmitEvents( FALSE );
	scene->SetName( NEW_SCENE );
	scene->SetEmitEvents( TRUE );
	_currScenes.Add( scene );

	// send event
	EventManager::Singleton()->PushEvent( EventType::SCENE_OPEN, scene );

	return scene;
}


void SceneManagerImpl::SaveScene( cchar* sceneUID )
{
	ASSERT( _isInitialized );

	FileManager* fileManager = FileManager::Singleton();
	Scene* scene = GetScene( sceneUID );

	// get the path
	FiniteString filePath( "%s%s%s", scene->GetDirPath(),
									 scene->GetName(),
									 FileManager::FILE_FORMAT_SCENE );

	fileManager->WriteFile( filePath.GetChar(), fileManager->Serialize<Scene>( scene ).GetChar() );
}


Scene* SceneManagerImpl::SaveSceneAs( cchar* sceneUID, cchar* newSceneName, 
									  cchar* dirPath )
{
	ASSERT( _isInitialized );

	// get existing scene
	Scene* scene = GetScene( sceneUID );
	ASSERT( scene != NULL );

	// uids
	String newUID = Random::GenerateUniqueID();
	String oldUID = scene->GetUniqueID();

	// create new asset scene
	Scene* newScene = new Scene();
	newScene->SetEmitEvents( FALSE );
	newScene->SetName( newSceneName );
	newScene->SetDirPath( dirPath );
	newScene->SetUniqueID( newUID.GetChar() );
	newScene->SetIsTemporary( FALSE );
	newScene->SetEmitEvents( TRUE );

	AssetManager::Singleton()->AddScene( newScene );
	
	// save new scene to file
	FiniteString filePath( "%s%s%s", dirPath,
								     newSceneName,
								     FileManager::FILE_FORMAT_SCENE );

	// write file using new uid
	scene->SetUniqueID( newUID.GetChar() );
	FileManager* fileManager = FileManager::Singleton();
	fileManager->WriteFile( filePath.GetChar(), fileManager->Serialize<Scene>( scene ).GetChar() );
	scene->SetUniqueID( oldUID.GetChar() );

	// return new scene
	return newScene;
}


void SceneManagerImpl::SaveAllScenes()
{
	ASSERT( _isInitialized );

	FileManager* fileManager = FileManager::Singleton();

	for ( int i = 0; i < _currScenes.Size(); ++i )
	{
		FiniteString filePath( "%s%s%s", _currScenes[i]->GetDirPath(),
									     _currScenes[i]->GetName(),
									     FileManager::FILE_FORMAT_SCENE );

		fileManager->WriteFile( filePath.GetChar(), 
								fileManager->Serialize<Scene>( _currScenes[i] ).GetChar() );
	}
}


void SceneManagerImpl::CloseScene( cchar* sceneUID )
{
	ASSERT( _isInitialized );

	String temp( sceneUID );

	for ( int i = 0; i < _currScenes.Size(); ++i ) {
		Scene* scene = _currScenes[i];
		if ( temp == scene->GetUniqueID() ) {
			scene->ClearRoot();
			_currScenes.Erase( i );

			// clear destroy list
			_toDestroy.Clear();

			// send event
			EventManager::Singleton()->PushEvent( EventType::SCENE_CLOSE, scene );

			return;
		}
	}
}


void SceneManagerImpl::CloseAllScenes()
{
	ASSERT( _isInitialized );

	for ( int i = 0; i < _currScenes.Size(); ++i )	{
		_currScenes[i]->ClearRoot();
	}
	_currScenes.Clear();

	// clear destroy list
	_toDestroy.Clear();

	// send event
	EventManager::Singleton()->PushEvent( EventType::SCENE_CLOSE, NULL );
}


void SceneManagerImpl::StoreScenes()
{
	ASSERT( _isInitialized );

	// remove all stored scenes
	_storedScenes.Clear();

	// serialize each scene and store result
	for ( int i = 0; i < _currScenes.Size(); i++ ) {
		_storedScenes.Add( StoredScene {
			_currScenes[i],
			FileManager::Singleton()->Serialize<Scene>( _currScenes[i] )
		} );
	}
}


void SceneManagerImpl::RestoreScenes()
{
	ASSERT( _isInitialized );

	// clear everything
	for ( int i = 0; i < _currScenes.Size(); ++i )	{
		_currScenes[i]->ClearRoot();
	}
	_currScenes.Clear();

	// clear destroy list
	_toDestroy.Clear();

	// load stored scenes
	for ( int i = 0; i < _storedScenes.Size(); i++ ) {
		// deserialize scene data
		FileManager::Singleton()->Deserialize<Scene>( _storedScenes[i].data.GetChar(),
													  _storedScenes[i].scene );
		// add scene
		_currScenes.Add( _storedScenes[i].scene );
	}
}


GameObject* SceneManagerImpl::NewGameObject( int sceneIndex )
{
	ASSERT( _isInitialized );

	Scene* scene = GetScene( sceneIndex );
	ASSERT( scene != NULL );

	GameObject* newObject = new GameObject( EMPTY_GAMEOBJECT, NextGameObjectUID() );
	newObject->SetInScene( TRUE );
	scene->GetRoot()->AddChildNode( newObject );

	// send event
	EventManager::Singleton()->PushEvent( EventType::GAMEOBJECT_CREATE, newObject );

	return newObject;
}


GameObject* SceneManagerImpl::NewGameObject( cchar* sceneUID )
{
	ASSERT( _isInitialized );
	
	Scene* scene = GetScene( sceneUID );
	ASSERT( scene != NULL );

	GameObject* newObject = new GameObject( EMPTY_GAMEOBJECT, NextGameObjectUID() );
	newObject->SetInScene( TRUE );
	scene->GetRoot()->AddChildNode( newObject );

	// send event
	EventManager::Singleton()->PushEvent( EventType::GAMEOBJECT_CREATE, newObject );

	return newObject;
}


GameObject* SceneManagerImpl::Instantiate( const Prefab* prefab, int sceneIndex )
{
	ASSERT( _isInitialized );
	
	if ( prefab != NULL && prefab->GetRoot() != NULL ) {
		Scene* scene = GetScene( sceneIndex );
		ASSERT( scene != NULL );

		// create game object for root
		GameObject* prefabRoot = prefab->GetRoot();
		// create object
		GameObject* newObject = new GameObject( NextGameObjectUID() );
		newObject->SetInScene( TRUE );
		// clone from prefab
		prefabRoot->Clone( newObject );
		// add to scene
		scene->GetRoot()->AddChildNode( newObject );

		// update hierarchy
		EventManager::Singleton()->PushEvent( EventType::GAMEOBJECT_CREATE, newObject );

		return newObject;
	}
	
	return NULL;
}


GameObject* SceneManagerImpl::Duplicate( const GameObject* object )
{
	ASSERT( _isInitialized );

	// clone gameobject
	GameObject* newObject = SceneManager::Singleton()->NewGameObject();
	newObject->SetInScene( TRUE );
	// clone attributes
	object->Clone( newObject );
	// update name
	newObject->SetName( object->GetName() );

	return newObject;
}


GameObject* SceneManagerImpl::Search( cchar* objectName, cchar* sceneUID )
{
	ASSERT( _isInitialized );

	Scene* scene = GetScene( sceneUID );
	ASSERT( scene != NULL );

	// find first gameobject with name
	FiniteString searchName( objectName );
	Node* sceneRoot = scene->GetRoot();
	for ( int i = 0; i < sceneRoot->GetChildNodeCount(); i++ ) {
		GameObject* childGO = CAST_S( GameObject*, sceneRoot->GetChildNodeAt( i ) );
		// check name
		if ( searchName == childGO->GetName() ) {
			// found
			return childGO;
		}
	}

	// nothing found
	return NULL;
}


GameObject* SceneManagerImpl::FindGameObject( cchar* objectName )
{
	// search in all opened scenes
	for ( int i = 0; i < _currScenes.Size(); i++ ) {
		GameObject* found = Search( objectName, _currScenes[i]->GetUniqueID() );
		if ( found != NULL ) {
			return found;
		}
	}

	// not found
	return NULL;
}


void SceneManagerImpl::Destroy( GameObject* gameObject )
{
	ASSERT( _isInitialized );

	if ( gameObject != NULL ) {
		// mark for destuction at the end of frame
		_toDestroy.Add( gameObject );
	}
}


int SceneManagerImpl::GetSceneIndex( cchar* sceneUID )
{
	ASSERT( _isInitialized );

	String temp( sceneUID );

	for ( int i = 0; i < _currScenes.Size(); ++i )
	{
		if ( temp == _currScenes[i]->GetUniqueID() )
		{
			return i;
		}
	}

	return INVALID_INDEX;
}


Scene* SceneManagerImpl::GetScene( cchar* sceneUID )
{
	ASSERT( _isInitialized );

	// in case of NULL name, return first scene
	// this also solves the 0 == NULL problem
	if ( sceneUID == NULL )
	{
		return GetScene();
	}

	String temp( sceneUID );

	for ( int i = 0; i < _currScenes.Size(); ++i )
	{
		if ( temp ==_currScenes[i]->GetUniqueID() )
		{
			return _currScenes[i];
		}
	}

	return NULL;
}


Scene* SceneManagerImpl::GetScene( int sceneIndex )
{
	ASSERT( _isInitialized );
	ASSERT( sceneIndex < _currScenes.Size() );

	return _currScenes[sceneIndex];
}


int SceneManagerImpl::CountLoadedScenes()
{
	ASSERT( _isInitialized );
	return _currScenes.Size();
}


int SceneManagerImpl::NextGameObjectUID()
{
	ASSERT( _isInitialized );
	++_generatedUID;
	return _generatedUID;
}


void SceneManagerImpl::SetMainCamera( GameObject* cameraGO )
{
	ASSERT( _isInitialized );
	_mainCameraGO = cameraGO;
}


GameObject* SceneManagerImpl::GetMainCamera()
{
	ASSERT( _isInitialized );

	if ( _mainCameraGO == NULL ) {
		// search for first active camera
		for ( int i = 0; i < _currScenes.Size(); ++i )	{
			Node* root = _currScenes[i]->GetRoot();

			_mainCameraGO = _RecFindActiveCamera( root );

			if ( _mainCameraGO != NULL ) {
				break;
			}
		}
	}

	return _mainCameraGO;
}


void SceneManagerImpl::FindClosestLights( GameObject* target, OUT DataArray<GameObject*>& lightsGO )
{
	// parse all scenes
	for ( int i = 0; i < _currScenes.Size(); i++ ) {
		Node* root = _currScenes[i]->GetRoot();

		// parse all gameobjects
		for ( int j = 0; j < root->GetChildNodeCount(); j++ ) {
			GameObject* gameobject = CAST_S( GameObject*, root->GetChildNodeAt( j ) );

			// ignore target gameobject
			if ( gameobject != target && gameobject->IsEnabled() ) {
				Light* light = gameobject->GetComponent<Light>();

				// check if gameobject has active light
				if ( light != NULL && light->IsEnabled() ) {
					// add to list
					lightsGO.Add( gameobject );
				}
			}
		}
	}
}


GameObject* SceneManagerImpl::_RecFindActiveCamera( Node* parent )
{
	for ( int i = 0; i < parent->GetChildNodeCount(); ++i )
	{
		GameObject* gameObject = CAST_S( GameObject*, parent->GetChildNodeAt( i ) );

		if ( gameObject->IsEnabled() )
		{
			Camera* camera = gameObject->GetComponent<Camera>();
			Transform* transform = gameObject->GetComponent<Transform>();

			if ( camera != NULL && camera->IsEnabled() && 
				 transform != NULL && transform->IsEnabled() )
			{
				return gameObject;
			}
			else
			{
				GameObject* found = _RecFindActiveCamera( gameObject );

				if ( found != NULL )
				{
					return found;
				}
			}
		}
	}

	return NULL;
}
