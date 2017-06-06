// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Scene.h"
#include "../../../Scene/Sections/Scene.h"
#include "../../../../2_BuildingBlocks/GameObject.h"
#include "JsonSerializer_GameObject.h"
#include "../../../Scene/SceneManager.h"

#include "rapidjson\Include\document.h"
#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Scene::ToJson( const void* object )
{
	const Scene* scene = CAST_S( const Scene*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( UNIQUE_ID ),
					rapidjson::StringRef( scene->GetUniqueID() ),
					_al );
	{
		rapidjson::Value arrayNode;
		arrayNode.SetArray();

		JsonSerializer_GameObject serializer;
		Node* sceneRoot = scene->GetRoot();

		for ( int i = 0; i < sceneRoot->GetChildNodeCount(); ++i ) {
			GameObject* gameObject = CAST_S( GameObject*, sceneRoot->GetChildNodeAt( i ) );
			if ( gameObject->GetParentNode() == sceneRoot )	{
				arrayNode.PushBack( serializer.ToJson( gameObject ), _al );
			}
		}

		json.AddMember( rapidjson::StringRef( GAMEOBJECTS ), arrayNode, _al );
	}

	return json;
}


void JsonSerializer_Scene::FromJson( rapidjson::Value& json, OUT void* object,
									 DeserFlag flag )
{
	Scene* scene = CAST_S( Scene*, object );

	bool emitEvents = scene->DoesEmitEvents();
	scene->SetEmitEvents( FALSE );

	if ( json.HasMember( UNIQUE_ID ) ) {
		scene->SetUniqueID( json[UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY ) {
		return;
	}

	if ( json.HasMember( GAMEOBJECTS ) ) {
		rapidjson::Value& gameObjects = json[GAMEOBJECTS];

		for ( int i = 0; i < CAST_S(int, gameObjects.Size()); ++i )	{
			int uid = SceneManager::Singleton()->NextGameObjectUID();
			GameObject* gameObject = new GameObject( NULL, uid );
			gameObject->SetInScene( TRUE );
			gameObject->SetEmitEvents( FALSE );
			{
				// add to scene
				scene->GetRoot()->AddChildNode( gameObject );
				// extract data
				JsonSerializer_GameObject serializer;
				serializer.FromJson( gameObjects[i], gameObject, DeserFlag::FULL );
			}
			gameObject->SetEmitEvents( TRUE );
		}
	}

	scene->SetEmitEvents( emitEvents );
}
