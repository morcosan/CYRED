// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Light.h"
#include "../../../Render/Components/Light.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;


rapidjson::Value JsonSerializer_Light::ToJson( void* object )
{
	Light* light = CAST_S( Light*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ENABLED ),	
					light->IsEnabled(),	
					_al );

	switch ( light->GetLightType() ) {
		case LightType::DIRECTIONAL:
			json.AddMember( rapidjson::StringRef( LIGHT_TYPE ),
							rapidjson::StringRef( Light::TYPE_DIRECTIONAL ),
							_al );
			break;

		case LightType::POINT:
			json.AddMember( rapidjson::StringRef( LIGHT_TYPE ),
							rapidjson::StringRef( Light::TYPE_POINT ),
							_al );
			json.AddMember( rapidjson::StringRef( RANGE ), 
							light->GetRange(), 
							_al );
			break;

		case LightType::SPOT:
			json.AddMember( rapidjson::StringRef( LIGHT_TYPE ),
							rapidjson::StringRef( Light::TYPE_SPOT ),
							_al );
			json.AddMember( rapidjson::StringRef( RANGE ), 
							light->GetRange(), 
							_al );
			json.AddMember( rapidjson::StringRef( SPOT_ANGLE ), 
							light->GetSpotAngle(), 
							_al );
			break;
	}
	json.AddMember( rapidjson::StringRef( COLOR ), 
					_ToJsonVec3( light->GetColor() ),
					_al );
	json.AddMember( rapidjson::StringRef( INTENSITY ), 
					light->GetIntensity(), 
					_al );

	return json;
}


void JsonSerializer_Light::FromJson( rapidjson::Value& json, OUT void* object, DeserFlag flag )
{
	Light* light = CAST_S( Light*, object );

	Bool emitEvents = light->DoesEmitEvents();
	light->SetEmitEvents( FALSE );

	if ( json.HasMember( ENABLED ) ) {
		light->SetEnabled( json[ENABLED].GetBool() );
	}
	if ( json.HasMember( LIGHT_TYPE ) )	{
		FiniteString type( json[LIGHT_TYPE].GetString() );

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
			if ( json.HasMember( RANGE ) )
			{
				light->SetRange( CAST_S( Float, json[RANGE].GetDouble() ) );
			}
			break;

		case LightType::SPOT:
			if ( json.HasMember( RANGE ) )
			{
				light->SetRange( CAST_S( Float, json[RANGE].GetDouble() ) );
			}
			if ( json.HasMember( SPOT_ANGLE ) )
			{
				light->SetSpotAngle( CAST_S( Float, json[SPOT_ANGLE].GetDouble() ) );
			}
			break;
	}
	if ( json.HasMember( COLOR ) ) {
		light->SetColor(_FromJsonVec3( json[COLOR] ) );
	}
	if ( json.HasMember( INTENSITY ) ) {
		light->SetIntensity( CAST_S( Float, json[INTENSITY].GetDouble() ) );
	}

	light->SetEmitEvents( emitEvents );
}
