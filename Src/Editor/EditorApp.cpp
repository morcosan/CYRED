// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "EditorApp.h"

#include "CyredModule_Asset.h"
#include "CyredModule_Debug.h"
#include "CyredModule_Event.h"
#include "CyredModule_File.h"
#include "CyredModule_Input.h"
#include "CyredModule_Render.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Script.h"
#include "CyredModule_Time.h"

#include "Fragments\MenuBar.h"

#include "Fragments\Panels\HierarchyPanel.h"
#include "Fragments\Panels\AttributePanel.h"
#include "Fragments\Panels\ViewportPanel.h"
#include "Fragments\Panels\AssetsPanel.h"

#include "Fragments\Viewports\SceneViewport.h"

#include "Fragments\SelectorPopup.h"

#include "EngineOverride\OpenGL\GLImpl_3_0.h"
#include "EngineOverride\OpenGL\GLContextImpl.h"
#include "EngineOverride\File\FileSystemWindows.h"
#include "EngineOverride\Input\InputReceiverWindows.h"
#include "EngineOverride\Debug\ConsoleWindows.h"

#include "InternalAssets\FreeCameraScript.h"

#include "Fragments\EditorSettings.h"

#include <QtWidgets/QApplication>
#include <QtGui\qwindow.h>
#include "QtWidgets\qstatusbar.h"
#include <QtCore\qtime>
#include "QtWidgets\qtabbar.h"


using namespace CYRED;



class EditorApp::_QtMainWindow : public QMainWindow
{
public:
	_QtMainWindow( EditorApp* appImpl )
	{
		_appImpl = appImpl;
	}

	virtual void closeEvent( QCloseEvent *event )
	{
		_appImpl->Exit();
	}

protected:
	EditorApp* _appImpl;
};


class EditorApp::_QtWindow : public QWindow
{
public:

};


DEFINE_REMOTE_SINGLETON_IMPL( EditorApp )


void EditorApp::Run( Int& argc, Char* argv[], Char* configFilePath )
{
	_Run( argc, argv );
}


void EditorApp::_Run( Int& argc, Char* argv[] )
{
	_qtApp = Memory::Alloc<QApplication>( argc, argv );

	_CreateMainWindow();
	_CreateManagers();
	_InitializeManagers();
	_CreateMenuBar();
	_CreateStatusBar();
	_CreateCameras();
	_CreateSelectorPopup();

	// create some panels
	NewPanel( PanelType::ASSETS );
	NewPanel( PanelType::HIERARCHY );
	NewPanel( PanelType::SCENE_VIEWPORT );
	NewPanel( PanelType::ATTRIBUTES );

	// the window is first displayed after processing events
	// it is required for rendering to have displayed context
	// must be called after creating the main viewport
	_qtApp->processEvents();
	_shouldExit = FALSE;

	// it requires to be initialized after viewport is shown
	RenderManager::Singleton()->Initialize( _viewports[0]->GetGLContext(), 
											   Memory::Alloc<NotAPI::GLImpl_3_0>() );

	// load assets
	for ( UInt i = 0; i < _normalPanels.Size(); ++i )
	{
		AssetsPanel* assetsPanel = CAST_D( AssetsPanel*, _normalPanels[i] );
		if ( assetsPanel != NULL )
		{
			assetsPanel->ReloadAllAssets();
		}
	}

	_isPlayMode = FALSE;

	_skinStylesheet = NULL;
	// change skin after everything is created
	ApplySkin( EditorSettings::SKIN_DEFAULT_BLACK );


	//! start the main loop
	_qtTime = Memory::Alloc<QTime>();
	_qtTime->start();

	while ( !_shouldExit ) //! _shouldExit will be changed from other thread
	{
		_UpdateLoop();
	}

	_CleanUp();
}


void EditorApp::Exit()
{
	_shouldExit = TRUE;
}


void EditorApp::_CleanUp()
{
	_FinalizeManagers();
	_DestroyManagers();

	Memory::Free( _selectorPopup );
	Memory::Free( _qtTime );
	Memory::Free( _qtMainWindow );
	Memory::Free( _qtApp );
}


