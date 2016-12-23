// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Scripter.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"
#include "../../../Asset/AssetManager.h"
#include "../../../../2_BuildingBlocks/Random/Random.h"
#include "../../../Script/Assets/Script.h"
#include "../../../Script/Components/Scripter.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Scripter::ToJson( void* object )
{
	COMP::Scripter* scripter = CAST_S( COMP::Scripter*, object );

	rapidjson::Value json;
	json.SetObject();

	// add enabled
	json.AddMember( rapidjson::StringRef( ENABLED ),	
					scripter->IsEnabled(),	
					_al );
	
	// add script
	{
		Script* script = scripter->GetScript();
		if ( script == NULL ) {
			// add null value
			rapidjson::Value nullNode;
			nullNode.SetNull();
			json.AddMember( rapidjson::StringRef( SCRIPT ), nullNode, _al );
		}
		else {
			// add script uid
			json.AddMember( rapidjson::StringRef( SCRIPT ),
							rapidjson::StringRef( script->GetUniqueID() ),
							_al );
		}
	}

	return json;
}


void JsonSerializer_Scripter::FromJson( rapidjson::Value& json, OUT void* object, 
										 DeserFlag flag )
{
	COMP::Scripter* scripter = CAST_S( COMP::Scripter*, object );

	Bool emitEvents = scripter->DoesEmitEvents();
	scripter->SetEmitEvents( FALSE );

	// load script
	if ( json.HasMember( SCRIPT ) )
	{
		if ( json[SCRIPT].IsNull() )
		{
			scripter->SetScript( NULL );
		}
		else
		{
			const Char* uniqueID = json[SCRIPT].GetString();
			Script* script = AssetManager::Singleton()->GetScript( uniqueID );
			if ( script == NULL ) {
				Bool isOk = Random::ValidateUniqueID( uniqueID );
				if ( isOk )	{
					script = Memory::Alloc<Script>();
					script->SetUniqueID( uniqueID );
					AssetManager::Singleton()->AddScript( script );
				}
			}
			scripter->SetScript( script );
		}
	}

	scripter->SetEmitEvents( emitEvents );
}
