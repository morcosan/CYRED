// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_GameObject.h"
#include "../../../../2_BuildingBlocks/GameObject.h"
#include "../../../../2_BuildingBlocks/Component.h"
#include "../../../../2_BuildingBlocks/Components/Transform.h"
#include "../../../Render/Components/Camera.h"
#include "../../../Render/Components/Light.h"
#include "../../../Render/Components/ParticleEmitter.h"
#include "../../../Render/Components/MeshRendering.h"
#include "../../../Render/Components/MorphRendering.h"
#include "../../../Script/Components/Scripter.h"
#include "JsonSerializer_Transform.h"
#include "JsonSerializer_Camera.h"
#include "JsonSerializer_Light.h"
#include "JsonSerializer_ParticleEmitter.h"
#include "JsonSerializer_MeshRendering.h"
#include "JsonSerializer_MorphRendering.h"
#include "JsonSerializer_Scripter.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"
#include "../../../Scene/SceneManager.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_GameObject::ToJson( const void* object )
{
	const GameObject* gameObject = CAST_S( const GameObject*, object );

	rapidjson::Value json;
	json.SetObject();
	{
		json.AddMember( rapidjson::StringRef( NAME ),		
						rapidjson::StringRef( gameObject->GetName() ),
						_al );
		json.AddMember( rapidjson::StringRef( ENABLED ),	
						gameObject->IsEnabled(),	
						_al );
	}
	{
		rapidjson::Value objectNode;
		objectNode.SetObject();

		for ( int i = 0; i < gameObject->GetComponentCount(); ++i ) {
			Component* comp = gameObject->GetComponentAt( i );

			if ( comp->GetComponentType() == ComponentType::TRANSFORM ) {
				JsonSerializer_Transform serializer;
				objectNode.AddMember( rapidjson::StringRef( TRANSFORM ), 
									  serializer.ToJson( comp ), 
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::CAMERA ) {
				JsonSerializer_Camera serializer;
				objectNode.AddMember( rapidjson::StringRef( CAMERA ), 
									  serializer.ToJson( comp ),
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::LIGHT ) {
				JsonSerializer_Light serializer;
				objectNode.AddMember( rapidjson::StringRef( LIGHT ), 
									  serializer.ToJson( comp ),
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::PARTICLE_EMITTER ) {
				JsonSerializer_ParticleEmitter serializer;
				objectNode.AddMember( rapidjson::StringRef( PARTICLE_EMITTER ), 
									  serializer.ToJson( comp ),
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::MESH_RENDERING ) {
				JsonSerializer_MeshRendering serializer;
				objectNode.AddMember( rapidjson::StringRef( MESH_RENDERING ), 
									  serializer.ToJson( comp ),
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::MORPH_RENDERING ) {
				JsonSerializer_MorphRendering serializer;
				objectNode.AddMember( rapidjson::StringRef( MORPH_RENDERING ), 
									  serializer.ToJson( comp ),
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::SCRIPTER ) {
				JsonSerializer_Scripter serializer;
				objectNode.AddMember( rapidjson::StringRef( SCRIPTER ), 
									  serializer.ToJson( comp ),
									  _al );
			}
		}
		json.AddMember( rapidjson::StringRef( COMPONENTS ), objectNode, _al );
	}

	// add child nodes
	if ( gameObject->GetChildNodeCount() > 0 ) {
		rapidjson::Value arrayNode;
		arrayNode.SetArray();
		for ( int i = 0; i < gameObject->GetChildNodeCount(); i++ ) {
			GameObject* child = CAST_S( GameObject*, gameObject->GetChildNodeAt( i ) );
			arrayNode.PushBack( this->ToJson( child ), _al );
		}
		json.AddMember( rapidjson::StringRef( CHILD_NODES ), arrayNode, _al );
	}
	
	return json;
}


void JsonSerializer_GameObject::FromJson( rapidjson::Value& json, OUT void* object,
										  DeserFlag flag )
{
	GameObject* gameObject = CAST_S( GameObject*, object );

	bool emitEvents = gameObject->DoesEmitEvents();
	gameObject->SetEmitEvents( FALSE );

	if ( json.HasMember( NAME ) ) {
		gameObject->SetName( json[NAME].GetString() );
	}

	if ( json.HasMember( ENABLED ) ) {
		gameObject->SetEnabled( json[ENABLED].GetBool() );
	}

	if ( json.HasMember( COMPONENTS ) ) {
		rapidjson::Value& components = json[COMPONENTS];

		for ( auto iter = components.MemberBegin(); iter != components.MemberEnd(); ++iter ) {
			FiniteString comp( iter->name.GetString() );

			if ( comp == TRANSFORM ) {
				Transform* transform = gameObject->AddComponent<Transform>();

				JsonSerializer_Transform serializer;
				serializer.FromJson( iter->value, transform, DeserFlag::FULL );
			}
			else if ( comp == CAMERA ) {
				Camera* camera = gameObject->AddComponent<Camera>();

				JsonSerializer_Camera serializer;
				serializer.FromJson( iter->value, camera, DeserFlag::FULL );
			}
			else if ( comp == LIGHT ) {
				Light* light = gameObject->AddComponent<Light>();

				JsonSerializer_Light serializer;
				serializer.FromJson( iter->value, light, DeserFlag::FULL );
			}
			else if ( comp == PARTICLE_EMITTER ) {
				ParticleEmitter* emitter = gameObject->AddComponent<ParticleEmitter>();

				JsonSerializer_ParticleEmitter serializer;
				serializer.FromJson( iter->value, emitter, DeserFlag::FULL );
			}
			if ( comp == MESH_RENDERING ) {
				MeshRendering* meshRender = gameObject->AddComponent<MeshRendering>();

				JsonSerializer_MeshRendering serializer;
				serializer.FromJson( iter->value, meshRender, DeserFlag::FULL );
			}
			if ( comp == MORPH_RENDERING ) {
				MorphRendering* morphRender = gameObject->AddComponent<MorphRendering>();

				JsonSerializer_MorphRendering serializer;
				serializer.FromJson( iter->value, morphRender, DeserFlag::FULL );
			}
			if ( comp == SCRIPTER ) {
				Scripter* scripter = gameObject->AddComponent<Scripter>();

				JsonSerializer_Scripter serializer;
				serializer.FromJson( iter->value, scripter, DeserFlag::FULL );
			}
		}
	}

	if ( json.HasMember( CHILD_NODES ) ) {
		rapidjson::Value& childNodes = json[CHILD_NODES];

		for ( int i = 0; i < CAST_S(int, childNodes.Size()); i++ ) {
			int uid = SceneManager::Singleton()->NextGameObjectUID();
			GameObject* childObject = Memory::Alloc<GameObject>( NULL, uid );
			gameObject->AddChildNode( childObject );
			this->FromJson( childNodes[i], childObject, DeserFlag::FULL );
		}
	}

	gameObject->SetEmitEvents( emitEvents );
}