void EditorApp::_UpdateLoop()
{
	TimeManager* timeManager = TimeManager::Singleton();

	Double realGameTime = CAST_S( Double, _qtTime->elapsed() ) / 1000;
	timeManager->RenderUpdate( realGameTime );

	while ( timeManager->GetGameTime() < realGameTime )
	{
		timeManager->Update();

		//! game update
		{
			_UpdateCameras();

			//! update scripts
			SceneManager* sceneManager = SceneManager::Singleton();
			UInt totalScenes = sceneManager->CountLoadedScenes();
			for ( UInt i = 0; i < totalScenes; ++i )
			{
				sceneManager->GetScene( i )->OnStart( _isPlayMode );
				sceneManager->GetScene( i )->OnUpdate( _isPlayMode );
			}
		}
	}

	//! render frame
	for ( UInt i = 0; i < _viewports.Size(); ++i )
	{
		_viewports[ i ]->Update();
	}

	//! get and process events
	_qtApp->processEvents();
	InputManager::Singleton()->ProcessEvents();
}


void EditorApp::_CreateManagers()
{
	SceneManager::CreateSingleton();
	RenderManager::CreateSingleton();
	EventManager::CreateSingleton();
	AssetManager::CreateSingleton();
	FileManager::CreateSingleton();
	InputManager::CreateSingleton();
	TimeManager::CreateSingleton();
	DebugManager::CreateSingleton();

	Random::Initialize();
}


void EditorApp::_InitializeManagers()
{
	_inputReceiver = Memory::Alloc<InputReceiverWindows>();

	EventManager::Singleton()->Initialize();
	SceneManager::Singleton()->Initialize();
	AssetManager::Singleton()->Initialize();

	FileManager::Singleton()->Initialize( Memory::Alloc<FileSystemWindows>() );
	FileManager::Singleton()->SetSerializeSystem( Memory::Alloc<JsonSerializeSystem>() );

	InputManager::Singleton()->Initialize( _inputReceiver );
	TimeManager::Singleton()->Initialize( EditorSettings::fps );
	DebugManager::Singleton()->Initialize( Memory::Alloc<ConsoleWindows>() );
}


void EditorApp::_FinalizeManagers()
{
	SceneManager::Singleton()->Finalize();
	EventManager::Singleton()->Finalize();
	AssetManager::Singleton()->Finalize();
	RenderManager::Singleton()->Finalize();
	FileManager::Singleton()->Finalize();
	InputManager::Singleton()->Finalize();
	TimeManager::Singleton()->Finalize();
	DebugManager::Singleton()->Finalize();
}


void EditorApp::_DestroyManagers()
{
	SceneManager::DestroySingleton();
	EventManager::DestroySingleton();
	AssetManager::DestroySingleton();
	RenderManager::DestroySingleton();
	FileManager::DestroySingleton();
	InputManager::DestroySingleton();
	TimeManager::DestroySingleton();
	DebugManager::DestroySingleton();
}


void EditorApp::_CreateMainWindow()
{
	_qtMainWindow = Memory::Alloc<_QtMainWindow>( this );
	_qtMainWindow->resize( EditorSettings::width, EditorSettings::height );
	_qtMainWindow->move( EditorSettings::posX, EditorSettings::posY );
	_qtMainWindow->setWindowTitle( EditorSettings::EDITOR_NAME );
	_qtMainWindow->setDockOptions( QMainWindow::AnimatedDocks | 
								   QMainWindow::AllowNestedDocks | 
								   QMainWindow::AllowTabbedDocks );
	_qtMainWindow->setTabPosition( Qt::AllDockWidgetAreas, QTabWidget::TabPosition::North );
	_qtMainWindow->show();
}


void EditorApp::_CreateMenuBar()
{
	_menuBar = new MenuBar();
	_menuBar->Initialize();

	// add editor skins
	DataArray<const Char*> skinsName;
	skinsName.Add( EditorSettings::SKIN_DEFAULT_BLACK );
	skinsName.Add( EditorSettings::SKIN_DEFAULT_WHITE );
	_menuBar->AddSkins( skinsName, 0 );

	_qtMainWindow->setMenuBar( _menuBar );
}


