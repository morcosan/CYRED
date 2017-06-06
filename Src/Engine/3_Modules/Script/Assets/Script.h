// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Asset.h"
#include "../../../2_BuildingBlocks/Data/DataMap.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"
#include "../../../2_BuildingBlocks/Data/DataUnion.h"

extern "C" 
{
	#include "Lua_523\Include\lua.h"
	#include "Lua_523\Include\lauxlib.h"
	#include "Lua_523\Include\lualib.h"
};

#include "LuaBridge\Include\LuaBridge.h"

namespace CYRED
{
	class GameObject;
}


namespace CYRED
{
	class DLL Script : public Asset
	{
	public:
		static cchar* const FUNC_ON_START;
		static cchar* const FUNC_ON_UPDATE;
		static cchar* const FUNC_ON_COLLISION_ENTER;
		static cchar* const FUNC_ON_COLLISION_EXIT;
												
		static cchar* const GLOBAL_VARS;
		static cchar* const GLOBAL_GAMEOBJECT;
		static cchar* const TYPE_INT;
		static cchar* const TYPE_FLOAT;
		static cchar* const TYPE_BOOL;
		static cchar* const TYPE_VECTOR2;
		static cchar* const TYPE_VECTOR3;
		static cchar* const TYPE_VECTOR4;
		static cchar* const TYPE_STRING;
		static cchar* const TYPE_PREFAB;
													
		static cchar* const ERROR_UNKNOWN_TYPE;


	public:
		Script();
		virtual ~Script() {}


	public:
		struct LuaFunc
		{
			luabridge::LuaRef*	funcRef;
			bool				isBroken;
		};


	public:
		void	LoadUniqueID	() override;
		void	LoadFullFile	() override;
		void	ClearAsset		() override;
		Asset*	Clone			() override;
		cchar*	GetExtension	() override;


	public:
		template <typename... Args>
		void	CallFunction	( cchar* funcName, GameObject* gameObject, Args... args );

		void	SetVariable		( cchar* varName, DataUnion varValue );

		bool	RunsInEditor	()				const;
		bool	IsFirstUpdate	()				const;
		int		GetPathsCount	()				const;
		cchar*	GetFilePath		( int index )	const;

		void	SetRunInEditor	( bool value );
		void	SetFirstUpdate	( bool value );
		void	SetFilePath		( int index, cchar* filePath );
		void	ClearFilePaths	();

		void	LoadLuaFiles	( bool clearVars = TRUE );

		Iterator<String, DataArray<LuaFunc>>	GetFuncListIterator() const;
		Iterator<String, DataUnion>				GetVarsListIterator() const;


	private:
		bool				_runsInEditor;
		bool				_isFirstUpdate;
		DataArray<String>	_filePaths;

		// a list with all lua objects associated with this Scripter
		DataMap<String, DataArray<LuaFunc>>	_luaFuncList;
		DataMap<String, DataUnion>			_luaVarsList;
		luabridge::LuaRef*					_luaVarsRef;


	private:
		void _LoadLuaData	( cchar* luaData );
		void _AddLuaFunc	( cchar* funcName );
		void _LoadLuaVars	();

		DataArray<LuaFunc>* _CallFunction		( cchar* funcName, GameObject* gameObject );
		void				_CallFunctionError	( cchar* errorMessage );
	};



	template <typename... Args>
	void Script::CallFunction( cchar* funcName, GameObject* gameObject, Args... args )
	{
		// get available functions to call
		DataArray<LuaFunc>* list = _CallFunction( funcName, gameObject );
		if ( list == NULL ) {
			return;
		}

		// call all functions
		DataArray<LuaFunc>& funcList = *list;
		for ( int i = 0; i < funcList.Size(); i++ ) {
			// if is broken do not call
			if ( !funcList[i].isBroken ) {
				// try to call the function
				try {
					(*funcList[i].funcRef)( args... );
				}
				// handle error
				catch ( luabridge::LuaException const& e ) {
					// display error once
					_CallFunctionError( e.what() );
					// mark as broken
					funcList[i].isBroken = TRUE;
				}
			}
		}
	}

}