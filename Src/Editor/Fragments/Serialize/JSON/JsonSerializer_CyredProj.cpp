// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_CyredProj.h"
#include "../../Settings/ProjectSettings.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Scene.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_CyredProj::ToJson( void* object )
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
	{
		rapidjson::Value arrayNode;
		arrayNode.SetArray();

		for ( UInt i = 0; i < ProjectSettings::scenes.Size(); ++i )
		{
			Scene* scene = ProjectSettings::scenes[i];
			if ( scene != NULL )
			{
				arrayNode.PushBack( rapidjson::StringRef( scene->GetUniqueID() ), _al );
			}
			else
			{
				rapidjson::Value nullNode;
				nullNode.SetNull();

				arrayNode.PushBack( nullNode, _al );
			}
		}

		json.AddMember( rapidjson::StringRef( SCENES ), arrayNode, _al );
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

	if ( json.HasMember( APP_CONFIG ) )
	{
		rapidjson::Value& config = json[APP_CONFIG];

		if ( config.HasMember( APP_NAME ) )
		{
			ProjectSettings::appConfig.appName = config[APP_NAME].GetString();
		}
		if ( config.HasMember( FULLSCREEN ) )
		{
			ProjectSettings::appConfig.fullscreen = config[FULLSCREEN].GetBool();
		}
		if ( !ProjectSettings::appConfig.fullscreen )
		{
			if ( config.HasMember( WIDTH ) )
			{
				ProjectSettings::appConfig.width = config[WIDTH].GetInt();
			}
			if ( config.HasMember( HEIGHT ) )
			{
				ProjectSettings::appConfig.height = config[HEIGHT].GetInt();
			}
			if ( config.HasMember( POS_X ) )
			{
				ProjectSettings::appConfig.posX = config[POS_X].GetInt();
			}
			if ( config.HasMember( POS_Y ) )
			{
				ProjectSettings::appConfig.posY = config[POS_Y].GetInt();
			}
		}
		if ( config.HasMember( FPS ) )
		{
			ProjectSettings::appConfig.fps = config[FPS].GetInt();
		}
	}
	if ( json.HasMember( SCENES ) )
	{
		rapidjson::Value& sceneJson = json[SCENES];

		for ( UInt i = 0; i < sceneJson.Size(); ++i )
		{
			Scene* scene = NULL;

			if ( !sceneJson[i].IsNull() )
			{
				scene = AssetManager::Singleton()->GetScene( sceneJson[i].GetString() );
			}

			ProjectSettings::scenes.Add( scene );

			// add scene to app config
			ProjectSettings::appConfig.scenes.Add( scene->GetName() );
		}
	}
	if ( json.HasMember( BUILD_WINDOWS ) )
	{
		ProjectSettings::dirPathBuildWindows = json[BUILD_WINDOWS].GetString();
	}
	if ( json.HasMember( BUILD_ANDROID ) )
	{
		ProjectSettings::dirPathBuildAndroid = json[BUILD_ANDROID].GetString();
	}
}
