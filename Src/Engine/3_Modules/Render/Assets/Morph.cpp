// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Morph.h"
#include "../RenderManagerImpl.h"
#include "../../Event/EventManager.h"
#include "../../File/FileManager.h"
#include "../../File/Sections/MeshLoader.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"
#include "../OpenGL/Vertex.h"


using namespace CYRED;



Morph::Morph()
	: Asset( AssetType::MORPH )
	, _vbo( EMPTY_BUFFER )
	, _ibo( EMPTY_BUFFER )
	, _numIndices( 0 )
	, _totalStates( 0 )
	, _activeStates( 0 )
{
}


Morph::~Morph()
{
	NonAPI::RenderManagerImpl::Singleton()->DeleteBuffers( _vbo, _ibo );
}


void Morph::LoadUniqueID()
{
	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_MORPH );
	}

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Morph>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	ARRAY_FREE( fileData );
}


void Morph::LoadFullFile()
{
	bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_MORPH );
	}

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Morph>( fileData, this );

	// free memory for file
	ARRAY_FREE( fileData );

	BindToGPU();

	_emitEvents = oldEmitEvents;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


void Morph::ClearAsset()
{
	NonAPI::RenderManagerImpl::Singleton()->DeleteBuffers( _vbo, _ibo );
	_vbo = EMPTY_BUFFER;
	_ibo = EMPTY_BUFFER;
	_numIndices = 0;

	_totalStates = 0;

	_isTemporary = TRUE;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


Asset* Morph::Clone()
{
	return _BuildClone( new Morph() );
}


cchar* CYRED::Morph::GetExtension()
{
	if ( _useExtension ) {
		return FileManager::FILE_FORMAT_MORPH;
	}

	return NULL;
}


void Morph::BindToGPU()
{
	DataArray<MorphVertex>	vertices;
	DataArray<int>			indices;
	_activeStates = 0;

	for ( int i = 0; i < _totalStates; ++i )
	{
		FiniteString filePath( "%s%s", _dirPath.GetChar(), _filePaths[i].GetChar() );

		int fileSize;
		char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar(), fileSize );
		if ( fileData == NULL )
		{
			break;
		}

		DataArray<Vertex>	meshVertices;
		indices.Clear();

		// try custom format first, then try import
		bool isLoaded = FileManager::Singleton()->LoadMesh( fileData, meshVertices, indices );
		if ( !isLoaded ) {
			FileManager::Singleton()->ImportMesh( fileData, fileSize, meshVertices, indices );
		}

		ARRAY_FREE( fileData );

		if ( i == 0 )
		{
			_numIndices = indices.Size();

			for ( int j = 0; j < meshVertices.Size(); ++j )
			{
				vertices.Add( MorphVertex() );
				vertices[j].color = meshVertices[j].color;
				vertices[j].uv = meshVertices[j].uv;
			}
		}

		if ( _numIndices != indices.Size() || vertices.Size() != meshVertices.Size() )
		{
			break;
		}

		for ( int j = 0; j < meshVertices.Size(); ++j )
		{
			vertices[j].position[i] = meshVertices[j].position;
			vertices[j].normal[i] = meshVertices[j].normal;
		}

		++_activeStates;
	}

	NonAPI::RenderManagerImpl::Singleton()->CreateMorphBuffers( _vbo, _ibo, vertices, indices );
}


int Morph::GetVBO() const
{
	return _vbo;
}


int Morph::GetIBO() const
{
	return _ibo;
}


int Morph::GetNumIndices() const
{
	return _numIndices;
}


int Morph::GetTotalStates() const
{
	return _totalStates;
}


int Morph::GetActiveStates() const
{
	return _activeStates;
}


cchar* Morph::GetFilePath( int index ) const
{
	ASSERT( index < MORPH_LIMIT );

	return _filePaths[index].GetChar();
}


void Morph::SetTotalStates( int value )
{
	ASSERT( value >= 0 );
	ASSERT( value <= MORPH_LIMIT );

	_totalStates = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


void Morph::SetFilePath( int index, cchar* filePath )
{
	ASSERT( index < MORPH_LIMIT );

	_filePaths[index] = filePath;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}
