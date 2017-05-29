// Copyright (c) 2015-2017 Morco (www.morco.ro)
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


cchar* Asset::GetName() const
{
	return _name.GetChar();
}


cchar* Asset::GetDirPath() const
{
	return _dirPath.GetChar();
}


AssetType Asset::GetAssetType() const
{
	return _assetType;
}


cchar* Asset::GetUniqueID() const
{
	return _uniqueID.GetChar();
}


bool Asset::IsTemporary() const
{
	return _isTemporary;
}


bool Asset::DoesEmitEvents() const
{
	return _emitEvents;
}


void Asset::SetName( cchar* name, bool useExtension )
{
	_name = name;
	_useExtension = useExtension;

	// on rename callback
	_OnRename();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Asset::SetDirPath( cchar* dirPath )
{
	_dirPath = dirPath;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Asset::SetEmitEvents( bool value )
{
	_emitEvents = value;
}


void Asset::SetUniqueID( cchar* uniqueID )
{
	// this should not emit events
	_uniqueID = uniqueID;
}


void Asset::SetIsTemporary( bool value )
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

