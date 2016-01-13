// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "GLImpl_3_0.h"

#include <QtGui/QOpenGLFunctions_3_0>


using namespace CYRED;
using namespace NotAPI;



void GLImpl_3_0::Initialize()
{
	_qtGL = Memory::Alloc<QOpenGLFunctions_3_0>();
	_qtGL->initializeOpenGLFunctions();
}


void GLImpl_3_0::Viewport( Int x, Int y, UInt width, UInt height )
{
	ASSERT( _qtGL != NULL );
	_qtGL->glViewport( x, y, width, height );
}


void GLImpl_3_0::ClearColor( Float r, Float g, Float b, Float a )
{
	ASSERT( _qtGL != NULL );
	_qtGL->glClearColor( r, g, b, a );
}


void GLImpl_3_0::Clear( UInt bitMask )
{
	ASSERT( _qtGL != NULL );

	UInt glBitMask = 0;

	if ( (bitMask & GLFlag::COLOR_BUFFER_BIT) != 0 )
	{
		glBitMask |= GL_COLOR_BUFFER_BIT;
	}
	if ( (bitMask & GLFlag::DEPTH_BUFFER_BIT) != 0 )
	{
		glBitMask |= GL_DEPTH_BUFFER_BIT;
	}

	_qtGL->glClear( glBitMask );
}


