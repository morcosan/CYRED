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
	namespace Enum_TextureLoadType
	{
		enum Enum
		{
			SCRIPTED
			, EXTERNAL
		};
	}

	typedef Enum_TextureType::Enum		TextureType;
	typedef Enum_TextureLoadType::Enum	TextureLoadType;
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
		void		LoadUniqueID	() override;
		void		LoadFullFile	() override;
		void		ClearAsset		() override;
		Asset*		Clone			() override;
		const Char*	GetExtension	() override;


	public:
		//! must be called in order for texture to work
		void			BindToGPU				();
	
		UInt			GetTextureID			()				const;
		TextureType		GetTextureType			()				const;
		TextureLoadType	GetLoadType				()				const;
		Bool			HasMipmap				()				const;
		Bool			DoesClearBufferOnBind	()				const;
		UInt			GetWidth				()				const;
		UInt			GetHeight				()				const;
		UInt			GetChannels				()				const;
		UChar*			GetImageBuffer			( UInt index )	const;
		const Char*		GetImagePath			( UInt index )	const;

		void SetTextureType			( TextureType type );
		void SetLoadType			( TextureLoadType type );
		void SetHasMipmap			( Bool value );
		void SetClearBufferOnBind	( Bool value );
		void SetImageBuffer			( UInt index, UChar* buffer );
		void SetImageInfo			( UInt width, UInt height, UInt channels );
		void SetImagePath			( UInt index, const Char* path );


	protected:
		UInt 			_textureID;
		TextureType		_textureType;
		TextureLoadType	_loadType;
		Bool			_hasMipmap;
		Bool			_clearBufferOnBind;		//! when binding to GPU, clear image buffer
	
		UInt			_width;
		UInt			_height;
		UInt			_channels;
		UChar*			_imageBuffers[6];
	
		String			_imagePaths[6];
	};
}