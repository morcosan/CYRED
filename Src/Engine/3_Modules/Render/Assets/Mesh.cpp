// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Mesh.h"
#include "../RenderManagerImpl.h"
#include "../../Event/EventManager.h"
#include "../../File/FileManager.h"
#include "../../File/Fragments/MeshLoader.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"


using namespace CYRED;



Mesh::Mesh()
	: Asset( AssetType::MESH )
	, _vbo( EMPTY_BUFFER )
	, _ibo( EMPTY_BUFFER )
	, _numIndices( 0 )
	, _clearBuffersOnBind( TRUE )
	, _meshType( MeshType::POLYGON )
	, _loadType( LoadType::EXTERNAL )
{
}


Mesh::~Mesh()
{
	NotAPI::RenderManagerImpl::Singleton()->DeleteBuffers( _vbo, _ibo );
}


void Mesh::LoadUniqueID()
{
	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_MESH );
	}

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Mesh>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	Memory::FreeArray( fileData );
}


void Mesh::LoadFullFile()
{
	Bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_MESH );
	}

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Mesh>( fileData, this );

	// free memory for file
	Memory::FreeArray( fileData );

	BindToGPU();

	_emitEvents = oldEmitEvents;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}


void Mesh::ClearAsset()
{
	NotAPI::RenderManagerImpl::Singleton()->DeleteBuffers( _vbo, _ibo );
	_vbo = EMPTY_BUFFER;
	_ibo = EMPTY_BUFFER;
	_numIndices = 0;

	_isTemporary = TRUE;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}


Asset* Mesh::Clone()
{
	return _BuildClone( Memory::Alloc<Mesh>() );
}


void Mesh::BindToGPU()
{
	if ( _loadType == LoadType::EXTERNAL )
	{
		FiniteString filePath( "%s%s", _dirPath.GetChar(), _externalPath.GetChar() );

		Int fileSize;
		Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar(), fileSize );
		
		// try custom format first, then try import
		Bool isLoaded = FileManager::Singleton()->LoadMesh( fileData, _vertices, _indices );
		if ( !isLoaded ) {
			FileManager::Singleton()->ImportMesh( fileData, fileSize, _vertices, _indices );
		}

		Memory::FreeArray( fileData );

		_numIndices = _indices.Size();
	}

	NotAPI::RenderManagerImpl::Singleton()->CreateMeshBuffers( _vbo, _ibo, _vertices, _indices );

	if ( _clearBuffersOnBind )
	{
		_vertices.Clear();
		_indices.Clear();
	}
}


MeshType Mesh::GetMeshType() const
{
	return _meshType;
}

LoadType Mesh::GetLoadType() const
{
	return _loadType;
}


UInt Mesh::GetVBO() const
{
	return _vbo;
}


UInt Mesh::GetIBO() const
{
	return _ibo;
}


UInt Mesh::GetNumIndices() const
{
	return _numIndices;
}


Bool Mesh::DoesClearBuffersOnBind() const
{
	return _clearBuffersOnBind;
}


const Char* Mesh::GetExternalPath() const
{
	return _externalPath.GetChar();
}


void Mesh::SetMeshType( MeshType type )
{
	_meshType = type;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}


void Mesh::SetLoadType( LoadType type )
{
	_loadType = type;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}


void Mesh::SetVertices( DataArray<Vertex>& vertices )
{
	_vertices.Clear();
	for ( UInt i = 0; i < vertices.Size(); ++i )
	{
		_vertices.Add( vertices[i] );
	}

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}


void Mesh::SetIndices( DataArray<UInt>& indices )
{
	_indices.Clear();
	for ( UInt i = 0; i < indices.Size(); ++i )
	{
		_indices.Add( indices[i] );
	}

	_numIndices = indices.Size();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}


void Mesh::SetClearBuffersOnBind( Bool value )
{
	_clearBuffersOnBind = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}


void Mesh::SetExternalPath( const Char* filePath )
{
	_externalPath = filePath;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
}

