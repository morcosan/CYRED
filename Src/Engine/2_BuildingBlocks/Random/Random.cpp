// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Random.h"

#include <stdlib.h>     //! srand, rand
#include <time.h>       //! time 
#include <cstring>


using namespace CYRED;


const int	Random::UNIQUE_ID_SIZE	= 32;		// same as Unity
cchar		Random::ALPHA_NUM[]		= "0123456789"
									  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
									  "abcdefghijklmnopqrstuvwxyz";


void Random::Initialize()
{
	srand( CAST_S( int, time( NULL ) ) );
}


void Random::SetSeed( int value )
{
	srand( value );
}


float Random::FromRangeFloat( float min, float max )
{
	if ( min >= max )
	{
		return min;
	}

	float r = CAST_S( float, rand() ) / RAND_MAX; //! from 0-1 inclusive
	return (r * (max - min) + min);
}


int Random::FromRangeInt( int min, int max )
{
	if ( min >= max )
	{
		return min;
	}
	return (rand() % (max - min + 1) + min);
}


Vector2 Random::FromCircle( float radius )
{
	float theta = FromRangeFloat( 0.0f, 2 * Math::PI );
	float r = FromRangeFloat( 0.0f, radius );

	Vector2 point;
	point.x = r * Math::Cos( theta );
	point.y = r * Math::Sin( theta );

	return point;
}


Vector2 Random::FromCircleEdge( float radius )
{
	float theta = FromRangeFloat( 0.0f, 2 * Math::PI );

	Vector2 point;
	point.x = radius * Math::Cos( theta );
	point.y = radius * Math::Sin( theta );

	return point;
}


Vector3 Random::FromSphere( float radius )
{
	float theta = FromRangeFloat( 0.0f, 2 * Math::PI );
	float phi	= FromRangeFloat( - Math::HALF_PI, Math::HALF_PI );
	float r		= FromRangeFloat( 0.0f, radius );

	Vector3 point;
	point.x = r * Math::Cos( theta ) * Math::Cos( phi );
	point.y = r * Math::Sin( phi );
	point.z = r * Math::Sin( theta ) * Math::Cos( phi );

	return point;
}


Vector3 Random::FromSphereEdge( float radius )
{
	float theta = FromRangeFloat( 0.0f, 2 * Math::PI );
	float phi	= FromRangeFloat( - Math::HALF_PI, Math::HALF_PI );

	Vector3 point;
	point.x = radius * Math::Cos( theta ) * Math::Cos( phi );
	point.y = radius * Math::Sin( phi );
	point.z = radius * Math::Sin( theta ) * Math::Cos( phi );

	return point;
}


String Random::GenerateUniqueID()
{
	int totalChars = sizeof( ALPHA_NUM );

	char id[ UNIQUE_ID_SIZE + 1 ];

	for ( int i = 0; i < UNIQUE_ID_SIZE; ++i )
	{
		id[i] = ALPHA_NUM[ rand() % (totalChars - 1) ];
    }

    id[UNIQUE_ID_SIZE] = 0;

	return String( id );
}


bool Random::ValidateUniqueID( cchar* uniqueID )
{
	if ( strlen( uniqueID ) != UNIQUE_ID_SIZE )
	{
		return FALSE;
	}

	int totalChars = sizeof( ALPHA_NUM );

	for ( int i = 0; i < UNIQUE_ID_SIZE; ++i )
	{
		bool found = FALSE;
		for ( int j = 0; j < totalChars; ++j )
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


String Random::GenerateConstantID( cchar* text )
{
	// get available chars
	int totalAlphanum = sizeof( ALPHA_NUM );

	// create id string
	char id[ UNIQUE_ID_SIZE + 1 ];

	// start from end of text
	int textSize = strlen( text );

	// replace each char with an alphanum
	for ( int i = 0; i < UNIQUE_ID_SIZE; ++i )
	{
		int textIndex = textSize - i - 1;
		if ( textIndex < 0 ) {
			textIndex += textSize;
		}
		int alphanumIndex = CAST_S( int, text[textIndex] ) % totalAlphanum;
		id[i] = ALPHA_NUM[alphanumIndex];
    }

    id[UNIQUE_ID_SIZE] = 0;

	return String( id );
}
