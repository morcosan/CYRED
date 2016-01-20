// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_GameConfig.h"
#include "../../AppSettings.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_GameConfig::ToJson( void* object )
{
	ASSERT( object == NULL );

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


void JsonSerializer_GameConfig::FromJson( rapidjson::Value& json, OUT void* object, 
										  DeserFlag flag )
{
	ASSERT( object == NULL );

	if ( json.HasMember( APP_NAME ) )
	{
		AppSettings::appName = json[APP_NAME].GetString();
	}
	if ( json.HasMember( FULLSCREEN ) )
	{
		AppSettings::fullscreen = json[FULLSCREEN].GetBool();
	}
	if ( !AppSettings::fullscreen )
	{
		if ( json.HasMember( WIDTH ) )
		{
			AppSettings::width = json[WIDTH].GetInt();
		}
		if ( json.HasMember( HEIGHT ) )
		{
			AppSettings::height = json[HEIGHT].GetInt();
		}
		if ( json.HasMember( POS_X ) )
		{
			AppSettings::posX = json[POS_X].GetInt();
		}
		if ( json.HasMember( POS_Y ) )
		{
			AppSettings::posY = json[POS_Y].GetInt();
		}
	}
	if ( json.HasMember( FPS ) )
	{
		AppSettings::fps = json[FPS].GetInt();
	}
	if ( json.HasMember( SCENES ) )
	{
		rapidjson::Value& scenes = json[SCENES];

		for ( UInt i = 0; i < scenes.Size(); ++i )
		{
			if ( scenes[i].HasMember( SCENE_NAME ) &&
				 scenes[i].HasMember( SCENE_DIR ) && 
				 scenes[i].HasMember( ASSETDB_DIR ) )
			{
				AppSettings::SceneData sceneData
				{
					scenes[i][SCENE_NAME].GetString(),
					scenes[i][SCENE_DIR].GetString(),
					scenes[i][ASSETDB_DIR].GetString()
				};

				AppSettings::scenesData.Add( sceneData );
			}
		}
	}
}
