// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../1_Required/Required.h"
#include "String/String.h"


namespace CYRED
{
	namespace Enum_AssetType
	{
		enum Enum
		{
			UNKNOWN
			, MATERIAL
			, MESH
			, TEXTURE
			, SHADER
			, SCENE
			, MORPH
			, SCRIPT
			, PREFAB
		};
	}

	typedef	Enum_AssetType::Enum	AssetType;
}


namespace CYRED
{
	ABSTRACT class DLL Asset
	{
	public:
		Asset( AssetType type );
		virtual ~Asset() {};


	public:
		virtual void		LoadUniqueID	() PURE_VIRTUAL;
		virtual void		LoadFullFile	() PURE_VIRTUAL;
		virtual void		ClearAsset		() PURE_VIRTUAL;
		virtual Asset*		Clone			() PURE_VIRTUAL;
		virtual const Char*	GetExtension	() PURE_VIRTUAL;


	public:
		const Char*	GetName			()	const;
		const Char*	GetDirPath		()	const;
		AssetType	GetAssetType	()	const;
		const Char* GetUniqueID		()	const;
		Bool		IsTemporary		()	const;
		Bool		DoesEmitEvents	()	const;

		void		SetName			( const Char* name, Bool useExtension = TRUE );
		void		SetDirPath		( const Char* dirPath );
		void		SetEmitEvents	( Bool value );
		void		SetUniqueID		( const Char* uniqueID );
		void		SetIsTemporary	( Bool value );


	protected:
		AssetType	_assetType;
		String		_name;
		String		_dirPath;
		Bool		_useExtension;
		Bool		_emitEvents;
		String		_uniqueID;
		Bool		_isTemporary;


	protected:
		Asset*		_BuildClone( Asset* clone )	const;
	};

}