// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "SceneManagerImpl.h"
#include "../Event/EventManager.h"
#include "../File/FileManager.h"
#include "../Asset/AssetManager.h"
#include "../../2_BuildingBlocks/GameObject.h"
#include "../../2_BuildingBlocks/Random/Random.h"
#include "Fragments/Scene.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition of SceneManager
DEFINE_LOCAL_SINGLETON( SceneManager, SceneManagerImpl )

DEFINE_LOCAL_SINGLETON_IMPL( SceneManagerImpl )



void SceneManagerImpl::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	_generatedUID = 0;
}


void SceneManagerImpl::Finalize()
{
	ASSERT( _isInitialized );
	CloseAllScenes();
}


Scene* SceneManagerImpl::OpenScene( const Char* sceneUID )
{
	ASSERT( _isInitialized );

	CloseAllScenes();

	Scene* scene = AssetManager::Singleton()->GetScene( sceneUID );
	scene->LoadFullFile();
	_currScenes.Add( scene );

	EventManager::Singleton()->EmitEvent( EventType::SCENE, EventName::HIERARCHY_CHANGED, NULL );

	return NULL;
}


Scene* SceneManagerImpl::OpenNewScene()
{
	ASSERT( _isInitialized );

	CloseAllScenes();

	Scene* scene = Memory::Alloc<Scene>();
	scene->SetEmitEvents( FALSE );
	scene->SetName( NEW_SCENE );
	scene->SetEmitEvents( TRUE );
	_currScenes.Add( scene );

	EventManager::Singleton()->EmitEvent( EventType::SCENE, EventName::HIERARCHY_CHANGED, NULL );

	return scene;
}


Scene* SceneManagerImpl::LoadScene( const Char* sceneUID )
{
	ASSERT( _isInitialized );

	String temp( sceneUID );

	for ( UInt i = 0; i < _currScenes.Size(); ++i )
	{
		if ( temp == _currScenes[i]->GetUniqueID() )
		{
			return NULL;
		}
	}

	Scene* scene = AssetManager::Singleton()->GetScene( sceneUID );
	scene->LoadFullFile();
	_currScenes.Add( scene );

	EventManager::Singleton()->EmitEvent( EventType::SCENE, EventName::HIERARCHY_CHANGED, NULL );
	
	return scene;
}


Scene* SceneManagerImpl::LoadNewScene()
{
	ASSERT( _isInitialized );

	Scene* scene = Memory::Alloc<Scene>();
	scene->SetEmitEvents( FALSE );
	scene->SetName( NEW_SCENE );
	scene->SetEmitEvents( TRUE );
	_currScenes.Add( scene );

	EventManager::Singleton()->EmitEvent( EventType::SCENE, EventName::HIERARCHY_CHANGED, NULL );

	return scene;
}


void SceneManagerImpl::SaveScene( const Char* sceneUID )
{
	ASSERT( _isInitialized );

	FileManager* fileManager = FileManager::Singleton();
	Scene* scene = GetScene( sceneUID );

	// get the path
	Char filePath[ MAX_SIZE_CUSTOM_STRING ];
	CUSTOM_STRING( filePath, "%s%s%s", 
				   scene->GetDirPath(),
				   scene->GetName(),
				   FileManager::FILE_FORMAT_SCENE );

	fileManager->WriteFile( filePath, fileManager->Serialize<Scene>( scene ).GetChar() );
}


Scene* SceneManagerImpl::SaveSceneAs( const Char* sceneUID, const Char* newSceneName, 
									  const Char* dirPath )
{
	ASSERT( _isInitialized );

	// create new asset scene
	Scene* scene = GetScene( sceneUID );
	ASSERT( scene != NULL );

	if ( !scene->IsTemporary() )
	{
		UInt sceneIndex = GetSceneIndex( sceneUID );
		_currScenes.Erase( sceneIndex );

		Scene* newScene = Memory::Alloc<Scene>();
		_currScenes.Add( newScene );

		// move all game objects to new scene
		for ( UInt i = 0; i < scene->GetRoot()->GetChildNodeCount(); ++i )
		{
			newScene->GetRoot()->AddChildNode( scene->GetRoot()->GetChildNodeAt( i ) );
		}

		scene = newScene;
	}

	scene->SetEmitEvents( FALSE );
	scene->SetName( newSceneName );
	scene->SetDirPath( dirPath );
	scene->SetUniqueID( Random::GenerateUniqueID().GetChar() );
	scene->SetIsTemporary( FALSE );
	scene->SetEmitEvents( TRUE );

	AssetManager::Singleton()->AddScene( scene );
	
		// save new scene to file
	Char filePath[ MAX_SIZE_CUSTOM_STRING ];
	CUSTOM_STRING( filePath, "%s%s%s", 
				   dirPath,
				   newSceneName,
				   FileManager::FILE_FORMAT_SCENE );

	FileManager* fileManager = FileManager::Singleton();
	fileManager->WriteFile( filePath, fileManager->Serialize<Scene>( scene ).GetChar() );

	return scene;
}


