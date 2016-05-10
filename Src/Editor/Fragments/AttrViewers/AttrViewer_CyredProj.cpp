// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_CyredProj.h"
#include "CyredModule_File.h"
#include "CyredModule_Scene.h"

#include "../Settings/ProjectSettings.h"
#include "../Settings/EditorSettings.h"
#include "../Selectors/Selector_Scene.h"


using namespace CYRED;



void AttrViewer_CyredProj::_OnInitialize()
{
	DataArray<AttrStruct> propertiesScheme;
	propertiesScheme.Add( AttrStruct{ ATTR_APP_NAME,	AttrType::STRING } );
	propertiesScheme.Add( AttrStruct{ ATTR_FULLSCREEN,	AttrType::BOOL } );
	propertiesScheme.Add( AttrStruct{ ATTR_WIDTH,		AttrType::INT } );
	propertiesScheme.Add( AttrStruct{ ATTR_HEIGHT,		AttrType::INT } );
	propertiesScheme.Add( AttrStruct{ ATTR_POS_X,		AttrType::INT } );
	propertiesScheme.Add( AttrStruct{ ATTR_POS_Y,		AttrType::INT } );
	propertiesScheme.Add( AttrStruct{ ATTR_FPS,			AttrType::INT } );

	_CreateAttrStruct( ATTR_APP_CONFIG, propertiesScheme );
	_CreateAttrListSelector( ATTR_SCENES, Selector_Scene::TYPE );
	_CreateAttrString( ATTR_BUILD_DIR );

	_AddToPanel( TITLE );
}


void AttrViewer_CyredProj::_OnChangeTarget( void* target )
{
}


void AttrViewer_CyredProj::_OnUpdateGUI()
{
	AppConfig& appConfig = ProjectSettings::appConfig;

	DataUnion data;
	_WriteAttrStruct( ATTR_APP_CONFIG, ATTR_APP_NAME,	data.SetString( appConfig.appName.GetChar() ) );
	_WriteAttrStruct( ATTR_APP_CONFIG, ATTR_FULLSCREEN, data.SetBool( appConfig.fullscreen ) );
	_WriteAttrStruct( ATTR_APP_CONFIG, ATTR_WIDTH,		data.SetInt( appConfig.width ) );
	_WriteAttrStruct( ATTR_APP_CONFIG, ATTR_HEIGHT,		data.SetInt( appConfig.height ) );
	_WriteAttrStruct( ATTR_APP_CONFIG, ATTR_POS_X,		data.SetInt( appConfig.posX ) );
	_WriteAttrStruct( ATTR_APP_CONFIG, ATTR_POS_Y,		data.SetInt( appConfig.posY ) );
	_WriteAttrStruct( ATTR_APP_CONFIG, ATTR_FPS,		data.SetInt( appConfig.fps ) );
	
	_WriteAttrListSize( ATTR_SCENES, ProjectSettings::scenes.Size() );
	for ( UInt i = 0; i < ProjectSettings::scenes.Size(); ++i )
	{
		Scene* scene = ProjectSettings::scenes[i];

		if ( scene != NULL )
		{
			_WriteAttrListIndex( ATTR_SCENES, i, data.SetReference( scene ), scene->GetName() );
		}
		else
		{
			_WriteAttrListIndex( ATTR_SCENES, i, data.SetReference( NULL ), Selector_Scene::OPTION_NULL );
		}
	}

	_WriteAttrString( ATTR_BUILD_DIR, ProjectSettings::dirPathBuild.GetChar() );
}


void AttrViewer_CyredProj::_OnUpdateTarget()
{
	AppConfig& appConfig = ProjectSettings::appConfig;

	appConfig.appName		= _ReadAttrStruct( ATTR_APP_CONFIG, ATTR_APP_NAME ).GetString();
	appConfig.fullscreen	= _ReadAttrStruct( ATTR_APP_CONFIG, ATTR_FULLSCREEN ).GetBool();
	appConfig.width			= _ReadAttrStruct( ATTR_APP_CONFIG, ATTR_WIDTH ).GetInt();
	appConfig.height		= _ReadAttrStruct( ATTR_APP_CONFIG, ATTR_HEIGHT ).GetInt();
	appConfig.posX			= _ReadAttrStruct( ATTR_APP_CONFIG, ATTR_POS_X ).GetInt();
	appConfig.posY			= _ReadAttrStruct( ATTR_APP_CONFIG, ATTR_POS_Y ).GetInt();
	appConfig.fps			= _ReadAttrStruct( ATTR_APP_CONFIG, ATTR_FPS ).GetInt();
	{
		UInt total = _ReadAttrListSize( ATTR_SCENES );

		ProjectSettings::scenes.Clear();

		for ( UInt i = 0; i < total; ++i )
		{
			Scene* scene = CAST_S( Scene*, _ReadAttrListIndex( ATTR_SCENES, i ).GetReference() );
			ProjectSettings::scenes.Add( scene );
		}
	}
	ProjectSettings::dirPathBuild = _ReadAttrString( ATTR_BUILD_DIR );

	_WriteToFile();
}


void AttrViewer_CyredProj::_WriteToFile()
{
	String& data = FileManager::Singleton()->Serialize<ProjectSettings>( NULL );

	FileManager::Singleton()->WriteFile( EditorSettings::projectPath.GetChar(), 
										 data.GetChar() );
}

