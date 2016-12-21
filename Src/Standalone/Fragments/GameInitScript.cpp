// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "GameInitScript.h"
#include "CyredBuildingBlocks.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "CyredModule_File.h"
#include "CyredModule_Render.h"


using namespace CYRED;



GameInitScript::GameInitScript( AppConfig* appConfig )
	: _appConfig( appConfig )
{
}


void GameInitScript::_OnStart()
{
	// load all assets
	_LoadAllAssets();

	// open start scene
	if ( _appConfig->startScene.path.GetLength() > 0 )
	{
		// find uid
		Scene* scene = Memory::Alloc<Scene>();
		scene->SetEmitEvents( FALSE );
		scene->SetName( _appConfig->startScene.path.GetChar(), FALSE );
		scene->SetDirPath( AppConfig::DIR_PATH_DATA );
		scene->LoadUniqueID();

		SceneManager::Singleton()->LoadScene( scene->GetUniqueID() );
	}
}


void GameInitScript::_LoadAllAssets()
{
	// load textures
	for ( UInt i = 0; i < _appConfig->assetTextures.Size(); i++ ) {
		// create asset
		Texture* texture = Memory::Alloc<Texture>();
		texture->SetEmitEvents( FALSE );
		texture->SetName( _appConfig->assetTextures[i].path.GetChar(), FALSE );
		texture->SetDirPath( AppConfig::DIR_PATH_DATA );
		texture->LoadFullFile();
		// manage asset
		AssetManager::Singleton()->AddTexture( texture );
	}

	// load meshes
	for ( UInt i = 0; i < _appConfig->assetMeshes.Size(); i++ ) {
		// create asset
		Mesh* mesh = Memory::Alloc<Mesh>();
		mesh->SetEmitEvents( FALSE );
		mesh->SetName( _appConfig->assetMeshes[i].path.GetChar(), FALSE );
		mesh->SetDirPath( AppConfig::DIR_PATH_DATA );
		mesh->LoadFullFile();
		// manage asset
		AssetManager::Singleton()->AddMesh( mesh );
	}

	// load morphs
	for ( UInt i = 0; i < _appConfig->assetMorphs.Size(); i++ ) {
		// create asset
		Morph* morph = Memory::Alloc<Morph>();
		morph->SetEmitEvents( FALSE );
		morph->SetName( _appConfig->assetMorphs[i].path.GetChar(), FALSE );
		morph->SetDirPath( AppConfig::DIR_PATH_DATA );
		morph->LoadFullFile();
		// manage asset
		AssetManager::Singleton()->AddMorph( morph );
	}

	// load shaders
	for ( UInt i = 0; i < _appConfig->assetShaders.Size(); i++ ) {
		// create asset
		Shader* shader = Memory::Alloc<Shader>();
		shader->SetEmitEvents( FALSE );
		shader->SetName( _appConfig->assetShaders[i].path.GetChar(), FALSE );
		shader->SetDirPath( AppConfig::DIR_PATH_DATA );
		shader->LoadFullFile();
		// manage asset
		AssetManager::Singleton()->AddShader( shader );
	}

	// load materials after shaders
	for ( UInt i = 0; i < _appConfig->assetMaterials.Size(); i++ ) {
		// create asset
		Material* material = Memory::Alloc<Material>();
		material->SetEmitEvents( FALSE );
		material->SetName( _appConfig->assetMaterials[i].path.GetChar(), FALSE );
		material->SetDirPath( AppConfig::DIR_PATH_DATA );
		material->LoadFullFile();
		// manage asset
		AssetManager::Singleton()->AddMaterial( material );
	}

	// load scenes after all
	for ( UInt i = 0; i < _appConfig->assetScenes.Size(); i++ ) {
		// create asset
		Scene* scene = Memory::Alloc<Scene>();
		scene->SetEmitEvents( FALSE );
		scene->SetName( _appConfig->assetScenes[i].path.GetChar(), FALSE );
		scene->SetDirPath( AppConfig::DIR_PATH_DATA );
		scene->LoadUniqueID();
		// manage asset
		AssetManager::Singleton()->AddScene( scene );
	}
}

