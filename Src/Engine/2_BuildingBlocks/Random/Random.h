// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "../Math/Math.h"
#include "../String/String.h"


namespace CYRED
{
	class DLL Random
	{
		static const int UNIQUE_ID_SIZE;
		static const char ALPHA_NUM[];
        

	private:
		Random() {}
		virtual ~Random() {}


	public:
		static void		Initialize		();

		static void		SetSeed			( int value );

		static float	FromRangeFloat	( float min, float max );	//! both inclusive
		static int		FromRangeInt	( int min, int max );		//! both inclusive
		static Vector2	FromCircle		( float radius );
		static Vector2	FromCircleEdge	( float radius );
		static Vector3	FromSphere		( float radius );
		static Vector3	FromSphereEdge	( float radius );

		static String	GenerateUniqueID();
		static bool		ValidateUniqueID( const char* uniqueID );

		// generate a new id similar to the uid, but constant giving same text
		static String	GenerateConstantID( const char* text );
	};
}