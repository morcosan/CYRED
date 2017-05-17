// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "AssetManagerImpl.h"
#include "../../2_BuildingBlocks/Random/Random.h"
#include "../Render/Assets/Material.h"
#include "../Render/Assets/Texture.h"
#include "../Render/Assets/Mesh.h"
#include "../Render/Assets/Morph.h"
#include "../Render/Assets/Shader.h"
#include "../Scene/Sections/Scene.h"
#include "../Script/Assets/Script.h"
#include "../Asset/Assets/Prefab.h"


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
	if ( !_isInitialized ) {
		return;
	}
}


void AssetManagerImpl::ClearAll()
{
	// clear meshes
	for ( int i = 0; i < _meshes.Size(); ++i ) {
		bool emitEvents = _meshes[i]->DoesEmitEvents();
		_meshes[i]->SetEmitEvents( FALSE );
		_meshes[i]->ClearAsset();
		_meshes[i]->SetEmitEvents( emitEvents );
	}
	// clear materials
	for ( int i = 0; i < _materials.Size(); ++i ) {
		bool emitEvents = _materials[i]->DoesEmitEvents();
		_materials[i]->SetEmitEvents( FALSE );
		_materials[i]->ClearAsset();
		_materials[i]->SetEmitEvents( emitEvents );
	}
	// clear textures
	for ( int i = 0; i < _textures.Size(); ++i ) {
		bool emitEvents = _textures[i]->DoesEmitEvents();
		_textures[i]->SetEmitEvents( FALSE );
		_textures[i]->ClearAsset();
		_textures[i]->SetEmitEvents( emitEvents );
	}
	// clear shaders
	for ( int i = 0; i < _shaders.Size(); ++i ) {
		bool emitEvents = _shaders[i]->DoesEmitEvents();
		_shaders[i]->SetEmitEvents( FALSE );
		_shaders[i]->ClearAsset();
		_shaders[i]->SetEmitEvents( emitEvents );
	}
	// clear scripts
	for ( int i = 0; i < _scripts.Size(); ++i ) {
		bool emitEvents = _scripts[i]->DoesEmitEvents();
		_scripts[i]->SetEmitEvents( FALSE );
		_scripts[i]->ClearAsset();
		_scripts[i]->SetEmitEvents( emitEvents );
	}
	// clear prefabs
	for ( int i = 0; i < _prefabs.Size(); ++i ) {
		bool emitEvents = _prefabs[i]->DoesEmitEvents();
		_prefabs[i]->SetEmitEvents( FALSE );
		_prefabs[i]->ClearAsset();
		_prefabs[i]->SetEmitEvents( emitEvents );
	}

	// do not clear scenes
	for ( int i = 0; i < _scenes.Size(); ++i )	{
		_scenes[i]->SetIsTemporary( TRUE );
	}
}


StatusAssetAdd AssetManagerImpl::AddMesh( Mesh* mesh )
{
	ASSERT( _isInitialized );

	bool isOk = Random::ValidateUniqueID( mesh->GetUniqueID() );
	if ( !isOk ) {
		return StatusAssetAdd::FAIL_INVALID_ID;
	}

	Mesh* found = GetMesh( mesh->GetUniqueID() );
	if ( found != NULL ) {
		return StatusAssetAdd::FAIL_EXISTING;
	}

	_meshes.Add( mesh );

	return StatusAssetAdd::SUCCESS;
}

StatusAssetAdd AssetManagerImpl::AddMorph( Morph* morph )
{
	ASSERT( _isInitialized );

	bool isOk = Random::ValidateUniqueID( morph->GetUniqueID() );
	if ( !isOk ) {
		return StatusAssetAdd::FAIL_INVALID_ID;
	}

	Mesh* found = GetMesh( morph->GetUniqueID() );
	if ( found != NULL ) {
		return StatusAssetAdd::FAIL_EXISTING;
	}

	_morphs.Add( morph );

	return StatusAssetAdd::SUCCESS;
}


