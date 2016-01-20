// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "GameInitScript.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "CyredModule_File.h"
#include "CyredModule_Render.h"


using namespace CYRED;


void GameInitScript::_OnStart()
{
	if ( AppSettings::scenesData.Size() > 0 )
	{
		_LoadSceneAssets( AppSettings::scenesData[0] );
		_LoadScene( AppSettings::scenesData[0] );
	}
}


void GameInitScript::_LoadScene( AppSettings::SceneData& sceneData )
{
	Scene* scene = Memory::Alloc<Scene>();
	scene->SetEmitEvents( FALSE );
	scene->SetName( sceneData.sceneName.GetChar() );
	scene->SetDirPath( sceneData.sceneDir.GetChar() );
	scene->LoadUniqueID();
	scene->SetEmitEvents( TRUE );

	AssetManager::Singleton()->AddScene( scene );

	SceneManager::Singleton()->OpenScene( scene->GetUniqueID() );
}


void GameInitScript::_LoadSceneAssets( AppSettings::SceneData& sceneData )
{
	AssetManager* assetMgr = AssetManager::Singleton();

	assetMgr->ClearAll();

	Char filePath[MAX_SIZE_CUSTOM_STRING];
	CUSTOM_STRING( filePath, "%s%s%s", sceneData.assetdbDir.GetChar(), 
									   sceneData.sceneName.GetChar(),
									   FileManager::FILE_FORMAT_ASSETDB );

	Char* fileData = FileManager::Singleton()->ReadFile( filePath );
	FileManager::Singleton()->Deserialize<AppSettings::SceneData>( fileData, NULL );

	// load assets full
	{
		for ( UInt i = 0; i < assetMgr->GetMaterialCount(); ++i )
		{
			assetMgr->GetMaterialAt( i )->LoadFullFile();
		}
		for ( UInt i = 0; i < assetMgr->GetShaderCount(); ++i )
		{
			assetMgr->GetShaderAt( i )->LoadFullFile();
		}
		for ( UInt i = 0; i < assetMgr->GetTextureCount(); ++i )
		{
			assetMgr->GetTextureAt( i )->LoadFullFile();
		}
	}
}
