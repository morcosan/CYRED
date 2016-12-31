// Copyright (c) 2015 Morco (www.morco.ro)
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
		static void		GLFW_KeyCallback	( GLFWwindow* window, Int key, Int scancode, 
											  Int action, Int mods );
		static void		GLFW_ButtonCallback	( GLFWwindow* window, Int button, Int action, Int mods );
		static void		GLFW_CursorCallback	( GLFWwindow* window, Double xpos, Double ypos );
		static void		GLFW_ScrollCallback	( GLFWwindow* window, Double xoffset, Double yoffset );
		static void		GLFW_FocusCallback	( GLFWwindow* window, Int entered );

		static KeyCode	ToKeyCode			( Int glfwKey );


	public:
		void	AddEventToQueue( InputEvent event );
		void	CheckModifiers	( Int mods );
	};
}