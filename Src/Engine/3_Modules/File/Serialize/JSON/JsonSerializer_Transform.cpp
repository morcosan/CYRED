// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Transform.h"
#include "../../../../2_BuildingBlocks/Components/Transform.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Transform::ToJson( void* object )
{
	COMP::Transform* transform = CAST_S( COMP::Transform*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ENABLED ),	
					transform->IsEnabled(),	
					_al );
	
	json.AddMember( rapidjson::StringRef( POSITION_WORLD ), 
					_ToJsonVec3( transform->GetPositionWorld() ), 
					_al );
	json.AddMember( rapidjson::StringRef( ROTATIO_WORLD ), 
					_ToJsonVec3( transform->GetEulerRotationWorld() ), 
					_al );
	json.AddMember( rapidjson::StringRef( SCALE_WORLD ), 
					_ToJsonVec3( transform->GetScaleWorld() ), 
					_al );

	return json;
}


void JsonSerializer_Transform::FromJson( rapidjson::Value& json, OUT void* object, 
										 DeserFlag flag )
{
	COMP::Transform* transform = CAST_S( COMP::Transform*, object );

	Bool emitEvents = transform->DoesEmitEvents();
	transform->SetEmitEvents( FALSE );

	if ( json.HasMember( ENABLED ) )
	{
		transform->SetEnabled( json[ENABLED].GetBool() );
	}
	if ( json.HasMember( POSITION_WORLD ) )
	{
		transform->SetPositionWorld( _FromJsonVec3( json[POSITION_WORLD] ) );
	}
	if ( json.HasMember( ROTATIO_WORLD ) )
	{
		transform->SetEulerRotationWorld( _FromJsonVec3( json[ROTATIO_WORLD] ) );
	}
	if ( json.HasMember( SCALE_WORLD ) )
	{
		transform->SetScaleWorld( _FromJsonVec3( json[SCALE_WORLD] ) );
	}

	transform->SetEmitEvents( emitEvents );
}
