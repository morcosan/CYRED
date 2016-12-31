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
		_BuildMaterialFile( assetManager->GetMaterialAt( i ) );
	}
	// copy meshes
	for ( UInt i = 0; i < assetManager->GetMeshCount(); i++ ) {
		_BuildMeshFile( assetManager->GetMeshAt( i ) );
	}
	// copy scenes
	for ( UInt i = 0; i < assetManager->GetSceneCount(); i++ ) {
		_BuildSceneFile( assetManager->GetSceneAt( i ) );
	}
	// copy morphs
	for ( UInt i = 0; i < assetManager->GetMorphCount(); i++ ) {
		_BuildMorphFile( assetManager->GetMorphAt( i ) );
	}
	// copy textures
	for ( UInt i = 0; i < assetManager->GetTextureCount(); i++ ) {
		_BuildTextureFile( assetManager->GetTextureAt( i ) );
	}
	// copy shaders
	for ( UInt i = 0; i < assetManager->GetShaderCount(); i++ ) {
		_BuildShaderFile( assetManager->GetShaderAt( i ) );
	}
}


void ProjectBuilderImpl::BuildAndroid( const Char* buildPath )
{
	ASSERT( _isInitialized );
}


void ProjectBuilderImpl::_BuildTextureFile( Texture* asset )
{
	ASSERT( asset != NULL );

	// use shortcut
	FileManager* fileManager = FileManager::Singleton();

	// copy additional files, rename, replace in asset file

	// read asset file
	Texture texture;
	texture.SetEmitEvents( FALSE );
	texture.SetName( asset->GetName() );
	texture.SetDirPath( asset->GetDirPath() );
	texture.LoadFullFile();

	// single texture
	if ( texture.GetTextureType() == TextureType::TEXTURE_2D ) {
		// generate constant ids
		FiniteString srcPath( "%s%s", asset->GetDirPath(), texture.GetImagePath(0) );
		String fileCID = Random::GenerateConstantID( srcPath.GetChar() );

		FiniteString dstPath( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
										AppConfig::DIR_PATH_DATA, 
										fileCID.GetChar() );
		fileManager->CopyFile( srcPath.GetChar(), dstPath.GetChar() );

		// replace data file in asset
		texture.SetImagePath( 0, fileCID.GetChar() );
	}
	// cube texture
	else {
		for ( UInt i = 0; i < 6; i++ ) {
			// generate constant ids
			FiniteString srcPath( "%s%s", asset->GetDirPath(), texture.GetImagePath(0) );
			String fileCID = Random::GenerateConstantID( srcPath.GetChar() );

			FiniteString dstPath( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
											AppConfig::DIR_PATH_DATA, 
											fileCID.GetChar() );
			fileManager->CopyFile( srcPath.GetChar(), dstPath.GetChar() );

			// replace data file in asset
			texture.SetImagePath( i, fileCID.GetChar() );
		}
	}

	// write asset file
	FiniteString dstPathAsset( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
										 AppConfig::DIR_PATH_DATA, 
										 asset->GetUniqueID() );
	fileManager->WriteFile( dstPathAsset.GetChar(), 
							fileManager->Serialize<Texture>( &texture ).GetChar() );
}


void ProjectBuilderImpl::_BuildSceneFile( Scene* asset )
{
	ASSERT( asset != NULL );

	// use shortcut
	FileManager* fileManager = FileManager::Singleton();

	// create paths
	FiniteString srcPathAsset( "%s%s%s", asset->GetDirPath(), asset->GetName(), 
										 FileManager::FILE_FORMAT_SCENE );
	FiniteString dstPathAsset( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
										 AppConfig::DIR_PATH_DATA, 
										 asset->GetUniqueID() );
	// copy file
	fileManager->CopyFile( srcPathAsset.GetChar(), dstPathAsset.GetChar() );
}


