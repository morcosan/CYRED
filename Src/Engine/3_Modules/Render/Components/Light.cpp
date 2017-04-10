// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Light.h"

#include "../../Event/EventManager.h"
#include "../../../2_BuildingBlocks/String/String.h"


using namespace CYRED;


const Char* const Light::TYPE_DIRECTIONAL	= "DIRECTIONAL";
const Char* const Light::TYPE_POINT			= "POINT";
const Char* const Light::TYPE_SPOT			= "SPOT";


Light::Light( GameObject * gameObject )
	: Component( gameObject )
	, _lightType( LightType::DIRECTIONAL )
	, _intensity( 1.0f )
	, _color( Vector3(1, 1, 1) )
	, _spotAngle( 90.0f )
	, _range( 1.0f )
{
	_componentType = ComponentType::LIGHT;
}


void Light::Clone( Component* clone ) const
{
}


LightType Light::GetLightType() const
{
	return _lightType;
}


Vector3 Light::GetColor() const
{
	return _color;
}


Float Light::GetIntensity() const
{
	return _intensity;
}


Float Light::GetRange() const
{
	return _range;
}


Float Light::GetSpotAngle() const
{
	return _spotAngle;
}


const Char* Light::GetLightTypeString() const
{
	if ( _lightType == LightType::DIRECTIONAL ) {
		return TYPE_DIRECTIONAL;
	}
	else if ( _lightType == LightType::POINT ) {
		return TYPE_POINT;
	}
	else if ( _lightType == LightType::SPOT ) {
		return TYPE_SPOT;
	}

	return NULL;
}


void Light::SetLightType( LightType value )
{
	_lightType = value;
}


void Light::SetColor( Vector3& value )
{
	_color = value;
}


void Light::SetIntensity( Float value )
{
	_intensity = value;
}


void Light::SetRange( Float value )
{
	_range = value;
}


void Light::SetSpotAngle( Float value )
{
	_spotAngle = value;
}

void Light::SetLightTypeString( const Char* value )
{
	String sType( value );

	if ( sType == TYPE_DIRECTIONAL ) {
		SetLightType( LightType::DIRECTIONAL );
	}
	else if ( sType == TYPE_POINT ) {
		SetLightType( LightType::POINT );
	}
	else if ( sType == TYPE_SPOT ) {
		SetLightType( LightType::SPOT );
	}
}
