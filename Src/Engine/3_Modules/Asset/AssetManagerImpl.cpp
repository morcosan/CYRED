// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "AssetManagerImpl.h"
#include "../../2_BuildingBlocks/Random/Random.h"
#include "../Render/Assets/Material.h"
#include "../Render/Assets/Texture.h"
#include "../Render/Assets/Mesh.h"
#include "../Render/Assets/Shader.h"
#include "../Scene/Fragments/Scene.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition
DEFINE_LOCAL_SINGLETON( AssetManager, AssetManagerImpl )

DEFINE_LOCAL_SINGLETON_IMPL( AssetManagerImpl )


void AssetManagerImpl::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = true;
}


void AssetManagerImpl::Finalize()
{
	if ( !_isInitialized )
	{
		return;
	}
}


void AssetManagerImpl::ClearAll()
{
	for ( UInt i = 0; i < _meshes.Size(); ++i )
	{
		Bool emitEvents = _meshes[i]->DoesEmitEvents();
		_meshes[i]->SetEmitEvents( FALSE );
		_meshes[i]->ClearAsset();
		_meshes[i]->SetEmitEvents( emitEvents );
	}

	for ( UInt i = 0; i < _materials.Size(); ++i )
	{
		Bool emitEvents = _materials[i]->DoesEmitEvents();
		_materials[i]->SetEmitEvents( FALSE );
		_materials[i]->ClearAsset();
		_materials[i]->SetEmitEvents( emitEvents );
	}

	for ( UInt i = 0; i < _textures.Size(); ++i )
	{
		Bool emitEvents = _textures[i]->DoesEmitEvents();
		_textures[i]->SetEmitEvents( FALSE );
		_textures[i]->ClearAsset();
		_textures[i]->SetEmitEvents( emitEvents );
	}

	for ( UInt i = 0; i < _shaders.Size(); ++i )
	{
		Bool emitEvents = _shaders[i]->DoesEmitEvents();
		_shaders[i]->SetEmitEvents( FALSE );
		_shaders[i]->ClearAsset();
		_shaders[i]->SetEmitEvents( emitEvents );
	}

	// do not clear scenes
	for ( UInt i = 0; i < _scenes.Size(); ++i )
	{
		_scenes[i]->SetIsTemporary( TRUE );
	}
}


StatusAssetAdd AssetManagerImpl::AddMesh( Mesh* mesh )
{
	ASSERT( _isInitialized );

	Bool isOk = Random::ValidateUniqueID( mesh->GetUniqueID() );
	if ( !isOk )
	{
		return StatusAssetAdd::FAIL_INVALID_ID;
	}

	Mesh* found = GetMesh( mesh->GetUniqueID() );
	if ( found != NULL )
	{
		return StatusAssetAdd::FAIL_EXISTING;
	}

	_meshes.Add( mesh );

	return StatusAssetAdd::SUCCESS;
}


StatusAssetAdd AssetManagerImpl::AddMaterial( Material* material )
{
	ASSERT( _isInitialized );

	Bool isOk = Random::ValidateUniqueID( material->GetUniqueID() );
	if ( !isOk )
	{
		return StatusAssetAdd::FAIL_INVALID_ID;
	}

	Material* found = GetMaterial( material->GetUniqueID() );
	if ( found != NULL )
	{
		return StatusAssetAdd::FAIL_EXISTING;
	}

	_materials.Add( material );

	return StatusAssetAdd::SUCCESS;
}


StatusAssetAdd AssetManagerImpl::AddShader( Shader* shader )
{
	ASSERT( _isInitialized );

	Bool isOk = Random::ValidateUniqueID( shader->GetUniqueID() );
	if ( !isOk )
	{
		return StatusAssetAdd::FAIL_INVALID_ID;
	}

	Shader* found = GetShader( shader->GetUniqueID() );
	if ( found != NULL )
	{
		return StatusAssetAdd::FAIL_EXISTING;
	}

	_shaders.Add( shader );

	return StatusAssetAdd::SUCCESS;
}


