// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Asset.h"
#include "../../../2_BuildingBlocks/Data/DataMap.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"


namespace luabridge
{
	class LuaRef;
}


namespace CYRED
{
	class DLL Script : public Asset
	{
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
		UInt		GetPathsCount	()				const;
		const Char*	GetFilePath		( UInt index )	const;

		void SetRunInEditor	( Bool value );
		void SetLuaData		( const Char* varName, luabridge::LuaRef* varData );
		void SetFilePath	( UInt index, const Char* filePath );
		void ClearFilePaths	();


	protected:
		Bool				_runsInEditor;
		DataArray<String>	_filePaths;

		// a list with all lua objects associated with this Scripter
		DataMap<String, luabridge::LuaRef*> _luaData;
	};
}