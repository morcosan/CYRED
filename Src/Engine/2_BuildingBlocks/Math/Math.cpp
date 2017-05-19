// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#include "Math.h"

#define GLM_FORCE_RADIANS
#include "glm/Include/glm.hpp"
#include "glm/Include/gtc/constants.hpp"


using namespace CYRED;


const float Math::PI		= glm::pi<float>();
const float Math::HALF_PI	= glm::pi<float>() / 2;


float Math::Dot( const Vector3& firstVec, const Vector3& secondVec )
{
	glm::vec3 first = glm::vec3( firstVec.x, firstVec.y, firstVec.z );
	glm::vec3 second = glm::vec3( secondVec.x, secondVec.y, secondVec.z );
	return glm::dot( first, second );
}


Vector3 Math::ToDegrees( const Vector3& vec )
{
	glm::vec3 newVec = glm::degrees( glm::vec3( vec.x, vec.y, vec.z ) );
	return Vector3( newVec.x, newVec.y, newVec.z );
}


float Math::ToDegrees( float value )
{
	return glm::degrees( value );
}


Vector3 Math::ToRadians( const Vector3& vec )
{
	glm::vec3 newVec = glm::radians( glm::vec3( vec.x, vec.y, vec.z ) );
	return Vector3( newVec.x, newVec.y, newVec.z );
}


float Math::ToRadians( float value )
{
	return glm::radians( value );
}


float Math::Max( float value1, float value2 )
{
	return glm::max( value1, value2 );
}


float Math::Min( float value1, float value2 )
{
	return glm::min( value1, value2 );
}


float Math::Tan( float value )
{
	return glm::tan( value );
}


float Math::Cos( float value )
{
	return glm::cos( value );
}


float Math::Sin( float value )
{
	return glm::sin( value );
}


float Math::Clamp( float value, float min, float max )
{
	return ( (value < min) ? min : ( (value > max) ? max : value) );
}