StatusAssetAdd AssetManagerImpl::AddTexture( Texture* texture )
{
	ASSERT( _isInitialized );

	Bool isOk = Random::ValidateUniqueID( texture->GetUniqueID() );
	if ( !isOk )
	{
		return StatusAssetAdd::FAIL_INVALID_ID;
	}

	Texture* found = GetTexture( texture->GetUniqueID() );
	if ( found != NULL )
	{
		return StatusAssetAdd::FAIL_EXISTING;
	}

	_textures.Add( texture );

	return StatusAssetAdd::SUCCESS;
}


StatusAssetAdd AssetManagerImpl::AddScene( Scene* scene )
{
	ASSERT( _isInitialized );

	Bool isOk = Random::ValidateUniqueID( scene->GetUniqueID() );
	if ( !isOk )
	{
		return StatusAssetAdd::FAIL_INVALID_ID;
	}

	Scene* found = GetScene( scene->GetUniqueID() );
	if ( found != NULL )
	{
		return StatusAssetAdd::FAIL_EXISTING;
	}

	_scenes.Add( scene );

	return StatusAssetAdd::SUCCESS;
}


Mesh* AssetManagerImpl::GetMesh( const Char* uniqueID )
{
	ASSERT( _isInitialized );

	String temp( uniqueID );

	for ( UInt i = 0; i < _meshes.Size(); ++i )
	{
		if ( temp == _meshes[i]->GetUniqueID() )
		{
			return _meshes[i];
		}
	}
	
	return NULL;
}


Material* AssetManagerImpl::GetMaterial( const Char* uniqueID )
{
	ASSERT( _isInitialized );

	String temp( uniqueID );

	for ( UInt i = 0; i < _materials.Size(); ++i )
	{
		if ( temp == _materials[i]->GetUniqueID() )
		{
			return _materials[i];
		}
	}

	return NULL;
}


Shader* AssetManagerImpl::GetShader( const Char* uniqueID )
{
	ASSERT( _isInitialized );

	String temp( uniqueID );

	for ( UInt i = 0; i < _shaders.Size(); ++i )
	{
		if ( temp == _shaders[i]->GetUniqueID() )
		{
			return _shaders[i];
		}
	}

	return NULL;
}


Texture* AssetManagerImpl::GetTexture( const Char* uniqueID )
{
	ASSERT( _isInitialized );

	String temp( uniqueID );

	for ( UInt i = 0; i < _textures.Size(); ++i )
	{
		if ( temp == _textures[i]->GetUniqueID() )
		{
			return _textures[i];
		}
	}

	return NULL;
}


Scene* AssetManagerImpl::GetScene( const Char* uniqueID )
{
	ASSERT( _isInitialized );

	String temp( uniqueID );

	for ( UInt i = 0; i < _scenes.Size(); ++i )
	{
		if ( temp == _scenes[i]->GetUniqueID() )
		{
			return _scenes[i];
		}
	}

	return NULL;
}


UInt AssetManagerImpl::GetMaterialCount()
{
	ASSERT( _isInitialized );

	return _materials.Size();
}


Material* AssetManagerImpl::GetMaterialAt( UInt index )
{
	ASSERT( _isInitialized );
	ASSERT( index < _materials.Size() );

	return _materials[index];
}


UInt AssetManagerImpl::GetShaderCount()
{
	ASSERT( _isInitialized );

	return _shaders.Size();
}


Shader* AssetManagerImpl::GetShaderAt( UInt index )
{
	ASSERT( _isInitialized );
	ASSERT( index < _shaders.Size() );

	return _shaders[index];
}


UInt AssetManagerImpl::GetTextureCount()
{
	ASSERT( _isInitialized );

	return _textures.Size();
}


Texture* AssetManagerImpl::GetTextureAt( UInt index )
{
	ASSERT( _isInitialized );
	ASSERT( index < _textures.Size() );

	return _textures[index];
}
