// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Texture.h"
#include "../RenderManagerImpl.h"
#include "../../File/FileManager.h"
#include "../../Event/EventManager.h"
#include "../../Script/ScriptManager.h"
#include "../../Debug/DebugManager.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"


extern "C" 
{
	#include "Lua_523\Include\lua.h"
	#include "Lua_523\Include\lauxlib.h"
	#include "Lua_523\Include\lualib.h"
};

#include "LuaBridge\Include\LuaBridge.h"


using namespace CYRED;


Texture::Texture()
	: Asset( AssetType::TEXTURE )
	, _textureID( INVALID_TEXTURE )
	, _hasMipmap( TRUE )
	, _clearBufferOnBind( TRUE )
	, _textureType( TextureType::TEXTURE_2D )
{
	for ( UInt i = 0; i < 6; i++ ) {
		_imageBuffer[i]		= NULL;
		_imageWidth[i]		= 0;
		_imageHeight[i]		= 0;
		_imageChannels[i]	= 0;
	}
}


Texture::Texture( UInt textureID )
	: Asset( AssetType::TEXTURE )
	, _textureID( textureID )
	, _hasMipmap( TRUE )
	, _clearBufferOnBind( TRUE )
	, _textureType( TextureType::TEXTURE_2D )
{
	for ( UInt i = 0; i < 6; i++ ) {
		_imageBuffer[i]		= NULL;
		_imageWidth[i]		= 0;
		_imageHeight[i]		= 0;
		_imageChannels[i]	= 0;
	}
}


Texture::~Texture()
{
	NotAPI::RenderManagerImpl::Singleton()->DeleteTexture( _textureID );

	Memory::FreeArray( _imageBuffer[0] );
	Memory::FreeArray( _imageBuffer[1] );
	Memory::FreeArray( _imageBuffer[2] );
	Memory::FreeArray( _imageBuffer[3] );
	Memory::FreeArray( _imageBuffer[4] );
	Memory::FreeArray( _imageBuffer[5] );

	for ( UInt i = 0; i < 6; i++ ) {
		_imageBuffer[i]		= NULL;
		_imageWidth[i]		= 0;
		_imageHeight[i]		= 0;
		_imageChannels[i]	= 0;
	}
}


void Texture::LoadUniqueID()
{
	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_TEXTURE );
	}

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Texture>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	Memory::FreeArray( fileData );
}


