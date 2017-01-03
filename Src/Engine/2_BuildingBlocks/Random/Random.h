// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "../Math/Math.h"
#include "../String/String.h"


namespace CYRED
{
	class DLL Random
	{
		static const UInt UNIQUE_ID_SIZE;
		static const Char ALPHA_NUM[];
        

	private:
		Random() {}
		virtual ~Random() {}


	public:
		static void		Initialize		();

		static void		SetSeed			( UInt value );

		static Float	FromRangeFloat	( Float min, Float max );	//! both inclusive
		static Int		FromRangeInt	( Int min, Int max );		//! both inclusive
		static Vector2	FromCircle		( Float radius );
		static Vector2	FromCircleEdge	( Float radius );
		static Vector3	FromSphere		( Float radius );
		static Vector3	FromSphereEdge	( Float radius );

		static String	GenerateUniqueID();
		static Bool		ValidateUniqueID( const Char* uniqueID );

		// generate a new id similar to the uid, but constant giving same text
		static String	GenerateConstantID( const Char* text );
	};
}