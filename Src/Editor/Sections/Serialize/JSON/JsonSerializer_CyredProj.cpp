// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_CyredProj.h"
#include "../../Settings/ProjectSettings.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Scene.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_CyredProj::ToJson( const void* object )
{
	ASSERT( object == NULL );

	rapidjson::Value json;
	json.SetObject();

	{
		rapidjson::Value objectNode;
		objectNode.SetObject();

		AppConfig& appConfig = ProjectSettings::appConfig;

		objectNode.AddMember( rapidjson::StringRef( APP_NAME ),
							  rapidjson::StringRef( appConfig.appName.GetChar() ),
							  _al );
		objectNode.AddMember( rapidjson::StringRef( FULLSCREEN ),
							  appConfig.fullscreen,
							  _al );

		if ( !appConfig.fullscreen )
		{
			objectNode.AddMember( rapidjson::StringRef( WIDTH ),
								  appConfig.width,
								  _al );
			objectNode.AddMember( rapidjson::StringRef( HEIGHT ),
								  appConfig.height,
								  _al );
			objectNode.AddMember( rapidjson::StringRef( POS_X ),
								  appConfig.posX,
								  _al );
			objectNode.AddMember( rapidjson::StringRef( POS_Y ),
								  appConfig.posY,
								  _al );
			objectNode.AddMember( rapidjson::StringRef( FPS ),
								  appConfig.fps,
								  _al );
		}

		json.AddMember( rapidjson::StringRef( APP_CONFIG ),	objectNode,	_al );
	}
	// add start scene
	if ( ProjectSettings::startScene != NULL ) {
		json.AddMember( rapidjson::StringRef( START_SCENE ), 
						rapidjson::StringRef( ProjectSettings::startScene->GetUniqueID() ), 
						_al );
	}
	else {
		json.AddMember( rapidjson::StringRef( START_SCENE ), 
						"", 
						_al );
	}
	json.AddMember( rapidjson::StringRef( BUILD_WINDOWS ), 
					rapidjson::StringRef( ProjectSettings::dirPathBuildWindows.GetChar() ), 
					_al );

	json.AddMember( rapidjson::StringRef( BUILD_ANDROID ), 
					rapidjson::StringRef( ProjectSettings::dirPathBuildAndroid.GetChar() ), 
					_al );

	return json;
}


void JsonSerializer_CyredProj::FromJson( rapidjson::Value& json, OUT void* object, 
										 DeserFlag flag )
{
	ASSERT( object == NULL );

	if ( json.HasMember( APP_CONFIG ) ) {
		rapidjson::Value& config = json[APP_CONFIG];

		if ( config.HasMember( APP_NAME ) ) {
			ProjectSettings::appConfig.appName = config[APP_NAME].GetString();
		}
		if ( config.HasMember( FULLSCREEN ) ) {
			ProjectSettings::appConfig.fullscreen = config[FULLSCREEN].GetBool();
		}
		if ( !ProjectSettings::appConfig.fullscreen ) {
			if ( config.HasMember( WIDTH ) ) {
				ProjectSettings::appConfig.width = config[WIDTH].GetInt();
			}
			if ( config.HasMember( HEIGHT ) ) {
				ProjectSettings::appConfig.height = config[HEIGHT].GetInt();
			}
			if ( config.HasMember( POS_X ) ) {
				ProjectSettings::appConfig.posX = config[POS_X].GetInt();
			}
			if ( config.HasMember( POS_Y ) ) {
				ProjectSettings::appConfig.posY = config[POS_Y].GetInt();
			}
		}
		if ( config.HasMember( FPS ) ) {
			ProjectSettings::appConfig.fps = config[FPS].GetInt();
		}
	}
	if ( json.HasMember( START_SCENE ) ) {
		ProjectSettings::startScene = AssetManager::Singleton()->GetScene( json[START_SCENE].GetString() );

		if ( ProjectSettings::startScene != NULL ) {
			// add scene to app config
			ProjectSettings::appConfig.startScene = AppConfig::AssetConfig {
				ProjectSettings::startScene->GetName(),
				ProjectSettings::startScene->GetUniqueID()
			};
		}
	}
	if ( json.HasMember( BUILD_WINDOWS ) ) {
		ProjectSettings::dirPathBuildWindows = json[BUILD_WINDOWS].GetString();
	}
	if ( json.HasMember( BUILD_ANDROID ) ) {
		ProjectSettings::dirPathBuildAndroid = json[BUILD_ANDROID].GetString();
	}
}