void ProjectBuilderImpl::_BuildMaterialFile( Material* asset )
{
	ASSERT( asset != NULL );

	// use shortcut
	FileManager* fileManager = FileManager::Singleton();

	// create paths
	FiniteString srcPathAsset( "%s%s%s", asset->GetDirPath(), asset->GetName(), 
										 FileManager::FILE_FORMAT_MATERIAL );
	FiniteString dstPathAsset( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
										 AppConfig::DIR_PATH_DATA, 
										 asset->GetUniqueID() );
	// copy file
	fileManager->CopyFile( srcPathAsset.GetChar(), dstPathAsset.GetChar() );
}


void ProjectBuilderImpl::_BuildMeshFile( Mesh* asset )
{
	ASSERT( asset != NULL );

	// use shortcut
	FileManager* fileManager = FileManager::Singleton();

	// copy additional files, rename, replace in asset file

	// read asset file
	Mesh mesh;
	mesh.SetEmitEvents( FALSE );
	mesh.SetName( asset->GetName() );
	mesh.SetDirPath( asset->GetDirPath() );
	mesh.LoadFullFile();

	// generate constant ids
	FiniteString srcPathFile( "%s%s", asset->GetDirPath(), mesh.GetExternalPath() );
	String fileCID = Random::GenerateConstantID( srcPathFile.GetChar() );

	// load data
	DataArray<Vertex>	vertices;
	DataArray<UInt>		indices;
	Int fileSize;
	Char* fileData = FileManager::Singleton()->ReadFile( srcPathFile.GetChar(), fileSize );
	if ( fileData != NULL ) {
		// try custom format first, then try import
		Bool isLoaded = FileManager::Singleton()->LoadMesh( fileData, vertices, indices );
		if ( !isLoaded ) {
			FileManager::Singleton()->ImportMesh( fileData, fileSize, vertices, indices );
		}

		// write data
		FiniteString dstPathFile( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
											AppConfig::DIR_PATH_DATA, 
											fileCID.GetChar() );
		fileManager->WriteFile( dstPathFile.GetChar(), 
								fileManager->SaveMesh( vertices, indices ).GetChar() );
	}

	// replace data file in asset
	mesh.SetExternalPath( fileCID.GetChar() );

	// write asset file
	FiniteString dstPathAsset( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
										 AppConfig::DIR_PATH_DATA, 
										 asset->GetUniqueID() );
	fileManager->WriteFile( dstPathAsset.GetChar(), 
							fileManager->Serialize<Mesh>( &mesh ).GetChar() );
}


void ProjectBuilderImpl::_BuildMorphFile( Morph* asset )
{
	ASSERT( asset != NULL );

	// use shortcut
	FileManager* fileManager = FileManager::Singleton();

	// copy additional files, rename, replace in asset file

	// read asset file
	Morph morph;
	morph.SetEmitEvents( FALSE );
	morph.SetName( asset->GetName() );
	morph.SetDirPath( asset->GetDirPath() );
	morph.LoadFullFile();

	for ( UInt i = 0; i < morph.GetTotalStates(); i++ ) {
		// generate constant ids
		FiniteString srcPathFile( "%s%s", asset->GetDirPath(), morph.GetFilePath(i) );
		String fileCID = Random::GenerateConstantID( srcPathFile.GetChar() );

		// load data
		DataArray<Vertex>	vertices;
		DataArray<UInt>		indices;
		Int fileSize;
		Char* fileData = FileManager::Singleton()->ReadFile( srcPathFile.GetChar(), fileSize );
		if ( fileData != NULL ) {
			// try custom format first, then try import
			Bool isLoaded = FileManager::Singleton()->LoadMesh( fileData, vertices, indices );
			if ( !isLoaded ) {
				FileManager::Singleton()->ImportMesh( fileData, fileSize, vertices, indices );
			}

			// write data
			FiniteString dstPathFile( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
												AppConfig::DIR_PATH_DATA, 
												fileCID.GetChar() );
			fileManager->WriteFile( dstPathFile.GetChar(), 
									fileManager->SaveMesh( vertices, indices ).GetChar() );
		}

		// replace data file in asset
		morph.SetFilePath( i, fileCID.GetChar() );
	}

	// write asset file
	FiniteString dstPathAsset( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
										 AppConfig::DIR_PATH_DATA, 
										 asset->GetUniqueID() );
	fileManager->WriteFile( dstPathAsset.GetChar(), 
							fileManager->Serialize<Morph>( &morph ).GetChar() );
}


