// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Morph.h"
#include "../RenderManagerImpl.h"
#include "../../Event/EventManager.h"
#include "../../File/FileManager.h"
#include "../../File/Fragments/MeshLoader.h"
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
	NotAPI::RenderManagerImpl::Singleton()->DeleteBuffers( _vbo, _ibo );
}


void Morph::LoadUniqueID()
{
	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_MORPH );
	}

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Morph>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	Memory::FreeArray( fileData );
}


void Morph::LoadFullFile()
{
	Bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_MORPH );
	}

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Morph>( fileData, this );

	// free memory for file
	Memory::FreeArray( fileData );

	BindToGPU();

	_emitEvents = oldEmitEvents;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Morph::ClearAsset()
{
	NotAPI::RenderManagerImpl::Singleton()->DeleteBuffers( _vbo, _ibo );
	_vbo = EMPTY_BUFFER;
	_ibo = EMPTY_BUFFER;
	_numIndices = 0;

	_totalStates = 0;

	_isTemporary = TRUE;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


Asset* Morph::Clone()
{
	return _BuildClone( Memory::Alloc<Morph>() );
}


void Morph::BindToGPU()
{
	DataArray<MorphVertex>	vertices;
	DataArray<UInt>			indices;
	_activeStates = 0;

	for ( UInt i = 0; i < _totalStates; ++i )
	{
		FiniteString filePath( "%s%s", _dirPath.GetChar(), _filePaths[i].GetChar() );

		Int fileSize;
		Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar(), fileSize );
		if ( fileData == NULL )
		{
			break;
		}

		DataArray<Vertex>	meshVertices;
		indices.Clear();

		// try custom format first, then try import
		Bool isLoaded = FileManager::Singleton()->LoadMesh( fileData, meshVertices, indices );
		if ( !isLoaded ) {
			FileManager::Singleton()->ImportMesh( fileData, fileSize, meshVertices, indices );
		}

		Memory::FreeArray( fileData );

		if ( i == 0 )
		{
			_numIndices = indices.Size();

			for ( UInt j = 0; j < meshVertices.Size(); ++j )
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

		for ( UInt j = 0; j < meshVertices.Size(); ++j )
		{
			vertices[j].position[i] = meshVertices[j].position;
			vertices[j].normal[i] = meshVertices[j].normal;
		}

		++_activeStates;
	}

	NotAPI::RenderManagerImpl::Singleton()->CreateMorphBuffers( _vbo, _ibo, vertices, indices );
}


UInt Morph::GetVBO() const
{
	return _vbo;
}


UInt Morph::GetIBO() const
{
	return _ibo;
}


UInt Morph::GetNumIndices() const
{
	return _numIndices;
}


UInt Morph::GetTotalStates() const
{
	return _totalStates;
}


UInt Morph::GetActiveStates() const
{
	return _activeStates;
}


const Char* Morph::GetFilePath( UInt index ) const
{
	ASSERT( index < MORPH_LIMIT );

	return _filePaths[index].GetChar();
}


void Morph::SetTotalStates( UInt value )
{
	ASSERT( value >= 0 );
	ASSERT( value <= MORPH_LIMIT );

	_totalStates = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Morph::SetFilePath( UInt index, const Char* filePath )
{
	ASSERT( index < MORPH_LIMIT );

	_filePaths[index] = filePath;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}
