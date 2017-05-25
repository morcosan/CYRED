// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Asset.h"
#include "../../../2_BuildingBlocks/Data/DataMap.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"
#include "../../../2_BuildingBlocks/Data/DataUnion.h"


namespace luabridge
{
	class LuaRef;
}

namespace CYRED
{
	class GameObject;
}


namespace CYRED
{
	class DLL Script : public Asset
	{
		cchar* GLOBAL_VARS			= "VARS";
		cchar* GLOBAL_GAMEOBJECT	= "GAMEOBJECT";
		cchar* FUNC_ONSTART		= "OnStart";
		cchar* FUNC_ONUPDATE		= "OnUpdate";
		cchar* TYPE_INT			= "INT";
		cchar* TYPE_FLOAT			= "FLOAT";
		cchar* TYPE_BOOL			= "BOOL";
		cchar* TYPE_VECTOR2		= "VECTOR2";
		cchar* TYPE_VECTOR3		= "VECTOR3";
		cchar* TYPE_VECTOR4		= "VECTOR4";
		cchar* TYPE_STRING			= "STRING";
		cchar* TYPE_PREFAB			= "PREFAB";

		cchar* ERROR_UNKNOWN_TYPE	= "Unknown variable type: %.";


	public:
		Script();
		virtual ~Script();


	public:
		struct LuaFunc
		{
			luabridge::LuaRef*	funcRef;
			bool				isBroken;
		};


	public:
		void		LoadUniqueID	() override;
		void		LoadFullFile	() override;
		void		ClearAsset		() override;
		Asset*		Clone			() override;
		cchar*	GetExtension	() override;


	public:
		void CallFunction	( cchar* funcName, GameObject* gameObject );
		void SetVariable	( cchar* varName, DataUnion varValue );

		bool		RunsInEditor	()				const;
		bool		IsFirstUpdate	()				const;
		int		GetPathsCount	()				const;
		cchar*	GetFilePath		( int index )	const;

		Iterator<String, DataArray<LuaFunc>>	GetFuncListIterator() const;
		Iterator<String, DataUnion>				GetVarsListIterator() const;

		void SetRunInEditor	( bool value );
		void SetFirstUpdate	( bool value );
		void SetFilePath	( int index, cchar* filePath );
		void ClearFilePaths	();

		void LoadLuaFiles	( bool clearVars = TRUE );


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
	};
}