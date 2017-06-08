// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Light.h"
#include "../../../Render/Components/Light.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;


rapidjson::Value JsonSerializer_Light::ToJson( const void* object )
{
	const Light* light = CAST_S( const Light*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ATTR_ENABLED ),	
					light->IsEnabled(),	
					_al );

	switch ( light->GetLightType() ) {
		case LightType::DIRECTIONAL:
			json.AddMember( rapidjson::StringRef( ATTR_LIGHT_TYPE ),
							rapidjson::StringRef( Light::TYPE_DIRECTIONAL ),
							_al );
			break;

		case LightType::POINT:
			json.AddMember( rapidjson::StringRef( ATTR_LIGHT_TYPE ),
							rapidjson::StringRef( Light::TYPE_POINT ),
							_al );
			json.AddMember( rapidjson::StringRef( ATTR_RANGE ), 
							light->GetRange(), 
							_al );
			break;

		case LightType::SPOT:
			json.AddMember( rapidjson::StringRef( ATTR_LIGHT_TYPE ),
							rapidjson::StringRef( Light::TYPE_SPOT ),
							_al );
			json.AddMember( rapidjson::StringRef( ATTR_RANGE ), 
							light->GetRange(), 
							_al );
			json.AddMember( rapidjson::StringRef( ATTR_SPOT_ANGLE ), 
							light->GetSpotAngle(), 
							_al );
			break;
	}
	json.AddMember( rapidjson::StringRef( ATTR_COLOR ), 
					_ToJsonVec3( light->GetColor() ),
					_al );
	json.AddMember( rapidjson::StringRef( ATTR_INTENSITY ), 
					light->GetIntensity(), 
					_al );

	return json;
}


void JsonSerializer_Light::FromJson( rapidjson::Value& json, OUT void* object, DeserFlag flag )
{
	Light* light = CAST_S( Light*, object );

	bool emitEvents = light->DoesEmitEvents();
	light->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_ENABLED ) ) {
		light->SetEnabled( json[ATTR_ENABLED].GetBool() );
	}

	if ( json.HasMember( ATTR_LIGHT_TYPE ) ) {
		FiniteString type( json[ATTR_LIGHT_TYPE].GetString() );

		if ( type == Light::TYPE_DIRECTIONAL ) {
			light->SetLightType( LightType::DIRECTIONAL );
		}
		else if ( type == Light::TYPE_POINT ) {
			light->SetLightType( LightType::POINT );
		}
		else if ( type == Light::TYPE_SPOT ) {
			light->SetLightType( LightType::SPOT );
		}
	}

	switch ( light->GetLightType() ) {
		case LightType::DIRECTIONAL:
			break;

		case LightType::POINT:
			if ( json.HasMember( ATTR_RANGE ) )	{
				light->SetRange( CAST_S( float, json[ATTR_RANGE].GetDouble() ) );
			}
			break;

		case LightType::SPOT:
			if ( json.HasMember( ATTR_RANGE ) ) {
				light->SetRange( CAST_S( float, json[ATTR_RANGE].GetDouble() ) );
			}
			if ( json.HasMember( ATTR_SPOT_ANGLE ) ) {
				light->SetSpotAngle( CAST_S( float, json[ATTR_SPOT_ANGLE].GetDouble() ) );
			}
			break;
	}

	if ( json.HasMember( ATTR_COLOR ) ) {
		light->SetColor(_FromJsonVec3( json[ATTR_COLOR] ) );
	}

	if ( json.HasMember( ATTR_INTENSITY ) ) {
		light->SetIntensity( CAST_S( float, json[ATTR_INTENSITY].GetDouble() ) );
	}


	light->SetEmitEvents( emitEvents );
}
