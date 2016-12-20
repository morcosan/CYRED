// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "ProjectBuilderImpl.h"

#include "CyredModule_File.h"
#include "CyredBuildingBlocks.h"
#include "CyredApplication.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Scene.h"

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

	// file manager shortcut
	FileManager* fileManager = FileManager::Singleton();

	// write app config
	FiniteString filePath( "%s%s", ProjectSettings::dirPathBuildWindows.GetChar(), 
								   AppConfig::FILE_PATH_CONFIG );
	fileManager->WriteFile( filePath.GetChar(), 
							fileManager->Serialize<AppConfig>( &ProjectSettings::appConfig ).GetChar() );

	// recreate build dir
	{
		// delete dir
		FiniteString dirPath( "%s%s", ProjectSettings::dirPathBuildWindows.GetChar(),
									  AppConfig::DIR_PATH_DATA );
		fileManager->DeleteDir( dirPath.GetChar() );

		// create dir
		fileManager->CreateDir( ProjectSettings::dirPathBuildWindows.GetChar(), AppConfig::DIR_PATH_DATA );
	}

	// copy files to build dir
	{
		// copy scenes
		for ( UInt i = 0; i < ProjectSettings::appConfig.scenes.Size(); i++ ) {
			// get scene
			Scene* scene = ProjectSettings::scenes[i];
			// get path
			FiniteString filePath( "%s%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(), 
											   AppConfig::DIR_PATH_DATA,
											   ProjectSettings::appConfig.scenes[i].GetChar(),
											   FileManager::FILE_FORMAT_SCENEDB );
			// write scene db file
			SceneDB sceneDB { scene };
			fileManager->WriteFile( filePath.GetChar(), 
									fileManager->Serialize<SceneDB>( &sceneDB ).GetChar() );
		}
	}

}


void ProjectBuilderImpl::BuildAndroid( const Char* buildPath )
{
	ASSERT( _isInitialized );
}
