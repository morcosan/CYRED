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
	if ( _appConfig->scenes.Size() > 0 )
	{
		_LoadSceneAssets( _appConfig->scenes[0].GetChar() );
		_LoadScene( _appConfig->scenes[0].GetChar() );
	}
}


void GameInitScript::_LoadScene( const Char* sceneName )
{
	/*Scene* scene = Memory::Alloc<Scene>();
	scene->SetEmitEvents( FALSE );
	scene->SetName( sceneData.name.GetChar() );
	scene->SetDirPath( sceneData.dir.GetChar() );
	scene->LoadUniqueID();
	scene->SetEmitEvents( TRUE );

	AssetManager::Singleton()->AddScene( scene );

	SceneManager::Singleton()->OpenScene( scene->GetUniqueID() );*/
}


void GameInitScript::_LoadSceneAssets( const Char* sceneName )
{
	//AssetManager* assetMgr = AssetManager::Singleton();

	//assetMgr->ClearAll();

	//FiniteString filePath( "%s%s%s", sceneData.dir.GetChar(), 
	//								 sceneData.name.GetChar(),
	//								 FileManager::FILE_FORMAT_ASSETDB );

	//Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	//FileManager::Singleton()->Deserialize<AssetDB>( fileData, NULL );

	//// load assets full
	//{
	//	for ( UInt i = 0; i < assetMgr->GetMaterialCount(); ++i )
	//	{
	//		assetMgr->GetMaterialAt( i )->LoadFullFile();
	//	}
	//	for ( UInt i = 0; i < assetMgr->GetShaderCount(); ++i )
	//	{
	//		assetMgr->GetShaderAt( i )->LoadFullFile();
	//	}
	//	for ( UInt i = 0; i < assetMgr->GetTextureCount(); ++i )
	//	{
	//		assetMgr->GetTextureAt( i )->LoadFullFile();
	//	}
	//}
}
