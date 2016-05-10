// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "FiniteString.h"

#include <cstdio>
#include <cstdarg>


using namespace CYRED;



FiniteString::FiniteString()
{
}


FiniteString::~FiniteString()
{
}


FiniteString::FiniteString( const Char* format, ... )
{
	va_list args;
	va_start( args, format );
	vsprintf( _data, format, args );
	va_end( args );
}


void FiniteString::Set( const Char* format, ... )
{
	va_list args;
	va_start( args, format );
	vsprintf( _data, format, args );
	va_end( args );
}


Bool FiniteString::operator==( const Char* other ) const
{
	if ( other == NULL )
	{
		return ( GetLength() == 0 );
	}

	return ( strcmp( _data, other ) == 0 );
}


Bool FiniteString::operator!=( const Char* other ) const
{
	return !operator==( other );
}


Bool FiniteString::operator<( const Char* other ) const
{
	if ( other == NULL )
	{
		return FALSE;
	}

	return ( strcmp( _data, other ) < 0 );
}


const Char* FiniteString::GetChar() const
{
	return _data;
}


UInt FiniteString::GetLength() const
{
	return strlen( _data );
}
