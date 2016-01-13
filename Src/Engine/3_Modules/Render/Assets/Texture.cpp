// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Texture.h"
#include "../RenderManagerImpl.h"
#include "../../File/FileManager.h"
#include "../../Event/EventManager.h"


using namespace CYRED;



Texture::Texture()
	: Asset( AssetType::TEXTURE )
	, _textureID( INVALID_TEXTURE )
	, _hasMipmap( TRUE )
	, _clearBufferOnBind( TRUE )
	, _width( 0 )
	, _height( 0 )
	, _channels( 0 )
	, _type( TextureType::TEXTURE_2D )
	, _loadFromFile( FALSE )
{
	_imageBuffers[0] = NULL;
	_imageBuffers[1] = NULL;
	_imageBuffers[2] = NULL;
	_imageBuffers[3] = NULL;
	_imageBuffers[4] = NULL;
	_imageBuffers[5] = NULL;
}


Texture::Texture( UInt textureID )
	: Asset( AssetType::TEXTURE )
	, _textureID( textureID )
	, _hasMipmap( TRUE )
	, _clearBufferOnBind( TRUE )
	, _width( 0 )
	, _height( 0 )
	, _channels( 0 )
	, _type( TextureType::TEXTURE_2D )
	, _loadFromFile( FALSE )
{
	_imageBuffers[0] = NULL;
	_imageBuffers[1] = NULL;
	_imageBuffers[2] = NULL;
	_imageBuffers[3] = NULL;
	_imageBuffers[4] = NULL;
	_imageBuffers[5] = NULL;
}


Texture::~Texture()
{
	NotAPI::RenderManagerImpl::Singleton()->DeleteTexture( _textureID );

	Memory::FreeArray( _imageBuffers[0] );
	Memory::FreeArray( _imageBuffers[1] );
	Memory::FreeArray( _imageBuffers[2] );
	Memory::FreeArray( _imageBuffers[3] );
	Memory::FreeArray( _imageBuffers[4] );
	Memory::FreeArray( _imageBuffers[5] );

	_imageBuffers[0] = NULL;
	_imageBuffers[1] = NULL;
	_imageBuffers[2] = NULL;
	_imageBuffers[3] = NULL;
	_imageBuffers[4] = NULL;
	_imageBuffers[5] = NULL;
}


void Texture::LoadUniqueID()
{
	Char filePath[ MAX_SIZE_CUSTOM_STRING ];
	CUSTOM_STRING( filePath, "%s%s%s", _dirPath.GetChar(), 
										_name.GetChar(), 
										FileManager::FILE_FORMAT_TEXTURE );

	Char* fileData = FileManager::Singleton()->ReadFile( filePath );
	FileManager::Singleton()->Deserialize<Texture>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	Memory::FreeArray( fileData );
}


void Texture::LoadFullFile()
{
	Bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	Char filePath[ MAX_SIZE_CUSTOM_STRING ];
	CUSTOM_STRING( filePath, "%s%s%s", _dirPath.GetChar(), 
										_name.GetChar(), 
										FileManager::FILE_FORMAT_TEXTURE );

	Char* fileData = FileManager::Singleton()->ReadFile( filePath );
	FileManager::Singleton()->Deserialize<Texture>( fileData, this );

	// free memory for file
	Memory::FreeArray( fileData );

	BindToGPU();

	_emitEvents = oldEmitEvents;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}


void Texture::BindToGPU()
{
	NotAPI::RenderManagerImpl* renderManager = NotAPI::RenderManagerImpl::Singleton();

	if ( _loadFromFile )
	{
		if ( _type == TextureType::TEXTURE_2D )
		{
			Char imagePath[MAX_SIZE_CUSTOM_STRING];
			CUSTOM_STRING( imagePath, "%s%s", _dirPath.GetChar(), _imagePaths[0].GetChar() );

			Int width, height, channels;
			_imageBuffers[0] = FileManager::Singleton()->ReadImage( imagePath, 
																	&width, 
																	&height, 
																	&channels );
			_width = width;
			_height = height;
			_channels = channels;
		}
		else if ( _type == TextureType::CUBE_MAP )
		{
			for ( UInt i = 0; i < 6; ++i )
			{
				Char imagePath[MAX_SIZE_CUSTOM_STRING];
				CUSTOM_STRING( imagePath, "%s%s", _dirPath.GetChar(), _imagePaths[i].GetChar() );

				Int width, height, channels;
				_imageBuffers[i] = FileManager::Singleton()->ReadImage( imagePath, 
																		&width, 
																		&height, 
																		&channels );
				_width = width;
				_height = height;
				_channels = channels;
			}
		}
	}

	if ( _type == TextureType::TEXTURE_2D )
	{
		renderManager->CreateTexture2D( _textureID, _width, _height, _channels, _hasMipmap, _imageBuffers[0] );

		if ( _clearBufferOnBind )
		{
			Memory::FreeArray( _imageBuffers[0] );
			_imageBuffers[0] = NULL;
		}
	}

	if ( _type == TextureType::CUBE_MAP )
	{
		renderManager->CreateCubeMapTexture( _textureID, _width, _height, _channels, _hasMipmap, 
											 _imageBuffers[0], _imageBuffers[1],
											 _imageBuffers[2], _imageBuffers[3],
											 _imageBuffers[4], _imageBuffers[5] );

		if ( _clearBufferOnBind )
		{
			Memory::FreeArray( _imageBuffers[0] );
			Memory::FreeArray( _imageBuffers[1] );
			Memory::FreeArray( _imageBuffers[2] );
			Memory::FreeArray( _imageBuffers[3] );
			Memory::FreeArray( _imageBuffers[4] );
			Memory::FreeArray( _imageBuffers[5] );

			_imageBuffers[0] = NULL;
			_imageBuffers[1] = NULL;
			_imageBuffers[2] = NULL;
			_imageBuffers[3] = NULL;
			_imageBuffers[4] = NULL;
			_imageBuffers[5] = NULL;
		}
	}
}


UInt Texture::GetTextureID() const
{
	return _textureID;
}


TextureType Texture::GetType() const
{
	return _type;
}


Bool Texture::HasMipmap() const
{
	return _hasMipmap;
}


Bool Texture::DoesClearBufferOnBind() const
{
	return _clearBufferOnBind;
}


UInt Texture::GetWidth() const
{
	return _width;
}


UInt Texture::GetHeight() const
{
	return _height;
}


UInt Texture::GetChannels() const
{
	return _channels;
}


UChar* Texture::GetImageBuffer( UInt index ) const
{
	ASSERT( index < 6 );

	return _imageBuffers[index];
}


const Char* Texture::GetImagePath( UInt index ) const
{
	ASSERT( index < 6 );

	return _imagePaths[index].GetChar();
}


void Texture::SetTextureType( TextureType type )
{
	_type = type;
}


void Texture::SetHasMipmap( Bool value )
{
	_hasMipmap = value;
}


void Texture::SetClearBufferOnBind( Bool value )
{
	_clearBufferOnBind = value;
}


void Texture::SetImageBuffer( UInt index, UChar* buffer )
{
	ASSERT( index < 6 );

	_imageBuffers[index] = buffer;
}


void Texture::SetImageInfo( UInt width, UInt height, UInt channels )
{
	_width = width;
	_height = height;
	_channels = channels;
}


void Texture::SetImagePath( UInt index, const Char* path )
{
	ASSERT( index < 6 );

	_imagePaths[index] = path;
}


void Texture::SetLoadFromFile( Bool value )
{
	_loadFromFile = value;
}
