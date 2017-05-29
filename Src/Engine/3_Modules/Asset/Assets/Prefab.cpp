// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Prefab.h"
#include "../../File/FileManager.h"
#include "../../Event/EventManager.h"
#include "../../Scene/SceneManager.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"
#include "../../../2_BuildingBlocks/GameObject.h"


using namespace CYRED;


Prefab::Prefab()
	: Asset( AssetType::PREFAB )
	, _root( NULL )
{
}


void Prefab::LoadUniqueID()
{
	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_PREFAB );
	}

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Prefab>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	Memory::FreeArray( fileData );
}


void Prefab::LoadFullFile()
{
	bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create root
	CreateRoot();

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_PREFAB );
	}

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Prefab>( fileData, this );

	// free memory for file
	Memory::FreeArray( fileData );

	_emitEvents = oldEmitEvents;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Prefab::ClearAsset()
{
	// delete root
	Memory::Free( _root );
	_root = NULL;

	_isTemporary = TRUE; 

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


Asset* Prefab::Clone()
{
	return _BuildClone( Memory::Alloc<Prefab>() );
}


cchar* Prefab::GetExtension()
{
	if ( _useExtension ) {
		return FileManager::FILE_FORMAT_PREFAB;
	}
	return NULL;
}


GameObject* Prefab::GetRoot() const
{
	return _root;
}


void Prefab::CreateRoot()
{
	// delete root
	Memory::Free( _root );
	_root = NULL;

	_root = Memory::Alloc<GameObject>( _name.GetChar(), SceneManager::Singleton()->NextGameObjectUID() );
}


void Prefab::_OnRename()
{
	// update gameobject name
	if ( _root != NULL ) {
		_root->SetName( _name.GetChar() );
	}
}