// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Random.h"

#include <stdlib.h>     //! srand, rand
#include <time.h>       //! time 
#include <cstring>


using namespace CYRED;



const UInt Random::UNIQUE_ID_SIZE	= 32;		// same as Unity
const Char Random::ALPHA_NUM[]		= "0123456789"
									  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
									  "abcdefghijklmnopqrstuvwxyz";


void Random::Initialize()
{
	srand( CAST_S( UInt, time( NULL ) ) );
}


void Random::SetSeed( UInt value )
{
	srand( value );
}


Float Random::FromRange( Float min, Float max )
{
	if ( min >= max )
	{
		return min;
	}

	Float r = CAST_S( Float, rand() ) / RAND_MAX; //! from 0-1 inclusive
	return (r * (max - min) + min);
}


Int Random::FromRange( Int min, Int max )
{
	if ( min >= max )
	{
		return min;
	}
	return (rand() % (max - min + 1) + min);
}


Vector2 Random::FromCircle( Float radius )
{
	Float theta = FromRange( 0.0f, 2 * Math::PI );
	Float r = FromRange( 0.0f, radius );

	Vector2 point;
	point.x = r * Math::Cos( theta );
	point.y = r * Math::Sin( theta );

	return point;
}


Vector2 Random::FromCircleEdge( Float radius )
{
	Float theta = FromRange( 0.0f, 2 * Math::PI );

	Vector2 point;
	point.x = radius * Math::Cos( theta );
	point.y = radius * Math::Sin( theta );

	return point;
}


Vector3 Random::FromSphere( Float radius )
{
	Float theta = FromRange( 0.0f, 2 * Math::PI );
	Float phi	= FromRange( - Math::HALF_PI, Math::HALF_PI );
	Float r		= FromRange( 0.0f, radius );

	Vector3 point;
	point.x = r * Math::Cos( theta ) * Math::Cos( phi );
	point.y = r * Math::Sin( phi );
	point.z = r * Math::Sin( theta ) * Math::Cos( phi );

	return point;
}


Vector3 Random::FromSphereEdge( Float radius )
{
	Float theta = FromRange( 0.0f, 2 * Math::PI );
	Float phi	= FromRange( - Math::HALF_PI, Math::HALF_PI );

	Vector3 point;
	point.x = radius * Math::Cos( theta ) * Math::Cos( phi );
	point.y = radius * Math::Sin( phi );
	point.z = radius * Math::Sin( theta ) * Math::Cos( phi );

	return point;
}


String Random::GenerateUniqueID()
{
	UInt totalChars = sizeof( ALPHA_NUM );

	Char id[ UNIQUE_ID_SIZE + 1 ];

	for ( UInt i = 0; i < UNIQUE_ID_SIZE; ++i )
	{
		id[i] = ALPHA_NUM[ rand() % (totalChars - 1) ];
    }

    id[UNIQUE_ID_SIZE] = 0;

	return String( id );
}


Bool Random::ValidateUniqueID( const Char* uniqueID )
{
	if ( strlen( uniqueID ) != UNIQUE_ID_SIZE )
	{
		return FALSE;
	}

	UInt totalChars = sizeof( ALPHA_NUM );

	for ( UInt i = 0; i < UNIQUE_ID_SIZE; ++i )
	{
		Bool found = FALSE;
		for ( UInt j = 0; j < totalChars; ++j )
		{
			if ( uniqueID[i] == ALPHA_NUM[j] )
			{
				found = TRUE;
			}
		}

		if ( !found )
		{
			return FALSE;
		}
	}

	return TRUE;
}
