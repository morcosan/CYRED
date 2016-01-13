// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Mesh.h"
#include "../RenderManagerImpl.h"
#include "../../Event/EventManager.h"


using namespace CYRED;



Mesh::Mesh()
	: Asset( AssetType::MESH )
	, _vbo( EMPTY_BUFFER )
	, _ibo( EMPTY_BUFFER )
	, _numIndices( 0 )
	, _clearBuffersOnBind( TRUE )
{
}


Mesh::~Mesh()
{
	NotAPI::RenderManagerImpl::Singleton()->DeleteBuffers( _vbo, _ibo );
}


void Mesh::LoadUniqueID()
{
}


void Mesh::LoadFullFile()
{
}


void Mesh::BindToGPU()
{
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


void Mesh::SetMeshType( MeshType type )
{
	_meshType = type;
}


void Mesh::SetVertices( DataArray<Vertex>& vertices )
{
	_vertices.Clear();
	for ( UInt i = 0; i < vertices.Size(); ++i )
	{
		_vertices.Add( vertices[i] );
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
}


void Mesh::SetClearBuffersOnBind( Bool value )
{
	_clearBuffersOnBind = value;
}

