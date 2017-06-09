// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Prefab.h"
#include "../../../Asset/Assets/Prefab.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"
#include "../../../../2_BuildingBlocks/GameObject.h"
#include "JsonSerializer_GameObject.h"
#include "../../../Scene/SceneManager.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;


rapidjson::Value JsonSerializer_Prefab::ToJson( const void* object )
{
	const Prefab* prefab = CAST_S( const Prefab*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ATTR_UNIQUE_ID ),
					rapidjson::StringRef( prefab->GetUniqueID() ),
					_al );

	JsonSerializer_GameObject serializer;
	GameObject* root = prefab->GetRoot();
	json.AddMember( rapidjson::StringRef( ATTR_ROOT ), serializer.ToJson( root ), _al );

	return json;
}


void JsonSerializer_Prefab::FromJson( rapidjson::Value& json, OUT void* object, DeserFlag flag )
{
	Prefab* prefab = CAST_S( Prefab*, object );

	bool emitEvents = prefab->DoesEmitEvents();
	prefab->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_UNIQUE_ID ) ) {
		prefab->SetUniqueID( json[ATTR_UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY ) {
		return;
	}

	if ( json.HasMember( ATTR_ROOT ) ) {
		rapidjson::Value& gameObjects = json[ATTR_ROOT];

		// create root
		prefab->CreateRoot();
		GameObject* root = prefab->GetRoot();
		root->SetEmitEvents( FALSE );
		{
			// deserialize data
			JsonSerializer_GameObject serializer;
			serializer.FromJson( json[ATTR_ROOT], root, DeserFlag::FULL );
			// set name as prefab
			root->SetName( prefab->GetName() );
		}
		root->SetEmitEvents( TRUE );
	}


	prefab->SetEmitEvents( emitEvents );
}
