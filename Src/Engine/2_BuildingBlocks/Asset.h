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
		virtual const char*	GetExtension	() PURE_VIRTUAL;


	public:
		const char*	GetName			()	const;
		const char*	GetDirPath		()	const;
		AssetType	GetAssetType	()	const;
		const char* GetUniqueID		()	const;
		bool		IsTemporary		()	const;
		bool		DoesEmitEvents	()	const;

		void		SetName			( const char* name, bool useExtension = TRUE );
		void		SetDirPath		( const char* dirPath );
		void		SetEmitEvents	( bool value );
		void		SetUniqueID		( const char* uniqueID );
		void		SetIsTemporary	( bool value );


	protected:
		AssetType	_assetType;
		String		_name;
		String		_dirPath;
		bool		_useExtension;
		bool		_emitEvents;
		String		_uniqueID;
		bool		_isTemporary;


	protected:
		Asset*		_BuildClone( Asset* clone )	const;
	};

}