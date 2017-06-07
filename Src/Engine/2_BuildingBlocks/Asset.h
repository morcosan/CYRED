// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
			, FONT
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
		virtual void	LoadUniqueID	() PURE_VIRTUAL;
		virtual void	LoadFullFile	() PURE_VIRTUAL;
		virtual void	ClearAsset		() PURE_VIRTUAL;
		virtual Asset*	Clone			() PURE_VIRTUAL;
		virtual cchar*	GetExtension	() PURE_VIRTUAL;


	public:
		cchar*		GetName			()	const;
		cchar*		GetDirPath		()	const;
		AssetType	GetAssetType	()	const;
		cchar*		GetUniqueID		()	const;
		bool		IsTemporary		()	const;
		bool		DoesEmitEvents	()	const;

		void		SetName			( cchar* name, bool useExtension = TRUE );
		void		SetDirPath		( cchar* dirPath );
		void		SetEmitEvents	( bool value );
		void		SetUniqueID		( cchar* uniqueID );
		void		SetIsTemporary	( bool value );


	protected:
		virtual void _OnRename() {}


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