// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Font.h"

#include "../RenderManagerImpl.h"
#include "../../File/FileManager.h"
#include "../../Event/EventManager.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"


using namespace CYRED;


Font::Font()
	: Asset( AssetType::FONT )
	, _freetypeFace( NULL )
{
}


Font::~Font()
{
}


void Font::LoadUniqueID()
{
	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_FONT );
	}

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Font>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	ARRAY_FREE( fileData );
}


void Font::LoadFullFile()
{
	bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_FONT );
	}

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Font>( fileData, this );

	// free memory for file
	ARRAY_FREE( fileData );

	BindToGPU();

	_emitEvents = oldEmitEvents;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, this );
	}
}


void Font::ClearAsset()
{
	_isTemporary = TRUE;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, this );
	}
}


Asset* Font::Clone()
{
	return _BuildClone( new Font() );
}


cchar* Font::GetExtension()
{
	if ( _useExtension ) {
		return FileManager::FILE_FORMAT_FONT;
	}

	return NULL;
}


void Font::BindToGPU()
{
	NonAPI::RenderManagerImpl* renderMngr = NonAPI::RenderManagerImpl::Singleton();

	// create freetype face
	if ( _externalPath.GetLength() > 0 ) {
		renderMngr->CreateFreeTypeFace( _externalPath.GetChar(), _freetypeFace );
	}
}


cchar* Font::GetExternalPath() const
{
	return _externalPath.GetChar();
}


void Font::SetExternalPath( cchar* filePath )
{
	_externalPath = filePath;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, this );
	}
}
