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
	: _self( new glm::quat() )
{
}


Quaternion::Quaternion( const Quaternion& other )
	: _self( new glm::quat( *other._self ) )
{
}


Quaternion::Quaternion( const glm::quat& other )
	: _self( new glm::quat( other ) )
{
}


Quaternion::Quaternion( const Vector3& other )
	: _self( new glm::quat( glm::vec3(other.x, other.y, other.z) ) )
{
}


Quaternion::~Quaternion()
{
	PTR_FREE( _self );
}


void Quaternion::operator=( const Quaternion& other )
{
	*_self = *other._self;
}


Quaternion Quaternion::operator+( const Quaternion& other ) const
{
	return Quaternion( *_self + *other._self );
}


Quaternion Quaternion::operator*( const Quaternion& other ) const
{
	return Quaternion( *_self * *other._self );
}


Vector3 Quaternion::ToEulerAngles() const
{
	glm::vec3 aux = glm::eulerAngles( *_self );
	return Vector3( aux.x, aux.y, aux.z );
}


Vector3 Quaternion::ApplyRotation( const Vector3& other )
{
	glm::vec3 otherVec3 = glm::vec3( other.x, other.y, other.z );
	glm::vec3 aux = glm::rotate( *_self, otherVec3 );
	return Vector3( aux.x, aux.y, aux.z );
}


Matrix4 Quaternion::ToMatrix4() const
{
	return Matrix4( glm::toMat4( *_self ) );
}


Quaternion Quaternion::Inverse( const Quaternion& other )
{
	return Quaternion( glm::inverse( *other._self ) );
}


Quaternion Quaternion::Normalize( const Quaternion& other )
{
	return Quaternion( glm::normalize( *other._self ) );
}

