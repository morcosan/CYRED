// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredApplication.h"

struct GLFWwindow;

namespace CYRED
{
	class GLContext;
	class GameObject;
}


namespace CYRED
{
	class StandaloneApp : public Application
	{
		DECLARE_REMOTE_SINGLETON_IMPL( StandaloneApp )


	public:
		void Exit() override;

	public:
		static void GlfwErrorCallback( Int code, const Char* desc );


	public:
		void Run( Int& argc, Char* argv[] );


	protected:
		GLFWwindow* _glfwWindow;
		GLContext*	_glContext;

		Bool		_shouldExit;
		GameObject* _missingCamera;

		AppConfig	_appConfig;


	protected:
		/* prepares the application for exit				*/
		/* called once before closing the editor			*/
		void _CleanUp			();
		/* the main loop of application						*/
		/* called continuously, every frame					*/
		void _UpdateLoop		();

		/* create singletons for manager classes			*/
		void _CreateManagers	();
		/* initialize singletons							*/
		void _InitializeManagers();
		/* finalize singletons								*/
		void _FinalizeManagers	();
		/* destroy the singletons							*/
		void _DestroyManagers	();

		void _InitializeGLFW	();
		void _ReadConfigFile	();
		void _CreateMainWindow	();
		void _InitializeRenderer();
		void _StartGame			();
		void _RenderScene		();
	};
}