// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "StandaloneApp.h"
#include "CyredBuildingBlocks.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Debug.h"
#include "CyredModule_Event.h"
#include "CyredModule_File.h"
#include "CyredModule_Input.h"
#include "CyredModule_Render.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Script.h"
#include "CyredModule_Time.h"

#include "EngineOverride\OpenGL\GLImpl_3_0.h"
#include "EngineOverride\OpenGL\GLContextImpl.h"
#include "EngineOverride\Input\InputReceiverGLFW.h"

#include "Sections\GameInitScript.h"
#include "Sections\Console.h"

#include "GLEW\Include\glew.h"
#include "GLFW\Include\glfw3.h"


using namespace CYRED;



DEFINE_REMOTE_SINGLETON_IMPL( StandaloneApp )


void StandaloneApp::Run( Int& argc, Char* argv[] )
{
	_InitializeGLFW();

	_CreateManagers();
	_InitializeManagers();

	// init console
	_console = Memory::Alloc<Console>();

	_ReadConfigFile();
	_CreateMainWindow();

	_InitializeRenderer();
	
	//! start the main loop
	glfwSetTime( 0 );
	_shouldExit = FALSE;
	_StartGame();

	while ( !_shouldExit ) //! _shouldExit will be changed from other thread
	{
		_UpdateLoop();
	}

	_CleanUp();
}


void StandaloneApp::Exit()
{
	_shouldExit = TRUE;
}


void StandaloneApp::_CleanUp()
{
	_FinalizeManagers();
	_DestroyManagers();

	Memory::Free( _glContext );

	// destroy input receiver
	InputReceiverGLFW::DestroySingleton();

	glfwDestroyWindow( _glfwWindow );
    glfwTerminate();
}


void StandaloneApp::_UpdateLoop()
{
	TimeManager* timeManager = TimeManager::Singleton();

	Double realGameTime = glfwGetTime();
	timeManager->RenderUpdate( realGameTime );

	while ( timeManager->GetGameTime() < realGameTime ) {
		// time update
		timeManager->Update();

		//! game update
		//! update scripts
		SceneManager* sceneManager = SceneManager::Singleton();
		UInt totalScenes = sceneManager->CountLoadedScenes();
		for ( UInt i = 0; i < totalScenes; ++i ) {
			sceneManager->GetScene( i )->OnUpdate();
		}
	}

	// render
	_RenderScene();

	//! get and process events
	glfwPollEvents();
	// check for close button
	_shouldExit |= ( glfwWindowShouldClose( _glfwWindow ) == 1 );

	// process input
	InputManager::Singleton()->ProcessEvents();
}


void StandaloneApp::_CreateManagers()
{
	SceneManager::CreateSingleton();
	RenderManager::CreateSingleton();
	EventManager::CreateSingleton();
	AssetManager::CreateSingleton();
	FileManager::CreateSingleton();
	InputManager::CreateSingleton();
	TimeManager::CreateSingleton();
	DebugManager::CreateSingleton();
	ScriptManager::CreateSingleton();

	Random::Initialize();
}


void StandaloneApp::_InitializeManagers()
{
	EventManager::Singleton()->Initialize();
	SceneManager::Singleton()->Initialize();
	AssetManager::Singleton()->Initialize();

	SerializeSystem* serializeSystem = Memory::Alloc<JsonSerializeSystem>();
	
	
	FileManager::Singleton()->Initialize( Memory::Alloc<FileSystem>(),
										  Memory::Alloc<MeshLoader>() );
	FileManager::Singleton()->SetSerializeSystem( serializeSystem );

	// also create input receiver
	InputReceiverGLFW::CreateSingleton();
	InputManager::Singleton()->Initialize( InputReceiverGLFW::Singleton() );

	TimeManager::Singleton()->Initialize( 0 );
	DebugManager::Singleton()->Initialize();
	ScriptManager::Singleton()->Initialize();
}


void StandaloneApp::_FinalizeManagers()
{
	SceneManager::Singleton()->Finalize();
	EventManager::Singleton()->Finalize();
	AssetManager::Singleton()->Finalize();
	RenderManager::Singleton()->Finalize();
	FileManager::Singleton()->Finalize();
	InputManager::Singleton()->Finalize();
	TimeManager::Singleton()->Finalize();
	DebugManager::Singleton()->Finalize();
	ScriptManager::Singleton()->Finalize();
}


void StandaloneApp::_DestroyManagers()
{
	SceneManager::DestroySingleton();
	EventManager::DestroySingleton();
	AssetManager::DestroySingleton();
	RenderManager::DestroySingleton();
	FileManager::DestroySingleton();
	InputManager::DestroySingleton();
	TimeManager::DestroySingleton();
	DebugManager::DestroySingleton();
	ScriptManager::DestroySingleton();
}


