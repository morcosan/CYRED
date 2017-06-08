// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Shader.h"
#include "../../../Render/Assets/Shader.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Shader::ToJson( const void* object )
{
	const Shader* shader = CAST_S( const Shader*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ATTR_UNIQUE_ID ),
					rapidjson::StringRef( shader->GetUniqueID() ),
					_al );

	cchar* vertexPath = NULL;
	cchar* geometryPath = NULL;
	cchar* fragmentPath = NULL;
	shader->GetShaderFiles( &vertexPath, &geometryPath, &fragmentPath );

	json.AddMember( rapidjson::StringRef( ATTR_VERTEX_FILE_PATH ),
					( vertexPath != NULL ) ? rapidjson::StringRef( vertexPath ) : "",
					_al );
	json.AddMember( rapidjson::StringRef( ATTR_GEOMETRY_FILE_PATH ),
					( geometryPath != NULL ) ? rapidjson::StringRef( geometryPath ) : "",
					_al );
	json.AddMember( rapidjson::StringRef( ATTR_FRAGMENT_FILE_PATH ),
					( fragmentPath != NULL ) ? rapidjson::StringRef( fragmentPath ) : "",
					_al );
	
	return json;
}


void JsonSerializer_Shader::FromJson( rapidjson::Value& json, OUT void* object, DeserFlag flag )
{
	Shader* shader = CAST_S( Shader*, object );

	bool emitEvents = shader->DoesEmitEvents();
	shader->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_UNIQUE_ID ) ) {
		shader->SetUniqueID( json[ATTR_UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY ) {
		return;
	}

	if ( json.HasMember( ATTR_VERTEX_FILE_PATH ) && 
		 json.HasMember( ATTR_GEOMETRY_FILE_PATH ) &&
		 json.HasMember( ATTR_FRAGMENT_FILE_PATH ) )
	{
		shader->SetShaderFiles( json[ATTR_VERTEX_FILE_PATH].GetString(),
								json[ATTR_GEOMETRY_FILE_PATH].GetString(),
								json[ATTR_FRAGMENT_FILE_PATH].GetString() );
		shader->LoadShaderProgram();
	}


	shader->SetEmitEvents( emitEvents );
}
