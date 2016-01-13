// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_GameObject.h"
#include "../../../../2_BuildingBlocks/GameObject.h"
#include "../../../../2_BuildingBlocks/Component.h"
#include "../../../../2_BuildingBlocks/Components/Transform.h"
#include "../../../Render/Components/ParticleEmitter.h"
#include "JsonSerializer_Transform.h"
#include "JsonSerializer_ParticleEmitter.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_GameObject::ToJson( void* object )
{
	GameObject* gameObject = CAST_S( GameObject*, object );

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

		for ( UInt i = 0; i < gameObject->GetComponentCount(); ++i )
		{
			COMP::Component* comp = gameObject->GetComponentAt( i );

			if ( comp->GetComponentType() == ComponentType::TRANSFORM )
			{
				JsonSerializer_Transform serializer;
				objectNode.AddMember( rapidjson::StringRef( TRANSFORM ), 
									  serializer.ToJson( comp ), 
									  _al );
			}
			else if ( comp->GetComponentType() == ComponentType::PARTICLE_EMITTER )
			{
				JsonSerializer_ParticleEmitter serializer;
				objectNode.AddMember( rapidjson::StringRef( PARTICLE_EMITTER ), 
									  serializer.ToJson( comp ),
									  _al );
			}
		}
		json.AddMember( rapidjson::StringRef(COMPONENTS), objectNode, _al );
	}

	//{
	//	rapidjson::Value arrayNode;
	//	arrayNode.SetArray();
	//	for ( uint i = 0; i < gameObject->GetChildNodeCount(); ++i )
	//	{
	//		GameObject* child = gameObject->GetChildNodeAt( i );
	//		if ( _useAll || child->IsSavable() )
	//		{
	//			arrayNode.PushBack( _GameObject_to_JSON( child, al ), al );
	//		}
	//	}
	//	json.AddMember( "childNodes", arrayNode, al );
	//}
	
	return json;
}


void JsonSerializer_GameObject::FromJson( rapidjson::Value& json, OUT void* object,
										  DeserFlag flag )
{
	GameObject* gameObject = CAST_S( GameObject*, object );

	Bool emitEvents = gameObject->DoesEmitEvents();
	gameObject->SetEmitEvents( FALSE );

	if ( json.HasMember( NAME ) )
	{
		gameObject->SetName( json[NAME].GetString() );
	}

	if ( json.HasMember( ENABLED ) )
	{
		gameObject->SetEnabled( json[ENABLED].GetBool() );
	}

	if ( json.HasMember( COMPONENTS ) )
	{
		rapidjson::Value& components = json[COMPONENTS];

		for ( auto iter = components.MemberBegin(); iter != components.MemberEnd(); ++iter )
		{
			if ( String( TRANSFORM ) == iter->name.GetString() )
			{
				COMP::Transform* transform = gameObject->AddComponent<COMP::Transform>();

				JsonSerializer_Transform serializer;
				serializer.FromJson( iter->value, transform, DeserFlag::FULL );
			}
			if ( String( PARTICLE_EMITTER ) == iter->name.GetString() )
			{
				COMP::ParticleEmitter* emitter = gameObject->AddComponent<COMP::ParticleEmitter>();

				JsonSerializer_ParticleEmitter serializer;
				serializer.FromJson( iter->value, emitter, DeserFlag::FULL );
			}
		}
	}

	gameObject->SetEmitEvents( emitEvents );
}