StatusAssetAdd AssetManagerImpl::AddMaterial( Material* material )
{
	ASSERT( _isInitialized );

	bool isOk = Random::ValidateUniqueID( material->GetUniqueID() );
	if ( !isOk ) {
		return StatusAssetAdd::FAIL_INVALID_ID;
	}

	Material* found = GetMaterial( material->GetUniqueID() );
	if ( found != NULL ) {
		return StatusAssetAdd::FAIL_EXISTING;
	}

	_materials.Add( material );

	return StatusAssetAdd::SUCCESS;
}


StatusAssetAdd AssetManagerImpl::AddShader( Shader* shader )
{
	ASSERT( _isInitialized );

	bool isOk = Random::ValidateUniqueID( shader->GetUniqueID() );
	if ( !isOk ) {
		return StatusAssetAdd::FAIL_INVALID_ID;
	}

	Shader* found = GetShader( shader->GetUniqueID() );
	if ( found != NULL ) {
		return StatusAssetAdd::FAIL_EXISTING;
	}

	_shaders.Add( shader );

	return StatusAssetAdd::SUCCESS;
}


StatusAssetAdd AssetManagerImpl::AddTexture( Texture* texture )
{
	ASSERT( _isInitialized );

	bool isOk = Random::ValidateUniqueID( texture->GetUniqueID() );
	if ( !isOk ) {
		return StatusAssetAdd::FAIL_INVALID_ID;
	}

	Texture* found = GetTexture( texture->GetUniqueID() );
	if ( found != NULL ) {
		return StatusAssetAdd::FAIL_EXISTING;
	}

	_textures.Add( texture );

	return StatusAssetAdd::SUCCESS;
}


StatusAssetAdd AssetManagerImpl::AddScene( Scene* scene )
{
	ASSERT( _isInitialized );

	bool isOk = Random::ValidateUniqueID( scene->GetUniqueID() );
	if ( !isOk ) {
		return StatusAssetAdd::FAIL_INVALID_ID;
	}

	Scene* found = GetScene( scene->GetUniqueID() );
	if ( found != NULL ) {
		return StatusAssetAdd::FAIL_EXISTING;
	}

	_scenes.Add( scene );

	return StatusAssetAdd::SUCCESS;
}


StatusAssetAdd AssetManagerImpl::AddScript( Script* script )
{
	ASSERT( _isInitialized );

	bool isOk = Random::ValidateUniqueID( script->GetUniqueID() );
	if ( !isOk ) {
		return StatusAssetAdd::FAIL_INVALID_ID;
	}

	Script* found = GetScript( script->GetUniqueID() );
	if ( found != NULL ) {
		return StatusAssetAdd::FAIL_EXISTING;
	}

	_scripts.Add( script );

	return StatusAssetAdd::SUCCESS;
}


StatusAssetAdd AssetManagerImpl::AddPrefab( Prefab* prefab )
{
	ASSERT( _isInitialized );

	bool isOk = Random::ValidateUniqueID( prefab->GetUniqueID() );
	if ( !isOk ) {
		return StatusAssetAdd::FAIL_INVALID_ID;
	}

	Prefab* found = GetPrefab( prefab->GetUniqueID() );
	if ( found != NULL ) {
		return StatusAssetAdd::FAIL_EXISTING;
	}

	_prefabs.Add( prefab );

	return StatusAssetAdd::SUCCESS;
}


void AssetManagerImpl::RemoveMesh( Mesh* mesh )
{
	ASSERT( _isInitialized );

	if ( mesh == NULL ) {
		return;
	}

	// find index in array
	int index = -1;
	for ( int i = 0; i < _meshes.Size(); i++ ) {
		if ( _meshes[i] == mesh ) {
			index = i;
			break;
		}
	}

	if ( index > -1 ) {
		// remove from index
		_meshes.Erase( index );
	}
}


void AssetManagerImpl::RemoveMorph( Morph* morph )
{
	ASSERT( _isInitialized );

	if ( morph == NULL ) {
		return;
	}

	// find index in array
	int index = -1;
	for ( int i = 0; i < _morphs.Size(); i++ ) {
		if ( _morphs[i] == morph ) {
			index = i;
			break;
		}
	}

	if ( index > -1 ) {
		// remove from index
		_morphs.Erase( index );
	}
}


