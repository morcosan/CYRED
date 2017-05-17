// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Light.h"

#include "../../Event/EventManager.h"
#include "../../../2_BuildingBlocks/String/String.h"


using namespace CYRED;


const char* const Light::TYPE_DIRECTIONAL	= "DIRECTIONAL";
const char* const Light::TYPE_POINT			= "POINT";
const char* const Light::TYPE_SPOT			= "SPOT";


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


float Light::GetIntensity() const
{
	return _intensity;
}


float Light::GetRange() const
{
	return _range;
}


float Light::GetSpotAngle() const
{
	return _spotAngle;
}


const char* Light::GetLightTypeString() const
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

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_LIGHT, this );
	}
}


void Light::SetColor( Vector3& value )
{
	_color = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_LIGHT, this );
	}
}


void Light::SetIntensity( float value )
{
	_intensity = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_LIGHT, this );
	}
}


void Light::SetRange( float value )
{
	_range = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_LIGHT, this );
	}
}


void Light::SetSpotAngle( float value )
{
	_spotAngle = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_LIGHT, this );
	}
}


void Light::SetLightTypeString( const char* value )
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
