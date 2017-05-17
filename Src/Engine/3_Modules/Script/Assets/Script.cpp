// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Script.h"
#include "../../Event/EventManager.h"
#include "../../File/FileManager.h"
#include "../../Debug/DebugManager.h"
#include "../ScriptManager.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"
#include "../../Asset/Assets/Prefab.h"

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

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Script>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	Memory::FreeArray( fileData );
}


void Script::LoadFullFile()
{
	bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_SCRIPT );
	}

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Script>( fileData, this );

	// free memory for file
	Memory::FreeArray( fileData );

	_emitEvents = oldEmitEvents;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Script::ClearAsset()
{
	_isTemporary = TRUE;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


Asset* Script::Clone()
{
	return _BuildClone( Memory::Alloc<Script>() );
}


const char* CYRED::Script::GetExtension()
{
	if ( _useExtension ) {
		return FileManager::FILE_FORMAT_SCRIPT;
	}

	return NULL;
}


void Script::CallFunction( const char* funcName, GameObject* gameObject )
{
	// call lua function if exists
	if ( _luaFuncList.Has( funcName ) ) {
		DataArray<LuaFunc>& funcList = _luaFuncList.Get( funcName );

		// bind globals
		lua_State* L = ScriptManager::Singleton()->GetLuaState();
		// bind varibles
		luabridge::setGlobal( L, *_luaVarsRef, GLOBAL_VARS );
		// bind gameobject
		luabridge::LuaRef goRef( L, gameObject );
		luabridge::setGlobal( L, goRef, GLOBAL_GAMEOBJECT );

		// call all functions
		for ( int i = 0; i < funcList.Size(); i++ ) {
			// if is broken do not call
			if ( !funcList[i].isBroken ) {
				// try to call the function
				try {
					(*funcList[i].funcRef)();
				}
				// handle error
				catch ( luabridge::LuaException const& e ) {
					// display error once
					DebugManager::Singleton()->Error( e.what () );
					// mark as broken
					funcList[i].isBroken = TRUE;
				}
			}
		}
	}
}


void Script::SetVariable( const char* varName, DataUnion varValue )
{
	_luaVarsList.Set( varName, varValue );

	switch ( varValue.GetValueType() ) {
		case DataUnion::INT:
			(*_luaVarsRef)[varName] = varValue.GetInt();
			break;

		case DataUnion::FLOAT:
			(*_luaVarsRef)[varName] = varValue.GetFloat();
			break;

		case DataUnion::BOOL:
			(*_luaVarsRef)[varName] = varValue.GetBool();
			break;

		case DataUnion::STRING:
			(*_luaVarsRef)[varName] = varValue.GetString();
			break;

		case DataUnion::VECTOR2:
			(*_luaVarsRef)[varName] = varValue.GetVector2();
			break;

		case DataUnion::VECTOR3:
			(*_luaVarsRef)[varName] = varValue.GetVector3();
			break;

		case DataUnion::VECTOR4:
			(*_luaVarsRef)[varName] = varValue.GetVector4();
			break;

		case DataUnion::REFERENCE:
		{
			Prefab* prefab = CAST_S( Prefab*, varValue.GetReference() );
			(*_luaVarsRef)[varName] = prefab;
			break;
		}
	}
}


bool Script::RunsInEditor() const
{
	return _runsInEditor;
}


bool Script::IsFirstUpdate() const
{
	return _isFirstUpdate;
}


int Script::GetPathsCount() const
{
	return _filePaths.Size();
}


const char* Script::GetFilePath( int index ) const
{
	ASSERT( index < _filePaths.Size() );
	return _filePaths[index].GetChar();
}


Iterator<String, DataArray<Script::LuaFunc>> Script::GetFuncListIterator() const
{
	return _luaFuncList.GetIterator();
}


Iterator<String, DataUnion> Script::GetVarsListIterator() const
{
	return _luaVarsList.GetIterator();
}


void Script::SetRunInEditor( bool value )
{
	_runsInEditor = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Script::SetFirstUpdate( bool value )
{
	_isFirstUpdate = value;
	// does not emit event
}


void Script::SetFilePath( int index, const char* filePath )
{
	// fill array
	while ( index >= _filePaths.Size() ) {
		_filePaths.Add( "" );
	}

	// set filepath
	_filePaths[index] = filePath;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Script::ClearFilePaths()
{
	// empty list
	_filePaths.Clear();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Script::LoadLuaFiles( bool clearVars )
{
	// reset flag
	_isFirstUpdate = TRUE;

	// clear function list
	_luaFuncList.Clear();

	// clear var list
	if ( clearVars ) {
		_luaVarsList.Clear();

		// recreate dictionary
		lua_State* L = ScriptManager::Singleton()->GetLuaState();
		_luaVarsRef = Memory::Alloc<luabridge::LuaRef>( L );
		(*_luaVarsRef) = luabridge::newTable( L );
	}

	// load all lua files
	for ( int i = 0; i < _filePaths.Size(); i++ ) {
		// create path
		FiniteString filePath( "%s%s", _dirPath.GetChar(), _filePaths[i].GetChar() );
		// read file
		char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
		// load lua
		_LoadLuaData( fileData );
		// free memory for file
		Memory::FreeArray( fileData );
	}
}

void Script::_LoadLuaData( const char* luaData )
{
	// nothing to load
	if ( luaData == NULL ) {
		return;
	}

	// get lua state
	lua_State* L = ScriptManager::Singleton()->GetLuaState();

	// load script data
	switch ( luaL_loadstring( L, luaData ) )	{
		case LUA_ERRSYNTAX:
		{
			// syntax error
			DebugManager::Singleton()->Error( lua_tostring( L, -1 ) );
			break;
		}

		case LUA_OK:
		{
			// script loaded
			// run lua
			if ( lua_pcall( L, 0, 0, 0 ) ) {
				// run error
				DebugManager::Singleton()->Error( lua_tostring( L, -1 ) );
				break;
			}
			else {
				// run lua: success
				// load default functions
				_AddLuaFunc( FUNC_ONSTART );
				_AddLuaFunc( FUNC_ONUPDATE );

				// load variables
				_LoadLuaVars();
			}
		}
	}
}


void Script::_AddLuaFunc( const char* funcName )
{
	// get lua state
	lua_State* L = ScriptManager::Singleton()->GetLuaState();

	// get ref
	luabridge::LuaRef luaFunc = luabridge::getGlobal( L, funcName );
	// remove ref
	luabridge::setGlobal( L, luabridge::Nil(), funcName );

	// check if exists
	if ( !luaFunc.isNil() && luaFunc.isFunction() ) {
		// add func
		if ( _luaFuncList.Has(funcName) ) {
			_luaFuncList.Get( funcName ).Add( LuaFunc {
				Memory::Alloc<luabridge::LuaRef>( luaFunc ),
				FALSE
			} );
		}
		else {
			DataArray<LuaFunc> funcList;
			funcList.Add( LuaFunc {
				Memory::Alloc<luabridge::LuaRef>( luaFunc ),
				FALSE
			} );
			_luaFuncList.Set( funcName, funcList );
		}
	}
}


void Script::_LoadLuaVars()
{
	// get lua state
	lua_State* L = ScriptManager::Singleton()->GetLuaState();

	// get ref
	luabridge::LuaRef luaVars = luabridge::getGlobal( L, GLOBAL_VARS );
	// remove ref
	luabridge::setGlobal( L, luabridge::Nil(), GLOBAL_VARS );

	// check if exists
	if ( !luaVars.isNil() && luaVars.isTable() ) {
		// iterate lua table
		luabridge::push( L, luaVars ); // push table
		lua_pushnil(L);  // push nil, so lua_next removes it from stack and puts (k, v) on stack
		while ( lua_next(L, -2) != 0 ) { // -2, because we have table at -1
			if ( lua_isstring(L, -2) ) { // only store stuff with string keys
				String varName( lua_tostring( L, -2 ) );
				DataUnion varValue;
				String type = lua_tostring( L, -1 );

				if ( type == TYPE_INT ) {
					varValue.SetInt( 0 );
				}
				else if ( type == TYPE_FLOAT ) {
					varValue.SetFloat( 0.0f );
				}
				else if ( type == TYPE_BOOL ) {
					varValue.SetBool( FALSE );
				}
				else if ( type == TYPE_STRING ) {
					varValue.SetString( "" );
				}
				else if ( type == TYPE_VECTOR2 ) {
					varValue.SetVector2( Vector2() );
				}
				else if ( type == TYPE_VECTOR3 ) {
					varValue.SetVector3( Vector3() );
				}
				else if ( type == TYPE_VECTOR4 ) {
					varValue.SetVector4( Vector4() );
				}
				else if ( type == TYPE_PREFAB ) {
					varValue.SetReference( NULL );
				}
				else {
					// unknown type error
					FiniteString errorMsg( ERROR_UNKNOWN_TYPE, type.GetChar() );
					DebugManager::Singleton()->Error( errorMsg.GetChar() );
					return;
				}

				// add to vars list
				if ( _luaVarsList.Has( varName ) ) {
					// check if same type
					if ( _luaVarsList.Get( varName ).GetValueType() == varValue.GetValueType() ) {
						// ignore it
					}
					else {
						// different type, overwrite var
						SetVariable( varName.GetChar(), varValue );
					}
				}
				else {
					// new variable
					SetVariable( varName.GetChar(), varValue );
				}
			}
			lua_pop(L, 1); // remove value, keep key for lua_next
		}
		lua_pop(L, 1); // pop table
	}
}
