// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_RigidBody.h"
#include "../../../Physics/Components/RigidBody.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_RigidBody::ToJson( const void* object )
{
	const RigidBody* rigidBody = CAST_S( const RigidBody*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ATTR_ENABLED ),	rigidBody->IsEnabled(), _al );
	json.AddMember( rapidjson::StringRef( ATTR_IS_TRIGGER ), rigidBody->IsTrigger(), _al );

	switch ( rigidBody->GetShapeType() ) {
		case CollisionShapeType::BOX:
			json.AddMember( rapidjson::StringRef( ATTR_SHAPE_TYPE ),	rapidjson::StringRef( TYPE_BOX ), _al );
			break;

		case CollisionShapeType::SPHERE:
			json.AddMember( rapidjson::StringRef( ATTR_SHAPE_TYPE ),	rapidjson::StringRef( TYPE_SPHERE ), _al );
			break;
	}
	
	json.AddMember( rapidjson::StringRef( ATTR_SHAPE_SIZE ), _ToJsonVec3( rigidBody->GetShapeSize() ), _al );
	json.AddMember( rapidjson::StringRef( ATTR_MASS ), rigidBody->GetMass(), _al );

	return json;
}


void JsonSerializer_RigidBody::FromJson( rapidjson::Value& json, OUT void* object, DeserFlag flag )
{
	RigidBody* rigidBody = CAST_S( RigidBody*, object );

	bool emitEvents = rigidBody->DoesEmitEvents();
	rigidBody->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_IS_TRIGGER ) ) {
		rigidBody->SetIsTrigger( json[ATTR_IS_TRIGGER].GetBool() );
	}

	if ( json.HasMember( ATTR_SHAPE_TYPE ) ) {
		FiniteString type( json[ATTR_SHAPE_TYPE].GetString() );

		if ( type == TYPE_BOX ) {
			rigidBody->SetShapeType( CollisionShapeType::BOX );
		}
		else if ( type == TYPE_SPHERE ) {
			rigidBody->SetShapeType( CollisionShapeType::SPHERE );
		}
	}

	if ( json.HasMember( ATTR_SHAPE_SIZE ) ) {
		rigidBody->SetShapeSize( _FromJsonVec3( json[ATTR_SHAPE_SIZE] ) );
	}

	if ( json.HasMember( ATTR_MASS ) ) {
		rigidBody->SetMass( CAST_S( float, json[ATTR_MASS].GetDouble() ) );
	}


	rigidBody->SetEmitEvents( emitEvents );
}
