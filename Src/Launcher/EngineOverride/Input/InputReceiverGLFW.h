// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Input.h"


struct GLFWwindow;


namespace CYRED
{
	class InputReceiverGLFW : public InputReceiver
	{
		DECLARE_LOCAL_SINGLETON_IMPL( InputReceiverGLFW )


	public:
		static void		GLFW_KeyCallback	( GLFWwindow* window, int key, int scancode, 
											  int action, int mods );
		static void		GLFW_ButtonCallback	( GLFWwindow* window, int button, int action, int mods );
		static void		GLFW_CursorCallback	( GLFWwindow* window, double xpos, double ypos );
		static void		GLFW_ScrollCallback	( GLFWwindow* window, double xoffset, double yoffset );
		static void		GLFW_FocusCallback	( GLFWwindow* window, int entered );

		static KeyCode	ToKeyCode			( int glfwKey );


	public:
		void	AddEventToQueue( InputEvent event );
		void	CheckModifiers	( int mods );
	};
}