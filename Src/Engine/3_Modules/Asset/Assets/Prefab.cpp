// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Prefab.h"
#include "../../File/FileManager.h"
#include "../../Event/EventManager.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"


using namespace CYRED;


Prefab::Prefab()
	: Asset( AssetType::PREFAB )
{
}


void Prefab::LoadUniqueID()
{
	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_MATERIAL );
	}

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Prefab>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	Memory::FreeArray( fileData );
}


void Prefab::LoadFullFile()
{
	Bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_PREFAB );
	}

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
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
	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}

Asset* Prefab::Clone()
{
	return _BuildClone( Memory::Alloc<Prefab>() );
}


const Char* Prefab::GetExtension()
{
	if ( _useExtension ) {
		return FileManager::FILE_FORMAT_PREFAB;
	}
	return NULL;
}
