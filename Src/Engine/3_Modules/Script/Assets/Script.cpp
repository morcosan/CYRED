// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Script.h"
#include "../../Event/EventManager.h"
#include "../../File/FileManager.h"
#include "../../Debug/DebugManager.h"
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
	, _isFirstUpdate( TRUE )
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
	if ( _luaFunc.Has( funcName ) ) {
		DataArray<luabridge::LuaRef*>& funcList = _luaFunc.Get( funcName );

		// get lua state
		lua_State* L = ScriptManager::Singleton()->GetLuaState();

		// bind variables
		luabridge::setGlobal( L, _luaVarsRef, VARS );

		// call all functions
		for ( UInt i = 0; i < funcList.Size(); i++ ) {
			(*funcList[i])();
		}
	}
}


Bool Script::RunsInEditor() const
{
	return _runsInEditor;
}


Bool Script::IsFirstUpdate() const
{
	return _isFirstUpdate;
}


UInt Script::GetPathsCount() const
{
	return _filePaths.Size();
}


const Char* Script::GetFilePath( UInt index ) const
{
	ASSERT( index < _filePaths.Size() );
	return _filePaths[index].GetChar();
}


void Script::SetRunInEditor( Bool value )
{
	_runsInEditor = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}


void Script::SetFirstUpdate( Bool value )
{
	_isFirstUpdate = value;
	// does not emit event
}


void Script::SetFilePath( UInt index, const Char* filePath )
{
	// fill array
	while ( index >= _filePaths.Size() ) {
		_filePaths.Add( "" );
	}

	// set filepath
	_filePaths[index] = filePath;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}


void Script::ClearFilePaths()
{
	// empty list
	_filePaths.Clear();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}


void Script::LoadLuaFiles()
{
	// reset flag
	_isFirstUpdate = TRUE;

	// clear function list
	_luaFunc.Clear();

	// clear var list
	_luaVarsList.Clear();

	// recreate dictionary
	// get lua state
	lua_State* L = ScriptManager::Singleton()->GetLuaState();
	_luaVarsRef = Memory::Alloc<luabridge::LuaRef>( L );
	_luaVarsRef->newTable( L );

	// load all lua files
	for ( UInt i = 0; i < _filePaths.Size(); i++ ) {
		// create path
		FiniteString filePath( "%s%s", _dirPath.GetChar(), _filePaths[i].GetChar() );
		// read file
		Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
		// load lua
		_LoadLuaData( fileData );
		// free memory for file
		Memory::FreeArray( fileData );
	}

}

void Script::_LoadLuaData( const Char* luaData )
{
	// get lua state
	lua_State* L = ScriptManager::Singleton()->GetLuaState();

	// load script data
	switch ( luaL_loadstring( L, luaData ) )	{
		case LUA_ERRSYNTAX:
		{
			// syntax error
			DebugManager::Singleton()->Log( lua_tostring( L, -1 ) );
			break;
		}

		case LUA_OK:
		{
			// script loaded
			// run lua
			if ( lua_pcall( L, 0, 0, 0 ) ) {
				// run error
				DebugManager::Singleton()->Log( lua_tostring( L, -1 ) );
				break;
			}
			else {
				// run lua: success
				// load default functions
				_AddLuaFunc( FUNC_ONSTART );
				_AddLuaFunc( FUNC_ONUPDATE );

				// load variables
				_LoadLuaVar();
			}
		}
	}
}


void Script::_AddLuaFunc( const Char* funcName )
{
	// get lua state
	lua_State* L = ScriptManager::Singleton()->GetLuaState();

	// get ref
	luabridge::LuaRef luaFunc = luabridge::getGlobal( L, funcName );

	// check if exists
	if ( !luaFunc.isNil() && luaFunc.isFunction() ) {
		// add func
		if ( _luaFunc.Has(funcName) ) {
			_luaFunc.Get(funcName).Add( Memory::Alloc<luabridge::LuaRef>( luaFunc ) );
		}
		else {
			DataArray<luabridge::LuaRef*> funcList;
			funcList.Add( Memory::Alloc<luabridge::LuaRef>( luaFunc ) );
			_luaFunc.Set( funcName, funcList );
		}
	}
}


void Script::_LoadLuaVar()
{
	// get lua state
	lua_State* L = ScriptManager::Singleton()->GetLuaState();

	// get ref
	luabridge::LuaRef luaVars = luabridge::getGlobal( L, VARS );
	luabridge::setGlobal( L, luabridge::Nil(), VARS );

	// check if exists
	if ( !luaVars.isNil() && luaVars.isTable() ) {
		// iterate lua table
		luabridge::push( L, luaVars ); // push table
		lua_pushnil(L);  // push nil, so lua_next removes it from stack and puts (k, v) on stack
		while ( lua_next(L, -2) != 0 ) { // -2, because we have table at -1
			if ( lua_isstring(L, -2) ) { // only store stuff with string keys
				//result.emplace(lua_tostring(L, -2), LuaRef::fromStack(L, -1));
				LuaVar luaVar;
				luaVar.name = lua_tostring( L, -2 );

				String type = lua_tostring( L, -1 );
				if ( type == TYPE_INT ) {
					luaVar.type = DataUnion::ValueType::INT;
				}
				else if ( type == TYPE_FLOAT ) {
					luaVar.type = DataUnion::ValueType::FLOAT;
				}
				else if ( type == TYPE_BOOL ) {
					luaVar.type = DataUnion::ValueType::BOOL;
				}
				else if ( type == TYPE_STRING ) {
					luaVar.type = DataUnion::ValueType::STRING;
				}
				else if ( type == TYPE_VECTOR2 ) {
					luaVar.type = DataUnion::ValueType::VECTOR2;
				}
				else if ( type == TYPE_VECTOR3 ) {
					luaVar.type = DataUnion::ValueType::VECTOR3;
				}
				else if ( type == TYPE_VECTOR4 ) {
					luaVar.type = DataUnion::ValueType::VECTOR4;
				}
				else if ( type == TYPE_REFERENCE ) {
					luaVar.type = DataUnion::ValueType::REFERENCE;
				}

				// add to vars list
				_luaVarsList.Add( luaVar );
			}
			lua_pop(L, 1); // remove value, keep key for lua_next
		}
		lua_pop(L, 1); // pop table
	}
}
