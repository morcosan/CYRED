// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "glm/Include/fwd.hpp"


namespace CYRED
{
	struct DLL Vector2
	{
		float	x;
		float	y;

		Vector2();
		Vector2( const Vector2& other );
		Vector2( float xx, float yy );

		void	operator=( const Vector2& other );

		bool	operator==( const Vector2& other )	const;
		bool	operator!=( const Vector2& other )	const;

		Vector2 operator-( const Vector2& other )	const;

		Vector2 operator+( const Vector2& other )	const;

		Vector2 operator/( const Vector2& other )	const;
		Vector2 operator/( float other )			const;
		Vector2 operator/( int other )				const;

		Vector2 operator*( const Vector2& other )	const;
		Vector2 operator*( float other )			const;
		Vector2 operator*( int other )				const;

		const float* Ptr();
	};
}