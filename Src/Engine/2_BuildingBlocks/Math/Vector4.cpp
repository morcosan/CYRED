// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#include "Vector4.h"

#define GLM_FORCE_RADIANS
#include "glm/Include/glm.hpp"
#include "glm/Include/gtc/type_ptr.hpp"


using namespace CYRED;



Vector4::Vector4()
	: x( 0 ), y( 0 ), z( 0 ), w( 0 )
{
}


Vector4::Vector4( const Vector4& other )
	: x( other.x ), y( other.y ), z( other.z ), w( other.w )
{
}


Vector4::Vector4( float xx, float yy, float zz, float ww )
	: x( xx ), y( yy ), z( zz ), w( ww )
{
}


void Vector4::operator=( const Vector4& other )
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}


bool Vector4::operator==( const Vector4& other ) const
{
	glm::vec4 vec_self( x, y, z, w );
	glm::vec4 vec_other( other.x, other.y, other.z, other.w );
	glm::bvec4 result = glm::equal( vec_self, vec_other );

	return result.x && result.y && result.z && result.w;
}


bool Vector4::operator!=( const Vector4& other ) const
{
	return !(*this == other);
}


Vector4 Vector4::operator-( const Vector4& other ) const
{
	return Vector4( x - other.x, y - other.y, z - other.z, w - other.w );
}


Vector4 Vector4::operator+( const Vector4& other ) const
{
	return Vector4( x + other.x, y + other.y, z + other.z, w + other.w );
}


Vector4 Vector4::operator/( const Vector4& other ) const
{
	return Vector4( x / other.x, y / other.y, z / other.z, w / other.w );
}


Vector4 Vector4::operator/( float value ) const
{
	return Vector4( x / value, y / value, z / value, w / value );
}


Vector4 Vector4::operator/( int value ) const
{
	return Vector4( x / value, y / value, z / value, w / value );
}


Vector4 Vector4::operator*( const Vector4& other ) const
{
	return Vector4( x * other.x, y * other.y, z * other.z, w * other.w );
}


Vector4 Vector4::operator*( float value ) const
{
	return Vector4( x * value, y * value, z * value, w * value );
}


Vector4 Vector4::operator*( int value ) const
{
	return Vector4( x * value, y * value, z * value, w * value );
}


const float* Vector4::Ptr()
{
	return &x;
}