void Texture::LoadFullFile()
{
	Bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_TEXTURE );
	}

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Texture>( fileData, this );

	// free memory for file
	Memory::FreeArray( fileData );

	BindToGPU();

	_emitEvents = oldEmitEvents;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Texture::ClearAsset()
{
	NotAPI::RenderManagerImpl::Singleton()->DeleteTexture( _textureID );
	_textureID = INVALID_TEXTURE;

	_hasMipmap = TRUE;
	_clearBufferOnBind = TRUE;
	_textureType = TextureType::TEXTURE_2D;
	_isTemporary = TRUE;

	Memory::FreeArray( _imageBuffer[0] );
	Memory::FreeArray( _imageBuffer[1] );
	Memory::FreeArray( _imageBuffer[2] );
	Memory::FreeArray( _imageBuffer[3] );
	Memory::FreeArray( _imageBuffer[4] );
	Memory::FreeArray( _imageBuffer[5] );

	for ( UInt i = 0; i < 6; i++ ) {
		_imageBuffer[i]		= NULL;
		_imageWidth[i]		= 0;
		_imageHeight[i]		= 0;
		_imageChannels[i]	= 0;
	}

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


Asset* Texture::Clone()
{
	return _BuildClone( Memory::Alloc<Texture>() );
}


const Char* CYRED::Texture::GetExtension()
{
	if ( _useExtension ) {
		return FileManager::FILE_FORMAT_TEXTURE;
	}

	return NULL;
}


void Texture::BindToGPU()
{
	NotAPI::RenderManagerImpl* renderManager = NotAPI::RenderManagerImpl::Singleton();

	// load texture into CPU
	if ( _loadType == TextureLoadType::EXTERNAL ) {
		UInt total = 0;
		switch ( _textureType ) {
			case TextureType::TEXTURE_2D:	total = 1;	break;
			case TextureType::CUBE_MAP:		total = 6;	break;
		}

		for ( UInt i = 0; i < total; ++i ) {
			FiniteString imagePath( "%s%s", _dirPath.GetChar(), _filePaths[i].GetChar() );
			_imageBuffer[i] = FileManager::Singleton()->ReadImage( imagePath.GetChar(), 
																	&_imageWidth[i], 
																	&_imageHeight[i], 
																	&_imageChannels[i] );
		}
	}
	else if ( _loadType == TextureLoadType::SCRIPTED ) {
		UInt total = 0;
		switch ( _textureType ) {
			case TextureType::TEXTURE_2D:	total = 1;	break;
			case TextureType::CUBE_MAP:		total = 6;	break;
		}

		for ( UInt i = 0; i < total; ++i ) {
			if ( _filePaths[i].GetLength() > 0 ) {
				FiniteString filePath( "%s%s", _dirPath.GetChar(), _filePaths[i].GetChar() );

				Int fileSize;
				Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar(), fileSize );

				// execute script

				// get lua state
				lua_State* L = ScriptManager::Singleton()->GetLuaState();

				// bind global this
				luabridge::LuaRef thisRef( L, this );
				luabridge::setGlobal( L, thisRef, GLOBAL_THIS );

				// load script data
				switch ( luaL_loadstring( L, fileData ) ) {
					case LUA_ERRSYNTAX:
					{
						// syntax error
						DebugManager::Singleton()->Error( lua_tostring( L, -1 ) );
						break;
					}

					case LUA_OK:
					{
						// script loaded
						// run lua
						if ( lua_pcall( L, 0, 0, 0 ) ) {
							// run error
							DebugManager::Singleton()->Error( lua_tostring( L, -1 ) );
							break;
						}
						else {
							// run lua: success
						}
					}
				}

				// free string memory
				Memory::FreeArray( fileData );
			}
		}
	}

	// bind texture to GPU
	if ( _textureType == TextureType::TEXTURE_2D ) {
		renderManager->CreateTexture2D( _textureID, _imageWidth[0], _imageHeight[0], 
										_imageChannels[0], _hasMipmap, _imageBuffer[0] );

		if ( _clearBufferOnBind ) {
			Memory::FreeArray( _imageBuffer[0] );
			_imageBuffer[0]		= NULL;
		}
	}
	else if ( _textureType == TextureType::CUBE_MAP ) {
		renderManager->CreateCubeMapTexture( _textureID, _imageWidth[0], _imageHeight[0], 
											 _imageChannels[0], _hasMipmap, 
											 _imageBuffer[0], _imageBuffer[1],
											 _imageBuffer[2], _imageBuffer[3],
											 _imageBuffer[4], _imageBuffer[5] );

		if ( _clearBufferOnBind ) {
			Memory::FreeArray( _imageBuffer[0] );
			Memory::FreeArray( _imageBuffer[1] );
			Memory::FreeArray( _imageBuffer[2] );
			Memory::FreeArray( _imageBuffer[3] );
			Memory::FreeArray( _imageBuffer[4] );
			Memory::FreeArray( _imageBuffer[5] );

			for ( UInt i = 0; i < 6; i++ ) {
				_imageBuffer[i]		= NULL;
			}
		}
	}
}


UInt Texture::GetTextureID() const
{
	return _textureID;
}


TextureType Texture::GetTextureType() const
{
	return _textureType;
}


TextureLoadType Texture::GetLoadType() const
{
	return _loadType;
}


Bool Texture::HasMipmap() const
{
	return _hasMipmap;
}


Bool Texture::DoesClearBufferOnBind() const
{
	return _clearBufferOnBind;
}


UChar* Texture::GetImageBuffer( UInt index ) const
{
	ASSERT( index < 6 );

	return _imageBuffer[index];
}


const Char* Texture::GetImagePath( UInt index ) const
{
	ASSERT( index < 6 );

	return _filePaths[index].GetChar();
}


void Texture::SetTextureType( TextureType type )
{
	_textureType = type;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Texture::SetLoadType( TextureLoadType type )
{
	_loadType = type;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Texture::SetHasMipmap( Bool value )
{
	_hasMipmap = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Texture::SetClearBufferOnBind( Bool value )
{
	_clearBufferOnBind = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Texture::SetImageBuffer( UInt index, UChar* buffer )
{
	ASSERT( index < 6 );

	_imageBuffer[index] = buffer;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Texture::SetImagePath( UInt index, const Char* path )
{
	ASSERT( index < 6 );

	_filePaths[index] = path;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Texture::SetImageData( UInt bufferIndex, Int width, Int height, Int channels )
{
	ASSERT( bufferIndex < 6 );

	_imageWidth[bufferIndex]	= width;
	_imageHeight[bufferIndex]	= height;
	_imageChannels[bufferIndex] = channels;
	_imageBuffer[bufferIndex]	= Memory::AllocArray<UChar>( width * height * channels );
}


void Texture::SetPixel( UInt bufferIndex, UInt pixelIndex, UChar pixelValue )
{
	ASSERT( bufferIndex < 6 );

	_imageBuffer[bufferIndex][pixelIndex] = pixelValue;
}
