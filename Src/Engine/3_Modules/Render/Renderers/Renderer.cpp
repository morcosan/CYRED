// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#include "Renderer.h"
#include "../Assets/Shader.h"


using namespace CYRED;


Shader* Renderer::_screenQuadShader = NULL;


/*****
* @desc: prepare the renderer
* @params: 
* 		glAPI		- the global GL API
* 		glContext	- the context of the canvas
*/
void Renderer::Initialize( GL* glAPI, GLContext* glContext )
{
	ASSERT( glAPI != NULL );
	ASSERT( glContext != NULL );

	// store data
	_gl = glAPI;
	_glContext = glContext;

	// prepare screen quad
	if ( _screenQuadShader == NULL ) {
		_screenQuadShader = Memory::Alloc<Shader>();
	}

	// call for specific initialization
	_OnInitialize();
}

