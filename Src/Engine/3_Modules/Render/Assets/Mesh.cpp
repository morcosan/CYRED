// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Mesh.h"
#include "../RenderManagerImpl.h"
#include "../../Event/EventManager.h"
#include "../../File/FileManager.h"
#include "../../Asset/AssetManager.h"
#include "../../Script/ScriptManager.h"
#include "../../Debug/DebugManager.h"
#include "../../File/Sections/MeshLoader.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"

extern "C" 
{
	#include "Lua_523\Include\lua.h"
	#include "Lua_523\Include\lauxlib.h"
	#include "Lua_523\Include\lualib.h"
};

#include "LuaBridge\Include\LuaBridge.h"


using namespace CYRED;


Mesh::Mesh()
	: Asset( AssetType::MESH )
	, _vbo( EMPTY_BUFFER )
	, _ibo( EMPTY_BUFFER )
	, _numIndices( 0 )
	, _clearBuffersOnBind( TRUE )
	, _meshType( MeshType::POLYGON )
	, _loadType( MeshLoadType::EXTERNAL )
{
}


Mesh::~Mesh()
{
	NonAPI::RenderManagerImpl::Singleton()->DeleteBuffers( _vbo, _ibo );
}


void Mesh::LoadUniqueID()
{
	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_MESH );
	}

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Mesh>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	ARRAY_FREE( fileData );
}


void Mesh::LoadFullFile()
{
	bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_MESH );
	}

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Mesh>( fileData, this );

	// free memory for file
	ARRAY_FREE( fileData );

	BindToGPU();

	_emitEvents = oldEmitEvents;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


void Mesh::ClearAsset()
{
	NonAPI::RenderManagerImpl::Singleton()->DeleteBuffers( _vbo, _ibo );
	_vbo = EMPTY_BUFFER;
	_ibo = EMPTY_BUFFER;
	_numIndices = 0;

	_isTemporary = TRUE;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


Asset* Mesh::Clone()
{
	return _BuildClone( new Mesh() );
}


cchar* Mesh::GetExtension()
{
	if ( _useExtension ) {
		return FileManager::FILE_FORMAT_MESH;
	}

	return NULL;
}


void Mesh::BindToGPU()
{
	if ( _loadType == MeshLoadType::EXTERNAL ) {
		if ( _externalPath.GetLength() > 0 ) {
			FiniteString filePath( "%s%s", _dirPath.GetChar(), _externalPath.GetChar() );

			int fileSize;
			char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar(), fileSize );

			// try custom format first, then try import
			bool isLoaded = FileManager::Singleton()->LoadMesh( fileData, _vertices, _indices );
			if ( !isLoaded ) {
				FileManager::Singleton()->ImportMesh( fileData, fileSize, _vertices, _indices );
			}

			// free string memory
			ARRAY_FREE( fileData );

			// store total indices
			_numIndices = _indices.Size();
		}
	}
	else if ( _loadType == MeshLoadType::SCRIPTED ) {
		if ( _externalPath.GetLength() > 0 ) {
			FiniteString filePath( "%s%s", _dirPath.GetChar(), _externalPath.GetChar() );

			int fileSize;
			char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar(), fileSize );

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

						// clear lists
						ClearVertices();
						ClearIndices();

						break;
					}
					else {
						// run lua: success
					}
				}
			}

			// free string memory
			ARRAY_FREE( fileData );

			// store total indices
			_numIndices = _indices.Size();
		}
	}

	// calculate tan and bitan for triangles
	if ( _numIndices % 3 == 0 ) {
		NonAPI::RenderManagerImpl::Singleton()->CalculateTanBitangents( _vertices, _indices );
	}

	// create buffers
	NonAPI::RenderManagerImpl::Singleton()->CreateMeshBuffers( _vbo, _ibo, _vertices, _indices );

	// clear data
	if ( _clearBuffersOnBind ) {
		_vertices.Clear();
		_indices.Clear();
	}
}


MeshType Mesh::GetMeshType() const
{
	return _meshType;
}

MeshLoadType Mesh::GetLoadType() const
{
	return _loadType;
}


int Mesh::GetVBO() const
{
	return _vbo;
}


int Mesh::GetIBO() const
{
	return _ibo;
}


int Mesh::GetNumIndices() const
{
	return _numIndices;
}


bool Mesh::DoesClearBuffersOnBind() const
{
	return _clearBuffersOnBind;
}


cchar* Mesh::GetExternalPath() const
{
	return _externalPath.GetChar();
}


void Mesh::SetMeshType( MeshType type )
{
	_meshType = type;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


void Mesh::SetLoadType( MeshLoadType type )
{
	_loadType = type;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


void Mesh::SetVertices( DataArray<Vertex>& vertices )
{
	_vertices.Clear();
	for ( int i = 0; i < vertices.Size(); ++i )
	{
		_vertices.Add( vertices[i] );
	}

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


void Mesh::SetIndices( DataArray<int>& indices )
{
	_indices.Clear();
	for ( int i = 0; i < indices.Size(); ++i )
	{
		_indices.Add( indices[i] );
	}

	_numIndices = indices.Size();

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


void Mesh::SetClearBuffersOnBind( bool value )
{
	_clearBuffersOnBind = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


void Mesh::SetExternalPath( cchar* filePath )
{
	_externalPath = filePath;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


void Mesh::ClearVertices()
{
	_vertices.Clear();
}


void Mesh::AddVertex( Vertex vertex )
{
	_vertices.Add( vertex );
}


void Mesh::ClearIndices()
{
	_indices.Clear();
}


void Mesh::AddIndex( int index )
{
	_indices.Add( index );
}

