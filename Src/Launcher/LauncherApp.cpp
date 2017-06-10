// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "LauncherApp.h"
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
#include "CyredModule_Physics.h"

#include "EngineOverride\OpenGL\GLImpl_3_0.h"
#include "EngineOverride\OpenGL\GLContextImpl.h"
#include "EngineOverride\Input\InputReceiverGLFW.h"

#include "Sections\GameInitScript.h"
#include "Sections\Console.h"

#include "GLEW\Include\glew.h"
#include "GLFW\Include\glfw3.h"


using namespace CYRED;


DEFINE_REMOTE_SINGLETON_IMPL( LauncherApp )


void LauncherApp::Run( int& argc, char* argv[] )
{
	_InitializeGLFW();

	_CreateManagers();
	_InitializeManagers();

	// init console
	_console = new Console();

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


void LauncherApp::Exit()
{
	_shouldExit = TRUE;
}


void LauncherApp::_CleanUp()
{
	_FinalizeManagers();
	_DestroyManagers();

	PTR_FREE( _glContext );

	// destroy input receiver
	InputReceiverGLFW::DestroySingleton();

	glfwDestroyWindow( _glfwWindow );
    glfwTerminate();
}


void LauncherApp::_UpdateLoop()
{
	TimeManager* timeManager = TimeManager::Singleton();

	double realGameTime = glfwGetTime();
	timeManager->RenderUpdate( realGameTime );

	while ( timeManager->GetGameTime() < realGameTime ) {
		// time update
		timeManager->Update();

		// update scripts
		SceneManager::Singleton()->Update( TRUE );
	}

	// update physics
	PhysicsManager::Singleton()->Update();

	// render
	_RenderScene();

	//! get and process events
	glfwPollEvents();
	// check for close button
	_shouldExit |= ( glfwWindowShouldClose( _glfwWindow ) == 1 );

	// process input
	InputManager::Singleton()->ProcessEvents();

	// process engine events after editor events
	EventManager::Singleton()->Update();
}


void LauncherApp::_CreateManagers()
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
	PhysicsManager::CreateSingleton();

	Random::Initialize();
}


void LauncherApp::_InitializeManagers()
{
	EventManager::Singleton()->Initialize();
	SceneManager::Singleton()->Initialize();
	AssetManager::Singleton()->Initialize();
	PhysicsManager::Singleton()->Initialize();

	SerializeSystem* serializeSystem = new JsonSerializeSystem();
	
	
	FileManager::Singleton()->Initialize( new FileSystem(),
										  new MeshLoader() );
	FileManager::Singleton()->SetSerializeSystem( serializeSystem );

	// also create input receiver
	InputReceiverGLFW::CreateSingleton();
	InputManager::Singleton()->Initialize( InputReceiverGLFW::Singleton() );

	TimeManager::Singleton()->Initialize( 0 );
	DebugManager::Singleton()->Initialize();
	ScriptManager::Singleton()->Initialize();
}


void LauncherApp::_FinalizeManagers()
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
	PhysicsManager::Singleton()->Finalize();
}


void LauncherApp::_DestroyManagers()
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
	PhysicsManager::DestroySingleton();
}


void LauncherApp::_InitializeGLFW()
{
	glfwSetErrorCallback( LauncherApp::GlfwErrorCallback );
	if ( !glfwInit() )
	{
		exit( EXIT_FAILURE );
	}
}


void LauncherApp::_ReadConfigFile()
{
	char* fileData = FileManager::Singleton()->ReadFile( AppConfig::FILE_PATH_CONFIG );
	FileManager::Singleton()->Deserialize<AppConfig>( fileData, &_appConfig );

	// update managers
	TimeManager::Singleton()->SetDesiredFPS( _appConfig.fps );
}


void LauncherApp::_CreateMainWindow()
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


void LauncherApp::_InitializeRenderer()
{
	// it requires to be initialized after window is shown
	RenderManager* renderMngr = RenderManager::Singleton();

	_glContext = new GLContextImpl( _glfwWindow );
	renderMngr->Initialize( _glContext, new GLImpl_3_0() );

	// create randerer
	renderMngr->SwitchCanvas( 0 );
	renderMngr->CreateRenderer( RendererType::GL_FORWARD );
}


