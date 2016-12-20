// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "ProjectBuilderImpl.h"

#include "CyredModule_File.h"
#include "CyredBuildingBlocks.h"
#include "CyredApplication.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Render.h"

#include "../Settings/ProjectSettings.h"


using namespace CYRED;


//! deferred definition of ProjectBuilder
DEFINE_LOCAL_SINGLETON( ProjectBuilder, ProjectBuilderImpl )
DEFINE_LOCAL_SINGLETON_IMPL( ProjectBuilderImpl )


void ProjectBuilderImpl::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = true;
}


void ProjectBuilderImpl::Finalize()
{
	ASSERT( _isInitialized );
}


void ProjectBuilderImpl::BuildWindows( const Char* buildPath )
{
	ASSERT( _isInitialized );

	// shortcut manager
	AssetManager* assetManager = AssetManager::Singleton();
	FileManager* fileManager = FileManager::Singleton();

	// add all assets to app config 
	{
		// clear lists
		ProjectSettings::appConfig.assetMaterials.Clear();
		ProjectSettings::appConfig.assetMeshes.Clear();
		ProjectSettings::appConfig.assetMorphs.Clear();
		ProjectSettings::appConfig.assetScenes.Clear();
		ProjectSettings::appConfig.assetShaders.Clear();
		ProjectSettings::appConfig.assetTextures.Clear();

		// fill lists
		for ( UInt i = 0; i < assetManager->GetMaterialCount(); i++ ) {
			Material* asset = assetManager->GetMaterialAt( i );
			ProjectSettings::appConfig.assetMaterials.Add( AppConfig::AssetConfig {
				asset->GetName(),
				asset->GetUniqueID()
			} );
		}
		for ( UInt i = 0; i < assetManager->GetMeshCount(); i++ ) {
			Mesh* asset = assetManager->GetMeshAt( i );
			ProjectSettings::appConfig.assetMeshes.Add( AppConfig::AssetConfig {
				asset->GetName(),
				asset->GetUniqueID()
			} );
		}
		for ( UInt i = 0; i < assetManager->GetSceneCount(); i++ ) {
			Scene* asset = assetManager->GetSceneAt( i );
			ProjectSettings::appConfig.assetScenes.Add( AppConfig::AssetConfig {
				asset->GetName(),
				asset->GetUniqueID()
			} );
		}
		for ( UInt i = 0; i < assetManager->GetMorphCount(); i++ ) {
			Morph* asset = assetManager->GetMorphAt( i );
			ProjectSettings::appConfig.assetMorphs.Add( AppConfig::AssetConfig {
				asset->GetName(),
				asset->GetUniqueID()
			} );
		}
		for ( UInt i = 0; i < assetManager->GetTextureCount(); i++ ) {
			Texture* asset = assetManager->GetTextureAt( i );
			ProjectSettings::appConfig.assetTextures.Add( AppConfig::AssetConfig {
				asset->GetName(),
				asset->GetUniqueID()
			} );
		}
		for ( UInt i = 0; i < assetManager->GetShaderCount(); i++ ) {
			Shader* asset = assetManager->GetShaderAt( i );
			ProjectSettings::appConfig.assetShaders.Add( AppConfig::AssetConfig {
				asset->GetName(),
				asset->GetUniqueID()
			} );
		}
	}


	// write app config
	FiniteString filePath( "%s%s", ProjectSettings::dirPathBuildWindows.GetChar(), 
								   AppConfig::FILE_PATH_CONFIG );
	fileManager->WriteFile( filePath.GetChar(), 
							fileManager->Serialize<AppConfig>( &ProjectSettings::appConfig ).GetChar() );


	// recreate build dir
	// delete dir
	FiniteString dirPath( "%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
									AppConfig::DIR_PATH_DATA );
	fileManager->DeleteDir( dirPath.GetChar() );
	// create dir
	fileManager->CreateDir( ProjectSettings::dirPathBuildWindows.GetChar(), AppConfig::DIR_PATH_DATA );
	

	// copy assets to build dir
	// copy materials
	for ( UInt i = 0; i < assetManager->GetMaterialCount(); i++ ) {
		_BuildAssetFiles( assetManager->GetMaterialAt( i ) );
	}
	// copy meshes
	for ( UInt i = 0; i < assetManager->GetMeshCount(); i++ ) {
		_BuildAssetFiles( assetManager->GetMeshAt( i ) );
	}
	// copy scenes
	for ( UInt i = 0; i < assetManager->GetSceneCount(); i++ ) {
		_BuildAssetFiles( assetManager->GetSceneAt( i ) );
	}
	// copy morphs
	for ( UInt i = 0; i < assetManager->GetMorphCount(); i++ ) {
		_BuildAssetFiles( assetManager->GetMorphAt( i ) );
	}
	// copy textures
	for ( UInt i = 0; i < assetManager->GetTextureCount(); i++ ) {
		_BuildAssetFiles( assetManager->GetTextureAt( i ) );
	}
	// copy shaders
	for ( UInt i = 0; i < assetManager->GetShaderCount(); i++ ) {
		_BuildAssetFiles( assetManager->GetShaderAt( i ) );
	}
}


void ProjectBuilderImpl::BuildAndroid( const Char* buildPath )
{
	ASSERT( _isInitialized );
}


void ProjectBuilderImpl::_BuildAssetFiles( Asset* asset )
{
	ASSERT( _isInitialized );
	ASSERT( asset != NULL );

	// create paths
	FiniteString srcPath( "%s%s", asset->GetDirPath(), asset->GetName() );
	FiniteString dstPath( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
									AppConfig::DIR_PATH_DATA, 
									asset->GetUniqueID() );

	switch ( asset->GetAssetType() ) {
		case AssetType::MATERIAL:
		{
			srcPath.Set( "%s%s", srcPath.GetChar(), FileManager::FILE_FORMAT_MATERIAL );
			break;
		}
			
		case AssetType::MATERIAL:
		{
			srcPath.Set( "%s%s", srcPath.GetChar(), FileManager::FILE_FORMAT_MATERIAL );
			break;
		}

		case AssetType::MATERIAL:
		{
			srcPath.Set( "%s%s", srcPath.GetChar(), FileManager::FILE_FORMAT_MATERIAL );
			break;
		}

		case AssetType::MATERIAL:
		{
			srcPath.Set( "%s%s", srcPath.GetChar(), FileManager::FILE_FORMAT_MATERIAL );
			break;
		}

		case AssetType::MATERIAL:
		{
			srcPath.Set( "%s%s", srcPath.GetChar(), FileManager::FILE_FORMAT_MATERIAL );
			break;
		}

		case AssetType::MATERIAL:
		{
			srcPath.Set( "%s%s", srcPath.GetChar(), FileManager::FILE_FORMAT_MATERIAL );
			break;
		}
	}

	// copy file
	FileManager::Singleton()->CopyFile( srcPath.GetChar(), dstPath.GetChar() );
}
