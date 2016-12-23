// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Script.h"
#include "../../Event/EventManager.h"
#include "../../File/FileManager.h"
#include "../ScriptManager.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"

extern "C" 
{
	#include "Lua_523\Include\lua.h"
	#include "Lua_523\Include\lauxlib.h"
	#include "Lua_523\Include\lualib.h"
};

#include "LuaBridge\Include\LuaBridge.h"


using namespace CYRED;


Script::Script()
	: Asset( AssetType::SCRIPT )
	, _runsInEditor( FALSE )
{
}


Script::~Script()
{
}


void Script::LoadUniqueID()
{
	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_SCRIPT );
	}

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Script>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	Memory::FreeArray( fileData );
}


void Script::LoadFullFile()
{
	Bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_SCRIPT );
	}

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Script>( fileData, this );

	// free memory for file
	Memory::FreeArray( fileData );

	_emitEvents = oldEmitEvents;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}


void Script::ClearAsset()
{
	_isTemporary = TRUE;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}


void Script::CallFunction( const Char* funcName )
{
	// call lua function if exists
	if ( _luaData.Has( funcName ) ) {
		luabridge::LuaRef* luaFunc = _luaData.Get( funcName );
		// check if is function
		if ( luaFunc->isFunction() ) {
			(*luaFunc)();
		}
	}
}


Bool Script::RunsInEditor() const
{
	return _runsInEditor;
}


UInt Script::GetPathsCount() const
{
	return _filePaths.Size();
}


const Char* Script::GetExternalPath( UInt index ) const
{
	ASSERT( index < _filePaths.Size() );
	return _filePaths[index].GetChar();
}


void Script::SetRunInEditor( Bool value )
{
	_runsInEditor = value;
}


void Script::SetLuaData( const Char* varName, luabridge::LuaRef* varData )
{
	_luaData.Set( varName, varData );
}


void Script::SetExternalPath( UInt index, const Char* filePath )
{
	// fill array
	while ( index >= _filePaths.Size() ) {
		_filePaths.Add( "" );
	}

	// set filepath
	_filePaths[index] = filePath;
}
