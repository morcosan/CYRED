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
		//_BuildAssetFiles( assetManager->GetMaterialAt( i ) );
	}
	// copy meshes
	for ( UInt i = 0; i < assetManager->GetMeshCount(); i++ ) {
		_BuildAssetFiles( assetManager->GetMeshAt( i ) );
	}
	// copy scenes
	for ( UInt i = 0; i < assetManager->GetSceneCount(); i++ ) {
		//_BuildAssetFiles( assetManager->GetSceneAt( i ) );
	}
	// copy morphs
	for ( UInt i = 0; i < assetManager->GetMorphCount(); i++ ) {
		//_BuildAssetFiles( assetManager->GetMorphAt( i ) );
	}
	// copy textures
	for ( UInt i = 0; i < assetManager->GetTextureCount(); i++ ) {
		//_BuildAssetFiles( assetManager->GetTextureAt( i ) );
	}
	// copy shaders
	for ( UInt i = 0; i < assetManager->GetShaderCount(); i++ ) {
		//_BuildAssetFiles( assetManager->GetShaderAt( i ) );
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
	FiniteString srcPathAsset( "%s%s", asset->GetDirPath(), asset->GetName() );
	FiniteString dstPathAsset( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
										 AppConfig::DIR_PATH_DATA, 
										 asset->GetUniqueID() );

	switch ( asset->GetAssetType() ) {
		case AssetType::MATERIAL:
		{
			srcPathAsset.Set( "%s%s", srcPathAsset.GetChar(), FileManager::FILE_FORMAT_MATERIAL );
			break;
		}
			
		case AssetType::MESH:
		{
			srcPathAsset.Set( "%s%s", srcPathAsset.GetChar(), FileManager::FILE_FORMAT_MESH );
			
			// copy additional files
			Mesh* mesh = CAST_S( Mesh*, asset );
			FiniteString srcPath( "%s%s", asset->GetDirPath(), mesh->GetExternalPath() );
			FiniteString dstPath( "%s%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
											  AppConfig::DIR_PATH_DATA, 
											  mesh->GetExternalPath(),
											  FileManager::FILE_FORMAT_MESHDATA );
			// load data
			DataArray<Vertex>	vertices;
			DataArray<UInt>		indices;
			Int fileSize;
			Char* fileData = FileManager::Singleton()->ReadFile( srcPath.GetChar(), fileSize );
			if ( fileData != NULL ) {
				// try custom format first, then try import
				Bool isLoaded = FileManager::Singleton()->LoadMesh( fileData, vertices, indices );
				if ( !isLoaded ) {
					FileManager::Singleton()->ImportMesh( fileData, fileSize, vertices, indices );
				}

				// write data
				String data( FileManager::Singleton()->SaveMesh( vertices, indices ) );
				FileManager::Singleton()->WriteFile( dstPath.GetChar(), data.GetChar() );

				FileManager::Singleton()->LoadMesh( data.GetChar(), vertices, indices );
			}

			break;
		}

		case AssetType::MORPH:
		{
			srcPathAsset.Set( "%s%s", srcPathAsset.GetChar(), FileManager::FILE_FORMAT_MORPH );
			break;
		}

		case AssetType::SCENE:
		{
			srcPathAsset.Set( "%s%s", srcPathAsset.GetChar(), FileManager::FILE_FORMAT_SCENE );
			break;
		}

		case AssetType::SHADER:
		{
			srcPathAsset.Set( "%s%s", srcPathAsset.GetChar(), FileManager::FILE_FORMAT_SHADER );
			
			// copy additional files
			Shader* shader = CAST_S( Shader*, asset );
			const Char* vertexFile;
			const Char* geometryFile;
			const Char* fragmentFile;
			FiniteString srcPath;
			FiniteString dstPath;
			// get files
			shader->GetShaderFiles( "FORWARD", &vertexFile, &geometryFile, &fragmentFile );
			// copy vertex file
			srcPath.Set( "%s%s", asset->GetDirPath(), vertexFile );
			dstPath.Set( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
								   AppConfig::DIR_PATH_DATA, vertexFile );
			FileManager::Singleton()->CopyFile( srcPath.GetChar(), dstPath.GetChar() );
			// copy geometry file
			srcPath.Set( "%s%s", asset->GetDirPath(), geometryFile );
			dstPath.Set( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
								   AppConfig::DIR_PATH_DATA, geometryFile );
			FileManager::Singleton()->CopyFile( srcPath.GetChar(), dstPath.GetChar() );
			// copy fragment file
			srcPath.Set( "%s%s", asset->GetDirPath(), fragmentFile );
			dstPath.Set( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
								   AppConfig::DIR_PATH_DATA, fragmentFile );
			FileManager::Singleton()->CopyFile( srcPath.GetChar(), dstPath.GetChar() );

			break;
		}

		case AssetType::TEXTURE:
		{
			srcPathAsset.Set( "%s%s", srcPathAsset.GetChar(), FileManager::FILE_FORMAT_TEXTURE );

			// copy additional files
			Texture* texture = CAST_S( Texture*, asset );
			if ( texture->GetTextureType() == TextureType::TEXTURE_2D ) {
				// single texture
				FiniteString srcPath( "%s%s", asset->GetDirPath(), texture->GetImagePath(0) );
				FiniteString dstPath( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
												AppConfig::DIR_PATH_DATA, 
												texture->GetImagePath(0) );
				FileManager::Singleton()->CopyFile( srcPath.GetChar(), dstPath.GetChar() );
			}
			else {
				// cube texture
				for ( UInt i = 0; i < 6; i++ ) {
					FiniteString srcPath( "%s%s", asset->GetDirPath(), texture->GetImagePath(i) );
					FiniteString dstPath( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
													AppConfig::DIR_PATH_DATA, 
													texture->GetImagePath(i) );
					FileManager::Singleton()->CopyFile( srcPath.GetChar(), dstPath.GetChar() );
				}
			}

			break;
		}
	}

	// copy file
	FileManager::Singleton()->CopyFile( srcPathAsset.GetChar(), dstPathAsset.GetChar() );
}
