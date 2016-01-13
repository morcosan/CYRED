// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "Matrix4.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#define GLM_FORCE_RADIANS
#include "glm/Include/glm.hpp"
#include "glm/Include/gtc/matrix_transform.hpp"
#include "glm/Include/gtc/type_ptr.hpp"


using namespace CYRED;



Matrix4::Matrix4()
	: _self( Memory::Alloc<glm::mat4>( 0.0f ) )
{
}


Matrix4::Matrix4( const Matrix4& other )
	: _self( Memory::Alloc<glm::mat4>( *other._self ) )
{
}


Matrix4::Matrix4( const glm::mat4& other )
	: _self( Memory::Alloc<glm::mat4>( other ) )
{
}


Matrix4::~Matrix4()
{
	Memory::Free( _self );
}


void Matrix4::operator=( const Matrix4& other )
{
	*_self = *other._self;
}


Matrix4 Matrix4::operator-( const Matrix4& other )
{
	return Matrix4( *_self - *other._self );
}


Matrix4 Matrix4::operator+( const Matrix4& other )
{
	return Matrix4( *_self + *other._self );
}


Matrix4 Matrix4::operator/( const Matrix4& other )
{
	return Matrix4( *_self / *other._self );
}


Matrix4 Matrix4::operator*( const Matrix4& other )
{
	return Matrix4( *_self * *other._self );
}


Vector4 Matrix4::operator*( const Vector4& other )
{
	glm::vec4 aux = (*_self) * glm::vec4( other.x, other.y, other.z, other.w );
	return Vector4( aux.x, aux.y, aux.z, aux.w );
}


Matrix4 Matrix4::Translate( const Matrix4& mat, const Vector3& other )
{
	return Matrix4( glm::translate( *mat._self, glm::vec3( other.x, other.y, other.z ) ) );
}


Matrix4 Matrix4::Scale( const Matrix4& mat, const Vector3& other )
{
	return Matrix4( glm::scale( *mat._self, glm::vec3( other.x, other.y, other.z ) ) );
}


Matrix4 Matrix4::Identity()
{
	return Matrix4( glm::mat4( 1.0f ) );
}


Matrix4 Matrix4::Inverse( const Matrix4& mat )
{
	return Matrix4( glm::inverse( *mat._self ) );
}


Matrix4 Matrix4::CreatePerspective( Float fovyAngle, Float aspectRatio, Float nearClipping, Float farClipping )
{
	if ( aspectRatio < 0.001f )
	{
		return Identity();
	}
	return Matrix4( glm::perspective( glm::radians( fovyAngle ), aspectRatio, nearClipping, farClipping ) );
}


Matrix4 Matrix4::CreateOrthogonal( const Vector2& size, Float nearClipping, Float farClipping )
{
	Float left		= - size.x / 2;
	Float right		=	size.x / 2;
	Float bottom	= - size.y / 2;
	Float top		=	size.y / 2;
	return Matrix4( glm::ortho( left, right, bottom, top, nearClipping, farClipping ) );
}


const Float* Matrix4::GetValuePtr()
{
	return glm::value_ptr( *_self );
}