void AssetManagerImpl::RemoveMaterial( Material* material )
{
	ASSERT( _isInitialized );

	if ( material == NULL ) {
		return;
	}

	// find index in array
	int index = -1;
	for ( int i = 0; i < _materials.Size(); i++ ) {
		if ( _materials[i] == material ) {
			index = i;
			break;
		}
	}

	if ( index > -1 ) {
		// remove from index
		_materials.Erase( index );
	}
}


void AssetManagerImpl::RemoveShader( Shader* shader )
{
	ASSERT( _isInitialized );

	if ( shader == NULL ) {
		return;
	}

	// find index in array
	int index = -1;
	for ( int i = 0; i < _shaders.Size(); i++ ) {
		if ( _shaders[i] == shader ) {
			index = i;
			break;
		}
	}

	if ( index > -1 ) {
		// remove from index
		_shaders.Erase( index );
	}
}


void AssetManagerImpl::RemoveTexture( Texture* texture )
{
	ASSERT( _isInitialized );

	if ( texture == NULL ) {
		return;
	}

	// find index in array
	int index = -1;
	for ( int i = 0; i < _textures.Size(); i++ ) {
		if ( _textures[i] == texture ) {
			index = i;
			break;
		}
	}

	if ( index > -1 ) {
		// remove from index
		_textures.Erase( index );
	}
}


void AssetManagerImpl::RemoveScene( Scene* scene )
{
	ASSERT( _isInitialized );

	if ( scene == NULL ) {
		return;
	}

	// find index in array
	int index = -1;
	for ( int i = 0; i < _scenes.Size(); i++ ) {
		if ( _scenes[i] == scene ) {
			index = i;
			break;
		}
	}

	if ( index > -1 ) {
		// remove from index
		_scenes.Erase( index );
	}
}


void AssetManagerImpl::RemoveScript( Script* script )
{
	ASSERT( _isInitialized );

	if ( script == NULL ) {
		return;
	}

	// find index in array
	int index = -1;
	for ( int i = 0; i < _scripts.Size(); i++ ) {
		if ( _scripts[i] == script ) {
			index = i;
			break;
		}
	}

	if ( index > -1 ) {
		// remove from index
		_scripts.Erase( index );
	}
}


void AssetManagerImpl::RemovePrefab( Prefab* prefab )
{
	ASSERT( _isInitialized );

	if ( prefab == NULL ) {
		return;
	}

	// find index in array
	int index = -1;
	for ( int i = 0; i < _prefabs.Size(); i++ ) {
		if ( _prefabs[i] == prefab ) {
			index = i;
			break;
		}
	}

	if ( index > -1 ) {
		// remove from index
		_prefabs.Erase( index );
	}
}


Mesh* AssetManagerImpl::GetMesh( const char* uniqueID )
{
	ASSERT( _isInitialized );

	String temp( uniqueID );

	for ( int i = 0; i < _meshes.Size(); ++i ) {
		if ( temp == _meshes[i]->GetUniqueID() ) {
			return _meshes[i];
		}
	}
	
	return NULL;
}

Morph* AssetManagerImpl::GetMorph( const char* uniqueID )
{
	ASSERT( _isInitialized );

	String temp( uniqueID );

	for ( int i = 0; i < _morphs.Size(); ++i ) {
		if ( temp == _morphs[i]->GetUniqueID() ) {
			return _morphs[i];
		}
	}
	
	return NULL;
}


Material* AssetManagerImpl::GetMaterial( const char* uniqueID )
{
	ASSERT( _isInitialized );

	String temp( uniqueID );

	for ( int i = 0; i < _materials.Size(); ++i ) {
		if ( temp == _materials[i]->GetUniqueID() ) {
			return _materials[i];
		}
	}

	return NULL;
}


Shader* AssetManagerImpl::GetShader( const char* uniqueID )
{
	ASSERT( _isInitialized );

	String temp( uniqueID );

	for ( int i = 0; i < _shaders.Size(); ++i ) {
		if ( temp == _shaders[i]->GetUniqueID() ) {
			return _shaders[i];
		}
	}

	return NULL;
}


