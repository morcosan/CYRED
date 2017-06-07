// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		cchar* GLOBAL_THIS	= "TEXTURE";


	public:
		Texture();
		Texture( int textureID );
		virtual ~Texture();


	public:
		void	LoadUniqueID	() override;
		void	LoadFullFile	() override;
		void	ClearAsset		() override;
		Asset*	Clone			() override;
		cchar*	GetExtension	() override;


	public:
		void			BindToGPU				();
	
		int				GetTextureID			()				const;
		TextureType		GetTextureType			()				const;
		TextureLoadType	GetLoadType				()				const;
		bool			HasMipmap				()				const;
		bool			DoesClearBufferOnBind	()				const;
		uchar*			GetImageBuffer			( int index )	const;
		cchar*			GetImagePath			( int index )	const;

		void			SetTextureType			( TextureType type );
		void			SetLoadType				( TextureLoadType type );
		void			SetHasMipmap			( bool value );
		void			SetClearBufferOnBind	( bool value );
		void			SetImageBuffer			( int index, uchar* buffer );
		void			SetImagePath			( int index, cchar* path );

		//script API
		void			SetImageData			( int bufferIndex, int width, int height, int channels );
		void			SetPixel				( int bufferIndex, int pixelIndex, uchar pixelValue );


	private:
		uint 			_textureID;
		TextureType		_textureType;
		TextureLoadType	_loadType;
		bool			_hasMipmap;
		bool			_clearBufferOnBind;		//! when binding to GPU, clear image buffer
		String			_filePaths[6];

		int				_imageWidth[6];
		int				_imageHeight[6];
		int				_imageChannels[6];
		uchar*			_imageBuffer[6];
	};
}