// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_AppConfig.h"
#include "../../../../4_Application/AppConfig.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_AppConfig::ToJson( void* object )
{
	AppConfig* appConfig = CAST_S( AppConfig*, object );

	rapidjson::Value json;
	json.SetObject();

	//json.AddMember( rapidjson::StringRef( ENABLED ),	
	//				transform->IsEnabled(),	
	//				_al );
	//
	//json.AddMember( rapidjson::StringRef( POSITION_WORLD ), 
	//				_ToJsonVec3( transform->GetPositionWorld() ), 
	//				_al );
	//json.AddMember( rapidjson::StringRef( ROTATIO_WORLD ), 
	//				_ToJsonVec3( transform->GetEulerRotationWorld() ), 
	//				_al );
	//json.AddMember( rapidjson::StringRef( SCALE_WORLD ), 
	//				_ToJsonVec3( transform->GetScaleWorld() ), 
	//				_al );

	return json;
}


void JsonSerializer_AppConfig::FromJson( rapidjson::Value& json, OUT void* object, 
										 DeserFlag flag )
{
	AppConfig* appConfig = CAST_S( AppConfig*, object );

	if ( json.HasMember( APP_NAME ) )
	{
		appConfig->appName = json[APP_NAME].GetString();
	}
	if ( json.HasMember( FULLSCREEN ) )
	{
		appConfig->fullscreen = json[FULLSCREEN].GetBool();
	}
	if ( !appConfig->fullscreen )
	{
		if ( json.HasMember( WIDTH ) )
		{
			appConfig->width = json[WIDTH].GetInt();
		}
		if ( json.HasMember( HEIGHT ) )
		{
			appConfig->height = json[HEIGHT].GetInt();
		}
		if ( json.HasMember( POS_X ) )
		{
			appConfig->posX = json[POS_X].GetInt();
		}
		if ( json.HasMember( POS_Y ) )
		{
			appConfig->posY = json[POS_Y].GetInt();
		}
	}
	if ( json.HasMember( FPS ) )
	{
		appConfig->fps = json[FPS].GetInt();
	}
	if ( json.HasMember( SCENES ) )
	{
		rapidjson::Value& scenes = json[SCENES];

		for ( UInt i = 0; i < scenes.Size(); ++i )
		{
			if ( scenes[i].HasMember( SCENE_NAME ) &&
				 scenes[i].HasMember( SCENE_DIR ) )
			{
				AssetDB assetDB
				{
					scenes[i][SCENE_NAME].GetString(),
					scenes[i][SCENE_DIR].GetString()
				};

				appConfig->assetDB.Add( assetDB );
			}
		}
	}
}
