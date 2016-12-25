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


Asset* Script::Clone()
{
	return _BuildClone( Memory::Alloc<Script>() );
}


void Script::CallFunction( const Char* funcName )
{
	// call lua function if exists
	if ( _luaFuncList.Has( funcName ) ) {
		DataArray<luabridge::LuaRef*>& funcList = _luaFuncList.Get( funcName );

		// get lua state
		lua_State* L = ScriptManager::Singleton()->GetLuaState();

		// bind variables
		//luabridge::setGlobal( L, _luaVarsRef, VARS );

		// call all functions
		for ( UInt i = 0; i < funcList.Size(); i++ ) {
			//(*funcList[i])();
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


Iterator<String, DataArray<luabridge::LuaRef*>> Script::GetFuncListIterator() const
{
	return _luaFuncList.GetIterator();
}


Iterator<String, Script::LuaVar> Script::GetVarsListIterator() const
{
	return _luaVarsList.GetIterator();
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
	_luaFuncList.Clear();

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
		if ( _luaFuncList.Has(funcName) ) {
			_luaFuncList.Get(funcName).Add( Memory::Alloc<luabridge::LuaRef>( luaFunc ) );
		}
		else {
			DataArray<luabridge::LuaRef*> funcList;
			funcList.Add( Memory::Alloc<luabridge::LuaRef>( luaFunc ) );
			_luaFuncList.Set( funcName, funcList );
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
				String					varName( lua_tostring( L, -2 ) );
				DataUnion				varValue;
				DataUnion::ValueType	varType;

				String type = lua_tostring( L, -1 );
				if ( type == TYPE_INT ) {
					varType = DataUnion::ValueType::INT;
					varValue.SetInt( 0 );
				}
				else if ( type == TYPE_FLOAT ) {
					varType = DataUnion::ValueType::FLOAT;
					varValue.SetFloat( 0.0f );
				}
				else if ( type == TYPE_BOOL ) {
					varType = DataUnion::ValueType::BOOL;
					varValue.SetBool( FALSE );
				}
				else if ( type == TYPE_STRING ) {
					varType = DataUnion::ValueType::STRING;
					varValue.SetString( "" );
				}
				else if ( type == TYPE_VECTOR2 ) {
					varType = DataUnion::ValueType::VECTOR2;
					varValue.SetVector2( Vector2() );
				}
				else if ( type == TYPE_VECTOR3 ) {
					varType = DataUnion::ValueType::VECTOR3;
					varValue.SetVector3( Vector3() );
				}
				else if ( type == TYPE_VECTOR4 ) {
					varType = DataUnion::ValueType::VECTOR4;
					varValue.SetVector4( Vector4() );
				}
				else {
					// unknown type error
					DebugManager::Singleton()->Error( "Unknown type." );
					return;
				}

				// add to vars list
				if ( _luaVarsList.Has( varName ) ) {
					// check if same type
					if ( _luaVarsList.Get( varName ).type == varType ) {
						// ignore, already existing
					}
					else {
						// show type error
						DebugManager::Singleton()->Error( "Same variable, different type." );
					}
				}
				else {
					// new variable
					_luaVarsList.Set( varName, LuaVar { varType, varValue } );
				}
			}
			lua_pop(L, 1); // remove value, keep key for lua_next
		}
		lua_pop(L, 1); // pop table
	}
}
