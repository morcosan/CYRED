// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "glm/Include/fwd.hpp"


namespace CYRED
{
	struct DLL Vector2
	{
		Float	x;
		Float	y;

		Vector2();
		Vector2( const Vector2& other );
		Vector2( Float xx, Float yy );

		void	operator=( const Vector2& other );

		Bool	operator==( const Vector2& other )	const;
		Bool	operator!=( const Vector2& other )	const;

		Vector2 operator-( const Vector2& other )	const;

		Vector2 operator+( const Vector2& other )	const;

		Vector2 operator/( const Vector2& other )	const;
		Vector2 operator/( Float other )			const;
		Vector2 operator/( UInt other )				const;
		Vector2 operator/( Int other )				const;

		Vector2 operator*( const Vector2& other )	const;
		Vector2 operator*( Float other )			const;
		Vector2 operator*( UInt other )				const;
		Vector2 operator*( Int other )				const;

		const Float* Ptr();
	};
}