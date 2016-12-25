// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Asset.h"
#include "../3_Modules/Event/EventManager.h"


using namespace CYRED;



Asset::Asset( AssetType type )
	: _emitEvents( TRUE )
	, _assetType( type )
	, _isTemporary( TRUE )
	, _useExtension( TRUE )
{
}


const Char* Asset::GetName() const
{
	return _name.GetChar();
}


const Char* Asset::GetDirPath() const
{
	return _dirPath.GetChar();
}


AssetType Asset::GetAssetType() const
{
	return _assetType;
}


const Char* Asset::GetUniqueID() const
{
	return _uniqueID.GetChar();
}


Bool Asset::IsTemporary() const
{
	return _isTemporary;
}


Bool Asset::DoesEmitEvents() const
{
	return _emitEvents;
}


void Asset::SetName( const Char* name, Bool useExtension )
{
	_name = name;
	_useExtension = useExtension;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::ASSET, 
											  EventName::ASSET_CHANGED, 
											  this );
	}
}


void Asset::SetDirPath( const Char* dirPath )
{
	_dirPath = dirPath;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::ASSET, 
											  EventName::ASSET_CHANGED, 
											  this );
	}
}


void Asset::SetEmitEvents( Bool value )
{
	_emitEvents = value;
}


void Asset::SetUniqueID( const Char* uniqueID )
{
	// this should not emit events
	_uniqueID = uniqueID;
}


void Asset::SetIsTemporary( Bool value )
{
	// this should not emit events
	_isTemporary = value;
}


Asset* Asset::_BuildClone( Asset* clone ) const
{
	clone->SetEmitEvents( FALSE );
	clone->SetName( _name.GetChar(), _useExtension );
	clone->SetDirPath( _dirPath.GetChar() );
	clone->SetEmitEvents( TRUE );

	return clone;
}

