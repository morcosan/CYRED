// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Asset.h"


namespace CYRED
{
	namespace Enum_TextureType
	{
		enum Enum
		{
			TEXTURE_2D
			, CUBE_MAP
		};
	}

	typedef Enum_TextureType::Enum	TextureType;
}


namespace CYRED
{
	class DLL Texture : public Asset
	{
	public:
		Texture();
		Texture( UInt textureID );
		virtual ~Texture();


	public:
		void LoadUniqueID	() override;
		void LoadFullFile	() override;


	public:
		//! must be called in order for texture to work
		void		BindToGPU				();

		UInt		GetTextureID			()				const;
		TextureType	GetType					()				const;
		Bool		HasMipmap				()				const;
		Bool		DoesClearBufferOnBind	()				const;
		UInt		GetWidth				()				const;
		UInt		GetHeight				()				const;
		UInt		GetChannels				()				const;
		UChar*		GetImageBuffer			( UInt index )	const;
		const Char* GetImagePath			( UInt index )	const;

		void SetTextureType			( TextureType type );
		void SetHasMipmap			( Bool value );
		void SetClearBufferOnBind	( Bool value );
		void SetImageBuffer			( UInt index, UChar* buffer );
		void SetImageInfo			( UInt width, UInt height, UInt channels );
		void SetImagePath			( UInt index, const Char* path );
		void SetLoadFromFile		( Bool value );


	protected:
		UInt 		_textureID;
		TextureType	_type;
		Bool		_hasMipmap;
		Bool		_clearBufferOnBind;		//! when binding to GPU, clear image buffer

		UInt		_width;
		UInt		_height;
		UInt		_channels;
		UChar*		_imageBuffers[6];

		String		_imagePaths[6];
		Bool		_loadFromFile;
	};
}