// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredApplication.h"
#include "CyredBuildingBlocks.h"

struct GLFWwindow;

namespace CYRED
{
	class GLContext;
	class GameObject;
	class Console;
	class Node;
}


namespace CYRED
{
	class LauncherApp : public Application
	{
		DECLARE_REMOTE_SINGLETON_IMPL( LauncherApp )


	public:
		void Exit() override;

	public:
		static void GlfwErrorCallback( int code, cchar* desc );


	public:
		void Run( int& argc, char* argv[] );


	private:
		GLFWwindow* _glfwWindow;
		GLContext*	_glContext;

		bool		_shouldExit;
		Console*	_console;

		AppConfig	_appConfig;


	private:
		void _CleanUp			();
		void _UpdateLoop		();

		void _CreateManagers	();
		void _InitializeManagers();
		void _FinalizeManagers	();
		void _DestroyManagers	();

		void _InitializeGLFW	();
		void _ReadConfigFile	();
		void _CreateMainWindow	();
		void _InitializeRenderer();
		void _StartGame			();
		void _RenderScene		();

		void _RecCollectLights	( Node* root, DataArray<GameObject*>& lightsGO );
		void _RecCollectLayers	( GameObject* root, OUT DataArray<int>& layers );
	};
}