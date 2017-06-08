// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Transform.h"
#include "../../../../2_BuildingBlocks/Components/Transform.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Transform::ToJson( const void* object )
{
	const Transform* transform = CAST_S( const Transform*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ATTR_ENABLED ),	
					transform->IsEnabled(),	
					_al );
	
	json.AddMember( rapidjson::StringRef( ATTR_POSITION_WORLD ), 
					_ToJsonVec3( transform->GetPositionWorld() ), 
					_al );
	json.AddMember( rapidjson::StringRef( ATTR_ROTATIO_WORLD ), 
					_ToJsonVec3( transform->GetEulerRotationWorld() ), 
					_al );
	json.AddMember( rapidjson::StringRef( ATTR_SCALE_WORLD ), 
					_ToJsonVec3( transform->GetScaleWorld() ), 
					_al );

	return json;
}


void JsonSerializer_Transform::FromJson( rapidjson::Value& json, OUT void* object, 
										 DeserFlag flag )
{
	Transform* transform = CAST_S( Transform*, object );

	bool emitEvents = transform->DoesEmitEvents();
	transform->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_ENABLED ) ) {
		transform->SetEnabled( json[ATTR_ENABLED].GetBool() );
	}

	if ( json.HasMember( ATTR_POSITION_WORLD ) ) {
		transform->SetPositionWorld( _FromJsonVec3( json[ATTR_POSITION_WORLD] ) );
	}

	if ( json.HasMember( ATTR_ROTATIO_WORLD ) ) {
		transform->SetEulerRotationWorld( _FromJsonVec3( json[ATTR_ROTATIO_WORLD] ) );
	}

	if ( json.HasMember( ATTR_SCALE_WORLD ) ) {
		transform->SetScaleWorld( _FromJsonVec3( json[ATTR_SCALE_WORLD] ) );
	}


	transform->SetEmitEvents( emitEvents );
}
