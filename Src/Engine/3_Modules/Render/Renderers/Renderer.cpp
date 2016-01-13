// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "Renderer.h"
#include "../Techniques/Technique.h"
#include "../Assets/Shader.h"


using namespace CYRED;



Shader* Renderer::_screenQuadShader = NULL;


void Renderer::Initialize( GL* glAPI, GLContext* glContext )
{
	ASSERT( glAPI != NULL );
	ASSERT( glContext != NULL );

	_gl = glAPI;
	_glContext = glContext;

	if ( _screenQuadShader == NULL )
	{
		_screenQuadShader = Memory::Alloc<Shader>();
	}

	_OnInitialize();
}


void Renderer::SetTechnique( Technique* technique )
{
	ASSERT( technique != NULL );

	_technique = technique;
	_technique->SetAPI( _gl );
}


