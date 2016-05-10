// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "GLContextImpl.h"

#include "GLFW\Include\glfw3.h"


using namespace CYRED;



GLContextImpl::GLContextImpl( GLFWwindow* glfwWindow )
	: _glfwWindow( glfwWindow )
{
}


void GLContextImpl::MakeCurrent()
{
}


void GLContextImpl::SwapBuffers()
{
	glfwSwapBuffers( _glfwWindow );
}


void GLContextImpl::OnResize()
{
	Int width;
	Int height;
	glfwGetWindowSize( _glfwWindow, &width, &height );

	_width = width;
	_height = height;
}


void GLContextImpl::SetSharedContext( GLContext* other )
{
}