void EditorApp::_CreateStatusBar()
{
	_qtStatusBar = new QStatusBar();

	_qtMainWindow->setStatusBar( _qtStatusBar );

	ShowStatus( EditorSettings::MSG_EDITOR_STARTED );
}


void EditorApp::_CreateSelectorPopup()
{
	_selectorPopup = Memory::Alloc<SelectorPopup>( _qtMainWindow );
	_selectorPopup->Initialize();
}


void EditorApp::_CreateCameras()
{
	GameObject* cameraGO1 = Memory::Alloc<GameObject>();
	cameraGO1->AddComponent<COMP::Transform>()->SetPositionWorld( Vector3(0, 0, 10) );

	COMP::Camera* cameraComp1 = cameraGO1->AddComponent<COMP::Camera>();
	cameraComp1->SetFovYAngle( 60 );
	cameraComp1->SetNearClipping( 0.1f );
	cameraComp1->SetFarClipping( 200.0f );

	FreeCameraScript* script = cameraGO1->AddComponent<FreeCameraScript>();
	script->panSpeed	= 1.5f;
	script->rotateSpeed = 8.0f;
	script->zoomSpeed	= 80.0f;
	script->myWindows.Set( 0, TRUE );

	_cameras.Add( cameraGO1 );
}


void EditorApp::_UpdateCameras()
{
	FreeCameraScript* script = _cameras[ 0 ]->GetComponent<FreeCameraScript>();

	if ( script->DoesNeedStart() )
	{
		script->Start();
	}

	script->Update();
}


void EditorApp::NewPanel( PanelType type, UInt panelIndex, Bool isPrimary )
{
	Panel* panel = NULL;
	
	switch ( type )
	{
		case PanelType::HIERARCHY:
			{
				panel = Memory::Alloc<HierarchyPanel>();
				panel->Initialize();
				_normalPanels.Add( panel );
			}
			break;

		case PanelType::ATTRIBUTES:
			{
				AttributePanel* attrPanel = Memory::Alloc<AttributePanel>();
				panel = attrPanel;
				panel->Initialize();
				_normalPanels.Add( panel );
			}
			break;

		case PanelType::SCENE_VIEWPORT:
			{
				SceneViewport* viewportPanel = Memory::Alloc<SceneViewport>( panelIndex );
				viewportPanel->Initialize( isPrimary );
				viewportPanel->SetCamera( _cameras[ 0 ] );

				_viewports.Add( viewportPanel );
				panel = viewportPanel;
			}
			break;

		case PanelType::ASSETS:
			{
				panel = Memory::Alloc<AssetsPanel>();
				panel->Initialize();
				_normalPanels.Add( panel );
			}
			break;
	}

	_qtMainWindow->addDockWidget( Qt::LeftDockWidgetArea, panel, Qt::Orientation::Horizontal );
}


void EditorApp::ShowStatus( const Char* message )
{
	_qtStatusBar->showMessage( message, 0 );
}


// the skin must exist in skins directory
void EditorApp::ApplySkin( const Char* skinName )
{
	Char filePath[MAX_SIZE_CUSTOM_STRING];
	CUSTOM_STRING( filePath, "%s%s%s", EditorSettings::DIR_PATH_SKINS, 
									   skinName, 
									   EditorSettings::FILE_FORMAT_SKINS );

	Char* stylesheet = FileManager::Singleton()->ReadFile( filePath );

	// apply skin to all elements
	if ( stylesheet != NULL )
	{
		// remove old skin
		Memory::FreeArray( _skinStylesheet );

		_skinStylesheet = stylesheet;

		_qtMainWindow->setStyleSheet( _skinStylesheet );
	}
}


void EditorApp::ShowSelectorPopup( const Char* type, void* qtSelectorPtr )
{
	_selectorPopup->DisplayPopup( type, qtSelectorPtr );
}


InputReceiverWindows* EditorApp::GetInputReceiver()
{
	ASSERT( _inputReceiver != NULL );
	return _inputReceiver;
}