void ProjectBuilderImpl::_BuildShaderFile( Shader* asset )
{
	ASSERT( asset != NULL );

	// use shortcut
	FileManager* fileManager = FileManager::Singleton();

	// copy additional files, rename, replace in asset file

	// read asset file
	Shader shader;
	shader.SetEmitEvents( FALSE );
	shader.SetName( asset->GetName() );
	shader.SetDirPath( asset->GetDirPath() );
	shader.LoadFullFile();

	const Char* vertexFile;
	const Char* geometryFile;
	const Char* fragmentFile;
	// get shader files
	shader.GetShaderFiles( "FORWARD", &vertexFile, &geometryFile, &fragmentFile );
	FiniteString srcPathVertex( "%s%s", asset->GetDirPath(), vertexFile );
	FiniteString srcPathGeometry( "%s%s", asset->GetDirPath(), geometryFile );
	FiniteString srcPathFragment( "%s%s", asset->GetDirPath(), fragmentFile );

	// generate constant ids
	String vertexCID	= Random::GenerateConstantID( srcPathVertex.GetChar() );
	String geometryCID	= Random::GenerateConstantID( srcPathGeometry.GetChar() );
	String fragmentCID	= Random::GenerateConstantID( srcPathFragment.GetChar() );

	// copy vertex file
	{
		FiniteString dstPath( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
										AppConfig::DIR_PATH_DATA, 
										vertexCID.GetChar() );
		fileManager->CopyFile( srcPathVertex.GetChar(), dstPath.GetChar() );
	}
	// copy geometry file
	{
		FiniteString dstPath( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
										AppConfig::DIR_PATH_DATA, 
										geometryCID.GetChar() );
		fileManager->CopyFile( srcPathGeometry.GetChar(), dstPath.GetChar() );
	}
	// copy fragment file
	{
		FiniteString dstPath( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
										AppConfig::DIR_PATH_DATA, 
										fragmentCID.GetChar() );
		fileManager->CopyFile( srcPathFragment.GetChar(), dstPath.GetChar() );
	}

	// replace shader files in asset
	shader.SetShaderFiles( "FORWARD", vertexCID.GetChar(), geometryCID.GetChar(), fragmentCID.GetChar() );

	// write asset file
	FiniteString dstPathAsset( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
										 AppConfig::DIR_PATH_DATA, 
										 asset->GetUniqueID() );
	fileManager->WriteFile( dstPathAsset.GetChar(), 
							fileManager->Serialize<Shader>( &shader ).GetChar() );
}



//
//
//void ProjectBuilderImpl::_BuildAssetFiles( Asset* asset )
//{

//		case AssetType::TEXTURE:
//		{
//			srcPathAsset.Set( "%s%s", srcPathAsset.GetChar(), FileManager::FILE_FORMAT_TEXTURE );
//
//			// copy additional files
//			Texture* texture = CAST_S( Texture*, asset );
//			if ( texture->GetTextureType() == TextureType::TEXTURE_2D ) {
//				// single texture
//				FiniteString srcPath( "%s%s", asset->GetDirPath(), texture->GetImagePath(0) );
//				FiniteString dstPath( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
//												AppConfig::DIR_PATH_DATA, 
//												texture->GetImagePath(0) );
//				fileManager->CopyFile( srcPath.GetChar(), dstPath.GetChar() );
//			}
//			else {
//				// cube texture
//				for ( UInt i = 0; i < 6; i++ ) {
//					FiniteString srcPath( "%s%s", asset->GetDirPath(), texture->GetImagePath(i) );
//					FiniteString dstPath( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
//													AppConfig::DIR_PATH_DATA, 
//													texture->GetImagePath(i) );
//					fileManager->CopyFile( srcPath.GetChar(), dstPath.GetChar() );
//				}
//			}
//
//			break;
//		}
//	}
//}
