// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "Vector2.h"

#define GLM_FORCE_RADIANS
#include "glm/Include/glm.hpp"
#include "glm/Include/gtc/type_ptr.hpp"


using namespace CYRED;



Vector2::Vector2()
	: x( 0 ), y( 0 )
{
}


Vector2::Vector2( const Vector2& other )
	: x( other.x ), y( other.y )
{
}


Vector2::Vector2( float xx, float yy )
	: x( xx ), y( yy )
{
}


void Vector2::operator=( const Vector2& other )
{
	 x = other.x;
	 y = other.y;
}


bool Vector2::operator==( const Vector2& other ) const
{
	glm::vec2 vec_self( x, y );
	glm::vec2 vec_other( other.x, other.y );
	glm::bvec2 result = glm::equal( vec_self, vec_other );

	return result.x && result.y;
}


bool Vector2::operator!=( const Vector2& other ) const
{
	return !(*this == other);
}


Vector2 Vector2::operator-( const Vector2& other ) const
{
	return Vector2( x - other.x, y - other.y );
}


Vector2 Vector2::operator+( const Vector2& other ) const
{
	return Vector2( x + other.x, y + other.y );
}


Vector2 Vector2::operator/( const Vector2& other ) const
{
	return Vector2( x / other.x, y / other.y );
}


Vector2 Vector2::operator/( float value ) const
{
	return Vector2( x / value, y / value );
}


Vector2 Vector2::operator/( int value ) const
{
	return Vector2( x / value, y / value );
}


Vector2 Vector2::operator/( int value ) const
{
	return Vector2( x / value, y / value );
}


Vector2 Vector2::operator*( const Vector2& other ) const
{
	return Vector2( x * other.x, y * other.y );
}


Vector2 Vector2::operator*( float value ) const
{
	return Vector2( x * value, y * value );
}


Vector2 Vector2::operator*( int value ) const
{
	return Vector2( x * value, y * value );
}


Vector2 Vector2::operator*( int value ) const
{
	return Vector2( x * value, y * value );
}


const float* Vector2::Ptr()
{
	return &x;
}