void StandaloneApp::_InitializeGLFW()
{
	glfwSetErrorCallback( StandaloneApp::GlfwErrorCallback );
	if ( !glfwInit() )
	{
		exit( EXIT_FAILURE );
	}
}


void StandaloneApp::_ReadConfigFile()
{
	Char* fileData = FileManager::Singleton()->ReadFile( AppConfig::FILE_PATH_CONFIG );
	FileManager::Singleton()->Deserialize<AppConfig>( fileData, &_appConfig );

	// update managers
	TimeManager::Singleton()->SetDesiredFPS( _appConfig.fps );
}


void StandaloneApp::_CreateMainWindow()
{
	glfwDestroyWindow( _glfwWindow );

	if ( _appConfig.fullscreen )
	{
		const GLFWvidmode* mode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
		_appConfig.width = mode->width;
		_appConfig.height = mode->height;

		_glfwWindow = glfwCreateWindow( _appConfig.width, _appConfig.height,
										_appConfig.appName.GetChar(),
										glfwGetPrimaryMonitor(), NULL );
	}
	else
	{
		glfwWindowHint( GLFW_RESIZABLE, 0 ); // no resize
		_glfwWindow = glfwCreateWindow( _appConfig.width, _appConfig.height,
										_appConfig.appName.GetChar(),
										NULL, NULL );
	}
	if ( _glfwWindow == NULL )
    {
        glfwTerminate();
		_DestroyManagers();
        exit( EXIT_FAILURE );
    }

	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE ); 
	glfwSetWindowPos( _glfwWindow, _appConfig.posX, _appConfig.posY );

	glfwMakeContextCurrent( _glfwWindow );

	glfwSwapInterval( 0 );

	// add input listeners
	glfwSetKeyCallback			( _glfwWindow, InputReceiverGLFW::GLFW_KeyCallback );
	glfwSetMouseButtonCallback	( _glfwWindow, InputReceiverGLFW::GLFW_ButtonCallback );
	glfwSetCursorPosCallback	( _glfwWindow, InputReceiverGLFW::GLFW_CursorCallback );
	glfwSetScrollCallback		( _glfwWindow, InputReceiverGLFW::GLFW_ScrollCallback );
	glfwSetCursorEnterCallback	( _glfwWindow, InputReceiverGLFW::GLFW_FocusCallback );
}


void StandaloneApp::_InitializeRenderer()
{
	// it requires to be initialized after window is shown
	RenderManager* renderMngr = RenderManager::Singleton();

	_glContext = Memory::Alloc<GLContextImpl>( _glfwWindow );
	renderMngr->Initialize( _glContext, Memory::Alloc<GLImpl_3_0>() );

	UInt techSlot = renderMngr->NewTechnique( TechniqueType::FORWARD_BASIC );
	renderMngr->ChangeRenderer( 0, RendererType::GL_FORWARD );
	renderMngr->ChangeTechnique( 0, techSlot );

	_missingCamera = Memory::Alloc<GameObject>();
	_missingCamera->AddComponent<Transform>();
	_missingCamera->AddComponent<Camera>();
}


void StandaloneApp::_StartGame()
{
	GameInitScript* gameInit = Memory::Alloc<GameInitScript>( &_appConfig );
	gameInit->OnUpdate( TRUE );

	RenderManager::Singleton()->OnResize( 0 );
}


void StandaloneApp::_RenderScene()
{
	RenderManager* renderMngr = RenderManager::Singleton();
	SceneManager* sceneMngr = SceneManager::Singleton();

	GameObject* cameraGO = sceneMngr->GetMainCamera();
	if ( cameraGO != NULL )
	{
		Camera* camera = cameraGO->GetComponent<Camera>();
		Transform* transform = cameraGO->GetComponent<Transform>();

		if ( camera != NULL && camera->IsEnabled() &&
			 transform != NULL && transform->IsEnabled() )
		{
			//! update camera size
			Camera* cam = cameraGO->GetComponent<Camera>();
			Float aspectRatio = CAST_S( Float, _appConfig.width ) / _appConfig.height;
			Float height = cam->GetOrthoSize().y;
			cam->SetAspectRatio( aspectRatio );
			cam->SetOrthoWidth( aspectRatio * height );

			if ( sceneMngr->CountLoadedScenes() > 0 )
			{
				//for ( UInt i = 0; i < sceneMngr->CountLoadedScenes(); ++i )
				{
					Scene* scene = sceneMngr->GetScene();
					renderMngr->RenderScene( 0, scene, cameraGO );
				}

				return;
			}
		}
	}
	
	renderMngr->RenderScene( 0, NULL, _missingCamera );
}


void StandaloneApp::GlfwErrorCallback( Int code, const Char * desc )
{
	//char title[20];
	//sprintf(title, "GLFW error code %d", code);
}




