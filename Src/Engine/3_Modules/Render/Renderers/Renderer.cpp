// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Renderer.h"

#include "../Assets/Shader.h"
#include "../OpenGL/GL.h"
#include "../OpenGL/Vertex.h"


using namespace CYRED;


uint Renderer::_textVBO = EMPTY_BUFFER;


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

	// create text buffer
	if ( _textVBO == EMPTY_BUFFER ) {
		_CreateTextVBO();
	}

	// call for specific initialization
	_OnInitialize();
}


void Renderer::SwitchCamera( Transform* cameraTran, Camera* camera )
{
	// store camera
	_currCameraTran = cameraTran;
	_currCamera		= camera;
}


void Renderer::_CreateTextVBO()
{
	_gl->GenBuffers( 1, &_textVBO );
	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, _textVBO );
	_gl->BufferData( GLBuffer::ARRAY_BUFFER, sizeof(Vertex) * 6, NULL, GLDrawType::DYNAMIC_DRAW );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, EMPTY_BUFFER );
}

