// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Prefab.h"
#include "../../../Asset/Assets/Prefab.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"
#include "../../../../2_BuildingBlocks/GameObject.h"
#include "JsonSerializer_GameObject.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;


rapidjson::Value JsonSerializer_Prefab::ToJson( const void* object )
{
	const Prefab* prefab = CAST_S( const Prefab*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( UNIQUE_ID ),
					rapidjson::StringRef( prefab->GetUniqueID() ),
					_al );

	JsonSerializer_GameObject serializer;
	json.AddMember( rapidjson::StringRef( GAME_OBJECT ), 
					serializer.ToJson( prefab->GetGameObject() ), 
					_al );

	return json;
}


void JsonSerializer_Prefab::FromJson( rapidjson::Value& json, OUT void* object, DeserFlag flag )
{
	Prefab* prefab = CAST_S( Prefab*, object );

	Bool emitEvents = prefab->DoesEmitEvents();
	prefab->SetEmitEvents( FALSE );

	if ( json.HasMember( UNIQUE_ID ) ) {
		prefab->SetUniqueID( json[UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY ) {
		return;
	}

	if ( json.HasMember( GAME_OBJECT ) ) {
		GameObject* gameObject = Memory::Alloc<GameObject>( NULL, -1 );
		prefab->SetGameObject( gameObject );

		JsonSerializer_GameObject serializer;
		serializer.FromJson( json[GAME_OBJECT], gameObject, DeserFlag::FULL );
	}

	prefab->SetEmitEvents( emitEvents );
}
