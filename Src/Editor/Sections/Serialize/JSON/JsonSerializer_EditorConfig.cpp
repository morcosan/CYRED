// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_EditorConfig.h"
#include "../../Settings/EditorSettings.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_EditorConfig::ToJson( void* object )
{
	ASSERT( object == NULL );

	rapidjson::Value json;
	json.SetObject();

	return json;
}


void JsonSerializer_EditorConfig::FromJson( rapidjson::Value& json, OUT void* object, 
											DeserFlag flag )
{
	ASSERT( object == NULL );
	
	if ( json.HasMember( WIDTH ) ) {
		EditorSettings::width = json[WIDTH].GetInt();
	}
	if ( json.HasMember( HEIGHT ) ) {
		EditorSettings::height = json[HEIGHT].GetInt();
	}
	if ( json.HasMember( POS_X ) ) {
		EditorSettings::posX = json[POS_X].GetInt();
	}
	if ( json.HasMember( POS_Y ) ) {
		EditorSettings::posY = json[POS_Y].GetInt();
	}
	if ( json.HasMember( FPS ) ) {
		EditorSettings::fps = json[FPS].GetInt();
	}
	if ( json.HasMember( PROJECT_NAME ) ) {
		EditorSettings::projectName = json[PROJECT_NAME].GetString();
	}
	if ( json.HasMember( PROJECT_PATH ) ) {
		EditorSettings::projectPath = json[PROJECT_PATH].GetString();
	}
}
