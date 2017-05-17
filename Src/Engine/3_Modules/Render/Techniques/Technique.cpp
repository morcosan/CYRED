// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "Technique.h"


using namespace CYRED;


void Technique::SetAPI( GL* glAPI )
{
	ASSERT( glAPI != NULL );

	_gl = glAPI;
}


void Technique::SetViewportSize( int width, int height )
{
	_viewportWidth = width;
	_viewportHeight = height;
}
