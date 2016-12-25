// Copyright (c) 2015 Morco (www.morco.ro)
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
	class DLL Script : public Asset
	{
		const Char* VARS			= "_vars";
		const Char* FUNC_ONSTART	= "OnStart";
		const Char* FUNC_ONUPDATE	= "OnUpdate";
		const Char* TYPE_INT		= "Int";
		const Char* TYPE_FLOAT		= "Float";
		const Char* TYPE_BOOL		= "Bool";
		const Char* TYPE_VECTOR2	= "Vector2";
		const Char* TYPE_VECTOR3	= "Vector3";
		const Char* TYPE_VECTOR4	= "Vector4";
		const Char* TYPE_STRING		= "String";
		const Char* TYPE_REFERENCE	= "Reference";


	public:
		struct LuaVar
		{
			DataUnion::ValueType	type;
			DataUnion				value;
		};


	public:
		Script();
		virtual ~Script();


	public:
		void LoadUniqueID	() override;
		void LoadFullFile	() override;
		void ClearAsset		() override;


	public:
		void CallFunction( const Char* funcName );

		Bool		RunsInEditor	()				const;
		Bool		IsFirstUpdate	()				const;
		UInt		GetPathsCount	()				const;
		const Char*	GetFilePath		( UInt index )	const;

		Iterator<String, DataArray<luabridge::LuaRef*>> GetFuncListIterator() const;
		Iterator<String, LuaVar>						GetVarsListIterator() const;

		void SetRunInEditor	( Bool value );
		void SetFirstUpdate	( Bool value );
		void SetFilePath	( UInt index, const Char* filePath );
		void ClearFilePaths	();

		void LoadLuaFiles	();


	protected:
		Bool				_runsInEditor;
		Bool				_isFirstUpdate;
		DataArray<String>	_filePaths;

		// a list with all lua objects associated with this Scripter
		DataMap<String, DataArray<luabridge::LuaRef*>>	_luaFuncList;
		DataMap<String, LuaVar>							_luaVarsList;
		luabridge::LuaRef*								_luaVarsRef;


	protected:
		void _LoadLuaData	( const Char* luaData );
		void _AddLuaFunc	( const Char* funcName );
		void _LoadLuaVar	();
	};
}