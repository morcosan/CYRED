// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Vector3.h"

#define GLM_FORCE_RADIANS
#include "glm/Include/glm.hpp"


using namespace CYRED;


Vector3::Vector3()
	: x( 0 ), y( 0 ), z( 0 )
{
}


Vector3::Vector3( const Vector3& other )
	: x( other.x ), y( other.y ), z( other.z ) 
{
}


Vector3::Vector3( float xx, float yy, float zz )
	: x( xx ), y( yy ), z( zz )
{
}


void Vector3::operator=( const Vector3& other )
{
	x = other.x;
	y = other.y;
	z = other.z;
}


void Vector3::operator+=( const Vector3& other )
{
	x += other.x;
	y += other.y;
	z += other.z;
}


void Vector3::operator-=( const Vector3& other )
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}


void Vector3::operator/=( const Vector3& other )
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
}


void Vector3::operator*=( const Vector3& other )
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
}


void Vector3::operator*=( float other )
{
	x *= other;
	y *= other;
	z *= other;
}


void Vector3::operator*=( int other )
{
	x *= other;
	y *= other;
	z *= other;
}


bool Vector3::operator==( const Vector3& other ) const
{
	glm::vec3 vec_self( x, y, z );
	glm::vec3 vec_other( other.x, other.y, other.z );
	glm::bvec3 result = glm::equal( vec_self, vec_other );

	return result.x && result.y && result.z;
}


bool Vector3::operator!=( const Vector3& other ) const
{
	return !(*this == other);
}


Vector3 Vector3::operator-( const Vector3& other ) const
{
	return Vector3( x - other.x, y - other.y, z - other.z );
}


Vector3 Vector3::operator-() const
{
	return Vector3( - x, - y, - z );
}


Vector3 Vector3::operator+( const Vector3& other ) const
{
	return Vector3( x + other.x, y + other.y, z + other.z );
}


Vector3 Vector3::operator/( const Vector3& other ) const
{
	return Vector3( x / other.x, y / other.y, z / other.z );
}


Vector3 Vector3::operator/( float value ) const
{
	return Vector3( x / value, y / value, z / value );
}


Vector3 Vector3::operator/( int value ) const
{
	return Vector3( x / value, y / value, z / value );
}


Vector3 Vector3::operator*( const Vector3& other ) const
{
	return Vector3( x * other.x, y * other.y, z * other.z );
}


Vector3 Vector3::operator*( float value ) const
{
	return Vector3( x * value, y * value, z * value );
}


Vector3 Vector3::operator*( int value ) const
{
	return Vector3( x * value, y * value, z * value );
}


const float* Vector3::Ptr()
{
	return &x;
}


Vector3 Vector3::Normalize( const Vector3& other )
{
	glm::vec3 aux = glm::normalize( glm::vec3( other.x, other.y, other.z ) );
	return Vector3( aux.x, aux.y, aux.z );
}