// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Quaternion.h"

#include "Vector3.h"
#include "Matrix4.h"

#define GLM_FORCE_RADIANS
#include "glm/Include/glm.hpp"
#include "glm/Include/gtx/quaternion.hpp"


using namespace CYRED;


Quaternion::Quaternion()
	: x( 0 ), y( 0 ), z( 0 ), w( 1 )
{
}


Quaternion::Quaternion( const Quaternion& other )
	: x( other.x ), y( other.y ), z( other.z ), w( other.w )
{
}


Quaternion::Quaternion( const Vector3& other )
{
	glm::quat aux = glm::quat( glm::vec3( other.x, other.y, other.z ) );
	this->x = aux.x;
	this->y = aux.y;
	this->z = aux.z;
	this->w = aux.w;
}


Quaternion::Quaternion( float xx, float yy, float zz, float ww )
	: x( xx ), y( yy ), z( zz ), w( ww )
{
}


Quaternion::~Quaternion()
{
}


void Quaternion::operator=( const Quaternion& other )
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;
}


Quaternion Quaternion::operator+( const Quaternion& other ) const
{
	glm::quat aux = glm::quat( w, x, y, z ) + glm::quat( other.w, other.x, other.y, other.z );
	return Quaternion( aux.x, aux.y, aux.z, aux.w );
}


Quaternion Quaternion::operator*( const Quaternion& other ) const
{
	glm::quat aux = glm::quat( w, x, y, z ) * glm::quat( other.w, other.x, other.y, other.z );
	return Quaternion( aux.x, aux.y, aux.z, aux.w );
}


Vector3 Quaternion::ToEulerAngles() const
{
	glm::vec3 aux = glm::eulerAngles( glm::quat( w, x, y, z ) );
	return Vector3( aux.x, aux.y, aux.z );
}


Vector3 Quaternion::ApplyRotation( const Vector3& other )
{
	glm::vec3 otherVec3 = glm::vec3( other.x, other.y, other.z );
	glm::vec3 aux = glm::rotate( glm::quat( w, x, y, z ), otherVec3 );
	return Vector3( aux.x, aux.y, aux.z );
}


Matrix4 Quaternion::ToMatrix4() const
{
	return Matrix4( glm::toMat4( glm::quat( w, x, y, z ) ) );
}


Quaternion Quaternion::Inverse( const Quaternion& other )
{
	glm::quat aux = glm::inverse( glm::quat( other.w, other.x, other.y, other.z ) );
	return Quaternion( aux.x, aux.y, aux.z, aux.w );
}


Quaternion Quaternion::Normalize( const Quaternion& other )
{
	glm::quat aux = glm::normalize( glm::quat( other.w, other.x, other.y, other.z ) );
	return Quaternion( aux.x, aux.y, aux.z, aux.w );
}


Vector3 Quaternion::EulerAngles( const Quaternion& other )
{
	glm::vec3 aux = glm::eulerAngles( glm::quat( other.w, other.x, other.y, other.z ) );
	return Vector3( aux.x, aux.y, aux.z );
}