Texture* AssetManagerImpl::GetTexture( const char* uniqueID )
{
	ASSERT( _isInitialized );

	String temp( uniqueID );

	for ( int i = 0; i < _textures.Size(); ++i ) {
		if ( temp == _textures[i]->GetUniqueID() ) {
			return _textures[i];
		}
	}

	return NULL;
}


Scene* AssetManagerImpl::GetScene( const char* uniqueID )
{
	ASSERT( _isInitialized );

	String temp( uniqueID );

	for ( int i = 0; i < _scenes.Size(); ++i ) {
		if ( temp == _scenes[i]->GetUniqueID() ) {
			return _scenes[i];
		}
	}

	return NULL;
}


Script* AssetManagerImpl::GetScript( const char * uniqueID )
{
	ASSERT( _isInitialized );

	String temp( uniqueID );

	for ( int i = 0; i < _scripts.Size(); ++i ) {
		if ( temp == _scripts[i]->GetUniqueID() ) {
			return _scripts[i];
		}
	}

	return NULL;
}


Prefab* AssetManagerImpl::GetPrefab( const char* uniqueID )
{
	ASSERT( _isInitialized );

	String temp( uniqueID );

	for ( int i = 0; i < _prefabs.Size(); ++i ) {
		if ( temp == _prefabs[i]->GetUniqueID() ) {
			return _prefabs[i];
		}
	}

	return NULL;
}


int AssetManagerImpl::GetMaterialCount()
{
	ASSERT( _isInitialized );

	return _materials.Size();
}


Material* AssetManagerImpl::GetMaterialAt( int index )
{
	ASSERT( _isInitialized );
	ASSERT( index < _materials.Size() );

	return _materials[index];
}


int AssetManagerImpl::GetMeshCount()
{
	ASSERT( _isInitialized );

	return _meshes.Size();
}


Mesh* AssetManagerImpl::GetMeshAt( int index )
{
	ASSERT( _isInitialized );
	ASSERT( index < _meshes.Size() );

	return _meshes[index];
}


int AssetManagerImpl::GetMorphCount()
{
	ASSERT( _isInitialized );

	return _morphs.Size();
}


Morph* AssetManagerImpl::GetMorphAt( int index )
{
	ASSERT( _isInitialized );
	ASSERT( index < _morphs.Size() );

	return _morphs[index];
}


int AssetManagerImpl::GetShaderCount()
{
	ASSERT( _isInitialized );

	return _shaders.Size();
}


Shader* AssetManagerImpl::GetShaderAt( int index )
{
	ASSERT( _isInitialized );
	ASSERT( index < _shaders.Size() );

	return _shaders[index];
}


int AssetManagerImpl::GetTextureCount()
{
	ASSERT( _isInitialized );

	return _textures.Size();
}


Texture* AssetManagerImpl::GetTextureAt( int index )
{
	ASSERT( _isInitialized );
	ASSERT( index < _textures.Size() );

	return _textures[index];
}


int AssetManagerImpl::GetSceneCount()
{
	ASSERT( _isInitialized );
	return _scenes.Size();
}


Scene* AssetManagerImpl::GetSceneAt( int index )
{
	ASSERT( _isInitialized );
	ASSERT( index < _scenes.Size() );
	return _scenes[index];
}


int AssetManagerImpl::GetScriptCount()
{
	ASSERT( _isInitialized );
	return _scripts.Size();
}


Script* AssetManagerImpl::GetScriptAt( int index )
{
	ASSERT( _isInitialized );
	ASSERT( index < _scripts.Size() );
	return _scripts[index];
}

int AssetManagerImpl::GetPrefabCount()
{
	ASSERT( _isInitialized );
	return _prefabs.Size();
}

Prefab* AssetManagerImpl::GetPrefabAt( int index )
{
	ASSERT( _isInitialized );
	ASSERT( index < _prefabs.Size() );
	return _prefabs[index];
}

