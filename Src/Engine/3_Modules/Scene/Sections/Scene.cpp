// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Scene.h"

#include "../../File/FileManager.h"
#include "../../../2_BuildingBlocks/Node.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"
#include "../../Event/EventManager.h"


using namespace CYRED;


Scene::Scene()
	: Asset( AssetType::SCENE )
	, _root( new Node() )
{
}


Scene::~Scene()
{
	PTR_FREE( _root );
	_root = NULL;
}


void Scene::LoadUniqueID()
{
	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_SCENE );
	}

	// read the file
	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Scene>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	ARRAY_FREE( fileData );
}


void Scene::LoadFullFile()
{
	bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// delete previous data
	ClearRoot();

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_SCENE );
	}

	// read the file
	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Scene>( fileData, this );

	// free memory for file
	ARRAY_FREE( fileData );

	_emitEvents = oldEmitEvents;
}


void Scene::ClearAsset()
{
	_isTemporary = TRUE;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, this );
	}
}


Asset* Scene::Clone()
{
	return _BuildClone( new Scene() );
}


cchar* CYRED::Scene::GetExtension()
{
	if ( _useExtension ) {
		return FileManager::FILE_FORMAT_SCENE;
	}

	return NULL;
}


void Scene::OnUpdate( bool isRuntime )
{
	// ask gameobjects to update 
	for ( int i = 0; i < _root->GetChildNodeCount(); ++i ) {
		_root->GetChildNodeAt( i )->OnUpdate( isRuntime );
	}
}


Node* Scene::GetRoot() const
{
	ASSERT( _root != NULL );
	return _root;
}


void Scene::ClearRoot()
{
	PTR_FREE( _root );
	_root = new Node();
}

