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

	// add uid
	json.AddMember( rapidjson::StringRef( UNIQUE_ID ),
					rapidjson::StringRef( script->GetUniqueID() ),
					_al );

	// add run in editor
	json.AddMember( rapidjson::StringRef( RUN_IN_EDITOR ),
					script->RunsInEditor(),
					_al );

	// add file paths
	{
		// create list
		rapidjson::Value arrayNode;
		arrayNode.SetArray();
		// add to list
		for ( UInt i = 0; i < script->GetPathsCount(); ++i )
		{
			arrayNode.PushBack( rapidjson::StringRef( script->GetFilePath( i ) ), _al );
		}
		// add to json
		json.AddMember( rapidjson::StringRef( FILE_PATHS ),
						arrayNode,
						_al );
	}
	
	return json;
}


void JsonSerializer_Script::FromJson( rapidjson::Value& json, OUT void* object,
									  DeserFlag flag )
{
	Script* script = CAST_S( Script*, object );

	Bool emitEvents = script->DoesEmitEvents();
	script->SetEmitEvents( FALSE );

	if ( json.HasMember( UNIQUE_ID ) ) {
		script->SetUniqueID( json[UNIQUE_ID].GetString() );
	}

	// stop here for uid only
	if ( flag == DeserFlag::UID_ONLY ) {
		return;
	}

	if ( json.HasMember( RUN_IN_EDITOR ) ) {
		script->SetRunInEditor( json[RUN_IN_EDITOR].GetBool() );
	}

	if ( json.HasMember( FILE_PATHS ) ) {
		rapidjson::Value& paths = json[FILE_PATHS];
		for ( UInt i = 0; i < paths.Size(); ++i )
		{
			script->SetFilePath( i, paths[i].GetString() );
		}

		// load script data
		script->LoadLuaFiles();
	}

	script->SetEmitEvents( emitEvents );
}
