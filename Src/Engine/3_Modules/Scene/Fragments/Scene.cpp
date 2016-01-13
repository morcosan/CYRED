#include "Scene.h"
// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Scene.h"
#include "../../File/FileManager.h"
#include "../../../2_BuildingBlocks/Composite/Node.h"


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
	Char filePath[ MAX_SIZE_CUSTOM_STRING ];
	CUSTOM_STRING( filePath, "%s%s%s", 
				   _dirPath.GetChar(),
				   _name.GetChar(),
				   FileManager::FILE_FORMAT_SCENE );

	// read the file
	Char* fileData = FileManager::Singleton()->ReadFile( filePath );
	FileManager::Singleton()->Deserialize<Scene>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	Memory::FreeArray( fileData );
}


void Scene::LoadFullFile()
{
	Bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	ClearRoot();

	Char filePath[ MAX_SIZE_CUSTOM_STRING ];
	CUSTOM_STRING( filePath, "%s%s%s", 
				   _dirPath.GetChar(),
				   _name.GetChar(),
				   FileManager::FILE_FORMAT_SCENE );

	// read the file
	Char* fileData = FileManager::Singleton()->ReadFile( filePath );
	FileManager::Singleton()->Deserialize<Scene>( fileData, this );

	// free memory for file
	Memory::FreeArray( fileData );

	_emitEvents = oldEmitEvents;
}


void Scene::OnStart( Bool isRuntime )
{
}


void Scene::OnUpdate( Bool isRuntime )
{
}


void Scene::SetMainCamera( const Char* objectName )
{
}


void Scene::ClearRoot()
{
	Memory::Free( _root );
	_root = Memory::Alloc<Node>();
}


Node* Scene::GetRoot()
{
	ASSERT( _root != NULL );
	return _root;
}

