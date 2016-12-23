// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Script.h"
#include "../../../Script/Assets/Script.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Script::ToJson( void* object )
{
	Script* script = CAST_S( Script*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( UNIQUE_ID ),
					rapidjson::StringRef( script->GetUniqueID() ),
					_al );
	
	/*const Char* vertexPath = NULL;
	const Char* geometryPath = NULL;
	const Char* fragmentPath = NULL;
	shader->GetShaderFiles( RENDERER_FORWARD, &vertexPath, &geometryPath, &fragmentPath );

	json.AddMember( rapidjson::StringRef( VERTEX_FILE_PATH ),
					( vertexPath != NULL ) ? rapidjson::StringRef( vertexPath ) : "",
					_al );
	json.AddMember( rapidjson::StringRef( GEOMETRY_FILE_PATH ),
					( geometryPath != NULL ) ? rapidjson::StringRef( geometryPath ) : "",
					_al );
	json.AddMember( rapidjson::StringRef( FRAGMENT_FILE_PATH ),
					( fragmentPath != NULL ) ? rapidjson::StringRef( fragmentPath ) : "",
					_al );*/
	
	return json;
}


void JsonSerializer_Script::FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )
{
	Script* script = CAST_S( Script*, object );

	Bool emitEvents = script->DoesEmitEvents();
	script->SetEmitEvents( FALSE );

	if ( json.HasMember( UNIQUE_ID ) )
	{
		script->SetUniqueID( json[UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY )
	{
		return;
	}


	script->SetEmitEvents( emitEvents );
}
