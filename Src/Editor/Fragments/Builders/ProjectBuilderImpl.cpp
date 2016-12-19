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

	// copy files to build dir, renaming them by uid
	{
		// copy scenes
		for ( UInt i = 0; i < ProjectSettings::appConfig.scenes.Size(); i++ ) {
			// scene uid
			const Char* uid = ProjectSettings::appConfig.scenes[i].GetChar();
			// get scene
			Scene* scene = AssetManager::Singleton()->GetScene( uid );
			// get scene path
			FiniteString srcPath( "%s%s%s", scene->GetDirPath(), scene->GetName(),
											FileManager::FILE_FORMAT_SCENE );
			// get dest path
			FiniteString dstPath( "%s%s%s", ProjectSettings::dirPathBuildWindows.GetChar(), 
											AppConfig::DIR_PATH_DATA,
											ProjectSettings::appConfig.scenes[i].GetChar() );
			// copy file
			fileManager->CopyFile( srcPath.GetChar(), dstPath.GetChar() );
		}
	}

}


void ProjectBuilderImpl::BuildAndroid( const Char* buildPath )
{
	ASSERT( _isInitialized );
}
