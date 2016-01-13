// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "Math.h"

#define GLM_FORCE_RADIANS
#include "glm/Include/glm.hpp"
#include "glm/Include/gtc/constants.hpp"


using namespace CYRED;


const Float Math::PI		= glm::pi<Float>();
const Float Math::HALF_PI	= glm::pi<Float>() / 2;


Float Math::Dot( const Vector3& firstVec, const Vector3& secondVec )
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


Float Math::ToDegrees( Float value )
{
	return glm::degrees( value );
}


Vector3 Math::ToRadians( const Vector3& vec )
{
	glm::vec3 newVec = glm::radians( glm::vec3( vec.x, vec.y, vec.z ) );
	return Vector3( newVec.x, newVec.y, newVec.z );
}


Float Math::ToRadians( Float value )
{
	return glm::radians( value );
}


Float Math::Max( Float value1, Float value2 )
{
	return glm::max( value1, value2 );
}


Float Math::Min( Float value1, Float value2 )
{
	return glm::min( value1, value2 );
}


Float Math::Tan( Float value )
{
	return glm::tan( value );
}


Float Math::Cos( Float value )
{
	return glm::cos( value );
}


Float Math::Sin( Float value )
{
	return glm::sin( value );
}


Float Math::Clamp( Float value, Float min, Float max )
{
	return ( (value < min) ? min : ( (value > max) ? max : value) );
}


