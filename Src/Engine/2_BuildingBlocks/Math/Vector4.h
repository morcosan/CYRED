// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "glm/Include/fwd.hpp"


namespace CYRED
{
	struct DLL Vector4
	{
		float x;
		float y;
		float z;
		float w;

		Vector4();
		Vector4( const Vector4& other );
		Vector4( float xx, float yy, float zz, float ww );

		void	operator=( const Vector4& other );

		bool	operator==( const Vector4& other )	const;
		bool	operator!=( const Vector4& other )	const;

		Vector4 operator-( const Vector4& other )	const;

		Vector4 operator+( const Vector4& other )	const;

		Vector4 operator/( const Vector4& other )	const;
		Vector4 operator/( float other )			const;
		Vector4 operator/( int other )				const;
		Vector4 operator/( int other )				const;

		Vector4 operator*( const Vector4& other )	const;
		Vector4 operator*( float other )			const;	
		Vector4 operator*( int other )				const;
		Vector4 operator*( int other )				const;

		const float* Ptr();
	};
}