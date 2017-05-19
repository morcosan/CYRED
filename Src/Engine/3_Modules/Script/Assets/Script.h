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
		const char* GLOBAL_VARS			= "VARS";
		const char* GLOBAL_GAMEOBJECT	= "GAMEOBJECT";
		const char* FUNC_ONSTART		= "OnStart";
		const char* FUNC_ONUPDATE		= "OnUpdate";
		const char* TYPE_INT			= "INT";
		const char* TYPE_FLOAT			= "FLOAT";
		const char* TYPE_BOOL			= "BOOL";
		const char* TYPE_VECTOR2		= "VECTOR2";
		const char* TYPE_VECTOR3		= "VECTOR3";
		const char* TYPE_VECTOR4		= "VECTOR4";
		const char* TYPE_STRING			= "STRING";
		const char* TYPE_PREFAB			= "PREFAB";

		const char* ERROR_UNKNOWN_TYPE	= "Unknown variable type: %.";


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
		const char*	GetExtension	() override;


	public:
		void CallFunction	( const char* funcName, GameObject* gameObject );
		void SetVariable	( const char* varName, DataUnion varValue );

		bool		RunsInEditor	()				const;
		bool		IsFirstUpdate	()				const;
		int		GetPathsCount	()				const;
		const char*	GetFilePath		( int index )	const;

		Iterator<String, DataArray<LuaFunc>>	GetFuncListIterator() const;
		Iterator<String, DataUnion>				GetVarsListIterator() const;

		void SetRunInEditor	( bool value );
		void SetFirstUpdate	( bool value );
		void SetFilePath	( int index, const char* filePath );
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
		void _LoadLuaData	( const char* luaData );
		void _AddLuaFunc	( const char* funcName );
		void _LoadLuaVars	();
	};
}