void SceneManagerImpl::SaveAllScenes()
{
	ASSERT( _isInitialized );

	FileManager* fileManager = FileManager::Singleton();

	for ( UInt i = 0; i < _currScenes.Size(); ++i )
	{
		Char filePath[ MAX_SIZE_CUSTOM_STRING ];
		CUSTOM_STRING( filePath, "%s%s%s", 
					   _currScenes[i]->GetDirPath(),
					   _currScenes[i]->GetName(),
					   FileManager::FILE_FORMAT_SCENE );

		fileManager->WriteFile( filePath, fileManager->Serialize<Scene>( _currScenes[i] ).GetChar() );
	}
}


void SceneManagerImpl::CloseScene( const Char* sceneUID )
{
	ASSERT( _isInitialized );

	String temp( sceneUID );

	for ( UInt i = 0; i < _currScenes.Size(); ++i )
	{
		if ( temp == _currScenes[i]->GetUniqueID() )
		{
			 _currScenes[i]->ClearAsset();
			_currScenes.Erase( i );

			EventManager::Singleton()->EmitEvent( EventType::SCENE, EventName::HIERARCHY_CHANGED, NULL );

			return;
		}
	}
}


void SceneManagerImpl::CloseAllScenes()
{
	ASSERT( _isInitialized );

	for ( UInt i = 0; i < _currScenes.Size(); ++i )
	{
		_currScenes[i]->ClearAsset();
	}
	_currScenes.Clear();

	EventManager::Singleton()->EmitEvent( EventType::SCENE, EventName::HIERARCHY_CHANGED, NULL );
}


GameObject* SceneManagerImpl::NewGameObject( UInt sceneIndex )
{
	ASSERT( _isInitialized );

	Scene* scene = GetScene( sceneIndex );
	ASSERT( scene != NULL );

	GameObject* newObject = Memory::Alloc<GameObject>( EMPTY_GAMEOBJECT, NextGameObjectUID() );
	scene->GetRoot()->AddChildNode( newObject );

	EventManager::Singleton()->EmitEvent( EventType::SCENE, EventName::HIERARCHY_CHANGED, NULL );

	return newObject;
}


GameObject* SceneManagerImpl::NewGameObject( const Char* sceneUID )
{
	ASSERT( _isInitialized );
	
	Scene* scene = GetScene( sceneUID );
	ASSERT( scene != NULL );

	GameObject* newObject = Memory::Alloc<GameObject>( EMPTY_GAMEOBJECT, NextGameObjectUID() );
	scene->GetRoot()->AddChildNode( newObject );

	EventManager::Singleton()->EmitEvent( EventType::SCENE, EventName::HIERARCHY_CHANGED, NULL );

	return newObject;
}


GameObject* SceneManagerImpl::Duplicate( const GameObject* object )
{
	ASSERT( _isInitialized );
	// TODO
	return NULL;
}


GameObject* SceneManagerImpl::Search( const Char* objectName, UInt sceneIndex )
{
	ASSERT( _isInitialized );

	Scene* scene = GetScene( sceneIndex );
	ASSERT( scene != NULL );

	return NULL;
}


GameObject* SceneManagerImpl::Search( const Char* objectName, const Char* sceneUID )
{
	ASSERT( _isInitialized );

	Scene* scene = GetScene( sceneUID );
	ASSERT( scene != NULL );

	return NULL;
}


void SceneManagerImpl::Destroy( GameObject* object )
{
	ASSERT( _isInitialized );

}


Int SceneManagerImpl::GetSceneIndex( const Char* sceneUID )
{
	ASSERT( _isInitialized );

	String temp( sceneUID );

	for ( UInt i = 0; i < _currScenes.Size(); ++i )
	{
		if ( temp == _currScenes[i]->GetUniqueID() )
		{
			return i;
		}
	}

	return INVALID_INDEX;
}


Scene* SceneManagerImpl::GetScene( const Char* sceneUID )
{
	ASSERT( _isInitialized );

	// in case of NULL name, return first scene
	// this also solves the 0 == NULL problem
	if ( sceneUID == NULL )
	{
		return GetScene();
	}

	String temp( sceneUID );

	for ( UInt i = 0; i < _currScenes.Size(); ++i )
	{
		if ( temp ==_currScenes[i]->GetUniqueID() )
		{
			return _currScenes[i];
		}
	}

	return NULL;
}


Scene* SceneManagerImpl::GetScene( UInt sceneIndex )
{
	ASSERT( _isInitialized );
	ASSERT( sceneIndex < _currScenes.Size() );

	return _currScenes[sceneIndex];
}


UInt SceneManagerImpl::CountLoadedScenes()
{
	ASSERT( _isInitialized );
	return _currScenes.Size();
}


UInt SceneManagerImpl::NextGameObjectUID()
{
	++_generatedUID;
	return _generatedUID;
}

