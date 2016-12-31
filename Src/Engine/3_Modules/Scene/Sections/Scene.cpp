#include "Scene.h"
// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Scene.h"
#include "../../File/FileManager.h"
#include "../../../2_BuildingBlocks/Composite/Node.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"


using namespace CYRED;



Scene::Scene()
	: Asset( AssetType::SCENE )
	, _root( Memory::Alloc<Node>() )
{
}


Scene::~Scene()
{
	Memory::Free( _root );
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
	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Scene>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	Memory::FreeArray( fileData );
}


void Scene::LoadFullFile()
{
	Bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	ClearAsset();

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_SCENE );
	}

	// read the file
	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Scene>( fileData, this );

	// free memory for file
	Memory::FreeArray( fileData );

	_emitEvents = oldEmitEvents;
}


void Scene::ClearAsset()
{
	Memory::Free( _root );
	_root = Memory::Alloc<Node>();
}


Asset* Scene::Clone()
{
	return _BuildClone( Memory::Alloc<Scene>() );
}


void Scene::OnUpdate( Bool isRuntime )
{
	for ( UInt i = 0; i < _root->GetChildNodeCount(); ++i )
	{
		_root->GetChildNodeAt( i )->OnUpdate( isRuntime );
	}
}


Node* Scene::GetRoot()
{
	ASSERT( _root != NULL );
	return _root;
}

