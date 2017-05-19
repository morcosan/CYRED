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

	json.AddMember( rapidjson::StringRef( UNIQUE_ID ),
					rapidjson::StringRef( prefab->GetUniqueID() ),
					_al );

	{
		rapidjson::Value arrayNode;
		arrayNode.SetArray();

		JsonSerializer_GameObject serializer;
		Node* root = prefab->GetRoot();

		for ( int i = 0; i < root->GetChildNodeCount(); ++i ) {
			GameObject* gameObject = CAST_S( GameObject*, root->GetChildNodeAt( i ) );

			if ( gameObject->GetParentNode() == root )	{
				arrayNode.PushBack( serializer.ToJson( gameObject ), _al );
			}
		}

		json.AddMember( rapidjson::StringRef( GAME_OBJECTS ), arrayNode, _al );
	}


	return json;
}


void JsonSerializer_Prefab::FromJson( rapidjson::Value& json, OUT void* object, DeserFlag flag )
{
	Prefab* prefab = CAST_S( Prefab*, object );

	bool emitEvents = prefab->DoesEmitEvents();
	prefab->SetEmitEvents( FALSE );

	if ( json.HasMember( UNIQUE_ID ) ) {
		prefab->SetUniqueID( json[UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY ) {
		return;
	}

	if ( json.HasMember( GAME_OBJECTS ) ) {
		rapidjson::Value& gameObjects = json[GAME_OBJECTS];

		for ( int i = 0; i < CAST_S(int, gameObjects.Size()); ++i )	{
			int uid = SceneManager::Singleton()->NextGameObjectUID();
			GameObject* gameObject = Memory::Alloc<GameObject>( NULL, uid );
			prefab->GetRoot()->AddChildNode( gameObject );

			JsonSerializer_GameObject serializer;
			serializer.FromJson( gameObjects[i], gameObject, DeserFlag::FULL );
		}
	}

	prefab->SetEmitEvents( emitEvents );
}