void LauncherApp::_StartGame()
{
	GameInitScript* gameInit = new GameInitScript( &_appConfig );
	gameInit->OnUpdate( TRUE );

	RenderManager::Singleton()->OnResize( 0 );
}


void LauncherApp::_RenderScene()
{
	RenderManager* renderMngr = RenderManager::Singleton();
	SceneManager* sceneMngr = SceneManager::Singleton();

	// clear screen
	renderMngr->ClearScreen( 0, 0, 0 );

	// render
	if ( sceneMngr->CountLoadedScenes() > 0 ) {
		// get first scene's root
		Node* sceneRoot = SceneManager::Singleton()->GetScene()->GetRoot();

		// get main camera
		GameObject* cameraGO = sceneMngr->GetMainCamera();
		if ( cameraGO == NULL ) {
			// finish
			renderMngr->SwapBuffers();
			return;
		}

		Camera*		camera		= cameraGO->GetComponent<Camera>();
		Transform*	cameraTran	= cameraGO->GetComponent<Transform>();
		if ( camera == NULL || cameraTran == NULL ) {
			// finish
			renderMngr->SwapBuffers();
			return;
		}

		// all good 

		// set camera
		renderMngr->SwitchCamera( cameraTran, camera );

		// update camera size
		bool emitEvents = camera->DoesEmitEvents();
		camera->SetEmitEvents( FALSE );
		{
			float aspectRatio = CAST_S( float, _appConfig.width ) / _appConfig.height;
			float height = camera->GetOrthoSize().y;
			camera->SetAspectRatio( aspectRatio );
			camera->SetOrthoWidth( aspectRatio * height );
		}
		camera->SetEmitEvents( emitEvents );

		
		// collect layers
		DataArray<int> layers;
		for ( int i = 0; i < sceneRoot->GetChildNodeCount(); i++ ) {
			_RecCollectLayers( CAST_S( GameObject*, sceneRoot->GetChildNodeAt( i ) ), layers );
		}

		// collect lights
		DataArray<GameObject*> lightsGO;
		_RecCollectLights( sceneRoot, lightsGO );

		// render by layers
		for ( int i = 0; i < layers.Size(); i++ ) {
			// render meshes
			renderMngr->Render( layers[i], ComponentType::MESH_RENDERING, sceneRoot, lightsGO );
			// render morphs
			renderMngr->Render( layers[i], ComponentType::MORPH_RENDERING, sceneRoot, lightsGO );
			// render text 3d
			renderMngr->Render( layers[i], ComponentType::TEXT_3D, sceneRoot, lightsGO );
			// render particles
			renderMngr->Render( layers[i], ComponentType::PARTICLE_EMITTER, sceneRoot, lightsGO );

			// reset depth
			renderMngr->ResetDepth();
		}
	}

	// finish
	renderMngr->SwapBuffers();
}


void LauncherApp::_RecCollectLights( Node* root, DataArray<GameObject*>& lightsGO )
{
	if ( root == NULL ) {
		return;
	}

	// check if gameobject
	GameObject* gameObject = CAST_D( GameObject*, root );
	if ( gameObject != NULL ) {
		// check for light component
		Light* light = gameObject->GetComponent<Light>();
		if ( light != NULL ) {
			lightsGO.Add( gameObject );
		}
	}

	// parse children
	for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
		_RecCollectLights(root->GetChildNodeAt(i), lightsGO );
	}
}


void LauncherApp::_RecCollectLayers( GameObject* root, OUT DataArray<int>& layers )
{
	if ( root == NULL ) {
		return;
	}

	// get layer
	int layer = root->GetLayer();
	// ignore negative numbers
	if ( layer >= 0 ) {
		// if layer does not exist, add it, keep the list ordered
		bool found = false;
		for ( int i = 0; i < layers.Size(); i++ ) {
			if ( layer == layers[i] ) {
				// found
				found = true;
				break;
			}
			else if ( layer < layers[i] ) {
				// insert before this
				layers.Insert( i, layer );
				found = true;
				break;
			}
		}

		// if not found, just add it
		if ( !found ) {
			layers.Add( layer );
		}
	}

	// parse children
	for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
		_RecCollectLayers( CAST_S(GameObject*, root->GetChildNodeAt(i)), layers );
	}
}


void LauncherApp::GlfwErrorCallback( int code, cchar * desc )
{
	//char title[20];
	//sprintf(title, "GLFW error code %d", code);
}




