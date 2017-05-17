// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "FiniteString.h"

#include <cstdio>
#include <cstdarg>
#include <cstring>


using namespace CYRED;



FiniteString::FiniteString()
{
}


FiniteString::~FiniteString()
{
}


FiniteString::FiniteString( const char* format, ... )
{
	va_list args;
	va_start( args, format );
	vsprintf( _data, format, args );
	va_end( args );
}


void FiniteString::Set( const char* format, ... )
{
	va_list args;
	va_start( args, format );
	vsprintf( _data, format, args );
	va_end( args );
}


bool FiniteString::operator==( const char* other ) const
{
	if ( other == NULL )
	{
		return ( GetLength() == 0 );
	}

	return ( strcmp( _data, other ) == 0 );
}


bool FiniteString::operator!=( const char* other ) const
{
	return !operator==( other );
}


bool FiniteString::operator<( const char* other ) const
{
	if ( other == NULL )
	{
		return FALSE;
	}

	return ( strcmp( _data, other ) < 0 );
}


const char* FiniteString::GetChar() const
{
	return _data;
}


int FiniteString::GetLength() const
{
	return strlen( _data );
}
