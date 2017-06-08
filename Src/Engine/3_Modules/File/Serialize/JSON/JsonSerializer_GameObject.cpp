// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
#include "../../../Render/Components/Text3D.h"
#include "../../../Script/Components/Scripter.h"
#include "../../../Physics/Components/RigidBody.h"
#include "JsonSerializer_Transform.h"
#include "JsonSerializer_Camera.h"
#include "JsonSerializer_Light.h"
#include "JsonSerializer_ParticleEmitter.h"
#include "JsonSerializer_MeshRendering.h"
#include "JsonSerializer_MorphRendering.h"
#include "JsonSerializer_Scripter.h"
#include "JsonSerializer_RigidBody.h"
#include "JsonSerializer_Text3D.h"
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
		json.AddMember( rapidjson::StringRef( ATTR_NAME ),		
						rapidjson::StringRef( gameObject->GetName() ),
						_al );
		json.AddMember( rapidjson::StringRef( ATTR_TAG ),		
						rapidjson::StringRef( gameObject->GetTag() ),
						_al );
		json.AddMember( rapidjson::StringRef( ATTR_ENABLED ),	
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
				objectNode.AddMember( rapidjson::StringRef( COMP_TRANSFORM ), 
									  serializer.ToJson( comp ), 
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::CAMERA ) {
				JsonSerializer_Camera serializer;
				objectNode.AddMember( rapidjson::StringRef( COMP_CAMERA ), 
									  serializer.ToJson( comp ),
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::LIGHT ) {
				JsonSerializer_Light serializer;
				objectNode.AddMember( rapidjson::StringRef( COMP_LIGHT ), 
									  serializer.ToJson( comp ),
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::PARTICLE_EMITTER ) {
				JsonSerializer_ParticleEmitter serializer;
				objectNode.AddMember( rapidjson::StringRef( COMP_PARTICLE_EMITTER ), 
									  serializer.ToJson( comp ),
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::MESH_RENDERING ) {
				JsonSerializer_MeshRendering serializer;
				objectNode.AddMember( rapidjson::StringRef( COMP_MESH_RENDERING ), 
									  serializer.ToJson( comp ),
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::MORPH_RENDERING ) {
				JsonSerializer_MorphRendering serializer;
				objectNode.AddMember( rapidjson::StringRef( COMP_MORPH_RENDERING ), 
									  serializer.ToJson( comp ),
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::SCRIPTER ) {
				JsonSerializer_Scripter serializer;
				objectNode.AddMember( rapidjson::StringRef( COMP_SCRIPTER ), 
									  serializer.ToJson( comp ),
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::RIGID_BODY ) {
				JsonSerializer_RigidBody serializer;
				objectNode.AddMember( rapidjson::StringRef( COMP_RIGID_BODY ), 
									  serializer.ToJson( comp ),
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::TEXT_3D ) {
				JsonSerializer_Text3D serializer;
				objectNode.AddMember( rapidjson::StringRef( COMP_TEXT_3D ), 
									  serializer.ToJson( comp ),
									  _al );
			}
		}
		json.AddMember( rapidjson::StringRef( ATTR_COMPONENTS ), objectNode, _al );
	}

	// add child nodes
	if ( gameObject->GetChildNodeCount() > 0 ) {
		rapidjson::Value arrayNode;
		arrayNode.SetArray();
		for ( int i = 0; i < gameObject->GetChildNodeCount(); i++ ) {
			GameObject* child = CAST_S( GameObject*, gameObject->GetChildNodeAt( i ) );
			arrayNode.PushBack( this->ToJson( child ), _al );
		}
		json.AddMember( rapidjson::StringRef( ATTR_CHILD_NODES ), arrayNode, _al );
	}
	
	return json;
}


void JsonSerializer_GameObject::FromJson( rapidjson::Value& json, OUT void* object,
										  DeserFlag flag )
{
	GameObject* gameObject = CAST_S( GameObject*, object );

	bool emitEvents = gameObject->DoesEmitEvents();
	gameObject->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_NAME ) ) {
		gameObject->SetName( json[ATTR_NAME].GetString() );
	}

	if ( json.HasMember( ATTR_TAG ) ) {
		gameObject->SetTag( json[ATTR_TAG].GetString() );
	}

	if ( json.HasMember( ATTR_ENABLED ) ) {
		gameObject->SetEnabled( json[ATTR_ENABLED].GetBool() );
	}

	if ( json.HasMember( ATTR_COMPONENTS ) ) {
		rapidjson::Value& components = json[ATTR_COMPONENTS];

		for ( auto iter = components.MemberBegin(); iter != components.MemberEnd(); ++iter ) {
			FiniteString comp( iter->name.GetString() );

			if ( comp == COMP_TRANSFORM ) {
				Transform* transform = gameObject->AddComponent<Transform>();

				JsonSerializer_Transform serializer;
				serializer.FromJson( iter->value, transform, DeserFlag::FULL );
			}
			else if ( comp == COMP_CAMERA ) {
				Camera* camera = gameObject->AddComponent<Camera>();

				JsonSerializer_Camera serializer;
				serializer.FromJson( iter->value, camera, DeserFlag::FULL );
			}
			else if ( comp == COMP_LIGHT ) {
				Light* light = gameObject->AddComponent<Light>();

				JsonSerializer_Light serializer;
				serializer.FromJson( iter->value, light, DeserFlag::FULL );
			}
			else if ( comp == COMP_PARTICLE_EMITTER ) {
				ParticleEmitter* emitter = gameObject->AddComponent<ParticleEmitter>();

				JsonSerializer_ParticleEmitter serializer;
				serializer.FromJson( iter->value, emitter, DeserFlag::FULL );
			}
			else if ( comp == COMP_MESH_RENDERING ) {
				MeshRendering* meshRender = gameObject->AddComponent<MeshRendering>();

				JsonSerializer_MeshRendering serializer;
				serializer.FromJson( iter->value, meshRender, DeserFlag::FULL );
			}
			else if ( comp == COMP_MORPH_RENDERING ) {
				MorphRendering* morphRender = gameObject->AddComponent<MorphRendering>();

				JsonSerializer_MorphRendering serializer;
				serializer.FromJson( iter->value, morphRender, DeserFlag::FULL );
			}
			else if ( comp == COMP_SCRIPTER ) {
				Scripter* scripter = gameObject->AddComponent<Scripter>();

				JsonSerializer_Scripter serializer;
				serializer.FromJson( iter->value, scripter, DeserFlag::FULL );
			}
			else if ( comp == COMP_RIGID_BODY ) {
				RigidBody* rigidBody = gameObject->AddComponent<RigidBody>();

				JsonSerializer_RigidBody serializer;
				serializer.FromJson( iter->value, rigidBody, DeserFlag::FULL );
			}
			else if ( comp == COMP_TEXT_3D ) {
				Text3D* text3D = gameObject->AddComponent<Text3D>();

				JsonSerializer_Text3D serializer;
				serializer.FromJson( iter->value, text3D, DeserFlag::FULL );
			}
		}
	}

	if ( json.HasMember( ATTR_CHILD_NODES ) ) {
		rapidjson::Value& childNodes = json[ATTR_CHILD_NODES];

		for ( int i = 0; i < CAST_S(int, childNodes.Size()); i++ ) {
			int uid = SceneManager::Singleton()->NextGameObjectUID();
			GameObject* childObject = new GameObject( NULL, uid );
			childObject->SetInScene( gameObject->IsInScene() );
			childObject->SetEmitEvents( FALSE );
			{
				// add to parent
				gameObject->AddChildNode( childObject );
				// extract data
				this->FromJson( childNodes[i], childObject, DeserFlag::FULL );
			}
			childObject->SetEmitEvents( TRUE );
		}
	}


	gameObject->SetEmitEvents( emitEvents );
}
