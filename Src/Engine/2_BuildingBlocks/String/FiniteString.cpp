// Copyright (c) 2015-2017 Morco (www.morco.ro)
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


FiniteString::FiniteString( cchar* format, ... )
{
	va_list args;
	va_start( args, format );
	vsprintf( _data, format, args );
	va_end( args );
}


void FiniteString::Set( cchar* format, ... )
{
	va_list args;
	va_start( args, format );
	vsprintf( _data, format, args );
	va_end( args );
}


bool FiniteString::operator==( cchar* other ) const
{
	if ( other == NULL )
	{
		return ( GetLength() == 0 );
	}

	return ( strcmp( _data, other ) == 0 );
}


bool FiniteString::operator!=( cchar* other ) const
{
	return !operator==( other );
}


bool FiniteString::operator<( cchar* other ) const
{
	if ( other == NULL )
	{
		return FALSE;
	}

	return ( strcmp( _data, other ) < 0 );
}


cchar* FiniteString::GetChar() const
{
	return _data;
}


int FiniteString::GetLength() const
{
	return strlen( _data );
}
