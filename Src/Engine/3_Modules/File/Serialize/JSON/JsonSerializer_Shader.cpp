// Copyright (c) 2015 Morco (www.morco.ro)
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

	json.AddMember( rapidjson::StringRef( UNIQUE_ID ),
					rapidjson::StringRef( shader->GetUniqueID() ),
					_al );
	json.AddMember( rapidjson::StringRef( RENDERER ),
					rapidjson::StringRef( RENDERER_FORWARD ),
					_al );

	const Char* vertexPath = NULL;
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
					_al );
	
	return json;
}


void JsonSerializer_Shader::FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )
{
	Shader* shader = CAST_S( Shader*, object );

	Bool emitEvents = shader->DoesEmitEvents();
	shader->SetEmitEvents( FALSE );

	if ( json.HasMember( UNIQUE_ID ) )
	{
		shader->SetUniqueID( json[UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY )
	{
		return;
	}

	if ( json.HasMember( RENDERER ) &&
		 json.HasMember( VERTEX_FILE_PATH ) &&
		 json.HasMember( GEOMETRY_FILE_PATH ) &&
		 json.HasMember( FRAGMENT_FILE_PATH ) )
	{
		String renderer( json[RENDERER].GetString() );

		if ( renderer == RENDERER_FORWARD )
		{
			shader->SetShaderFiles( RENDERER_FORWARD,
									json[VERTEX_FILE_PATH].GetString(),
									json[GEOMETRY_FILE_PATH].GetString(),
									json[FRAGMENT_FILE_PATH].GetString() );
			shader->ChangeRenderer( RENDERER_FORWARD );
		}
	}

	shader->SetEmitEvents( emitEvents );
}
