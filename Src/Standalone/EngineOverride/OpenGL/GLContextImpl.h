// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredOpenGL.h"

struct GLFWwindow;


namespace CYRED
{
	class GLContextImpl : public GLContext
	{
	public:
		GLContextImpl( GLFWwindow* glfwWindow );
		virtual ~GLContextImpl() {}


	public:
		void MakeCurrent	() override;
		void SwapBuffers	() override;
		void OnResize		() override;

		void SetSharedContext( GLContext* other ) override;


	protected:
		GLFWwindow* _glfwWindow;
	};
}
