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

#include "Sections\MenuBar.h"
#include "Sections\Toolbar.h"

#include "Sections\Builders\ProjectBuilder.h"

#include "Sections\Panels\Panel_SceneHierarchy.h"
#include "Sections\Panels\Panel_Attributes.h"
#include "Sections\Panels\Panel_Assets.h"
#include "Sections\Panels\Panel_Console.h"
#include "Sections\Panels\Panel_PrefabHierarchy.h"
#include "Sections\Viewports\SceneViewport.h"
#include "Sections\Viewports\PrefabViewport.h"

#include "Sections\SelectorPopup.h"
#include "Sections\Settings\ProjectSettings.h"
#include "Sections\Serialize\JSON\JsonSerializer_EditorConfig.h"
#include "Sections\Serialize\JSON\JsonSerializer_CyredProj.h"

#include "EngineOverride\OpenGL\GLImpl_3_0.h"
#include "EngineOverride\OpenGL\GLContextImpl.h"
#include "EngineOverride\File\FileSystemWindows.h"
#include "EngineOverride\File\MeshLoaderWindows.h"
#include "EngineOverride\Input\InputReceiverQT.h"

#include "InternalAssets\FreeCameraControl.h"

#include <QtWidgets/QApplication>
#include <QtGui\qwindow.h>
#include "QtWidgets\qstatusbar.h"
#include <QtCore\qtime>
#include "QtWidgets\qtabbar.h"
#include "QtCore\qfileinfo.h"
#include "QtCore\qdir.h"


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


void EditorApp::Run( Int& argc, Char* argv[] )
{
	_qtApp = Memory::Alloc<QApplication>( argc, argv );

	_CreateManagers();
	_InitializeManagers();

	_ReadConfigFile();

	_CreateMainWindow();
	_CreateMenuBar();
	_CreateToolbar();
	_CreateStatusBar();
	_CreateCameras();
	_CreateSelectorPopup();

	// create panels
	for ( UInt i = 0; i < EditorSettings::panels.Size(); i++ ) {
		Panel* panel = NULL;
		if ( i == 0 ) {
			panel = _NewPanel( EditorSettings::panels[i].type, EditorSettings::panels[i].viewportIndex );
		}
		else {
			panel = _NewPanel( EditorSettings::panels[i].type, EditorSettings::panels[i].splitFrom,
								EditorSettings::panels[i].splitType, EditorSettings::panels[i].viewportIndex );

		}

		if ( EditorSettings::panels[i].viewportIndex == 0 ) {
			_mainViewport = CAST_S( Panel_Viewport*, panel );
		}
	}
	/*NewPanel( PanelType::ASSETS );
	NewPanel( PanelType::SCENE_HIERARCHY );
	NewPanel( PanelType::PREFAB_HIERARCHY, Qt::Vertical );
	_mainViewport = CAST_S( Panel_Viewport*, NewPanel( PanelType::SCENE_VIEWPORT ) );
	NewPanel( PanelType::PREFAB_VIEWPORT, Qt::Vertical, 1, FALSE );
	NewPanel( PanelType::CONSOLE, Qt::Vertical );
	NewPanel( PanelType::ATTRIBUTES );*/

	_skinStylesheet = NULL;
	// change skin after everything is created
	ApplySkin( EditorSettings::SKIN_DEFAULT_BLACK );


	// the window is first displayed after processing events
	// it is required for rendering to have displayed context
	// must be called after creating the main viewport
	_qtApp->processEvents();

	// it requires to be initialized after viewport is shown
	ASSERT( _mainViewport != NULL );
	RenderManager::Singleton()->Initialize( _mainViewport->GetGLContext(), Memory::Alloc<GLImpl_3_0>() );

	// load assets, after GL init
	if ( _panels.Has( PanelType::ASSETS ) ) {
		Panel_Assets* assetsPanel = CAST_S( Panel_Assets*, _panels.Get( PanelType::ASSETS ) );
		assetsPanel->ReloadAllAssets();
	}

	// must be called after all scenes assets are loaded
	_ReadProjectFile();

	// mark flags
	_isPlayMode = FALSE;
	_isPlayPaused = FALSE;
	_shouldStopPlay = FALSE;

	//! start the main loop
	_qtTime = Memory::Alloc<QTime>();
	_qtTime->start();

	_shouldExit = FALSE;

	while ( !_shouldExit ) { //! _shouldExit will be changed from other thread
		_UpdateLoop();
	}

	_CleanUp();
}


void EditorApp::Exit()
{
	// if play mode, exit play mode
	if ( _isPlayMode ) {
		StopPlayMode();
	}
	else {
		// exit editor
		_shouldExit = TRUE;
	}
}


void EditorApp::_CleanUp()
{
	_FinalizePanels();
	_FinalizeManagers();
	_DestroyManagers();

	Memory::Free( _inputReceiver );
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

	while ( timeManager->GetGameTime() < realGameTime ) {
		// time update
		timeManager->Update();

		//! game update
		_UpdateCameras();

		//! update scripts
		SceneManager* sceneManager = SceneManager::Singleton();
		UInt totalScenes = sceneManager->CountLoadedScenes();
		for ( UInt i = 0; i < totalScenes; ++i ) {
			sceneManager->GetScene( i )->OnUpdate( _isPlayMode && !_isPlayPaused );
		}
	}

	// stop play mode if needed
	if ( _shouldStopPlay ) {
		// disable play mode
		_isPlayMode = FALSE;
		_shouldStopPlay = FALSE;
		_toolbar->SetPlayButton( FALSE );

		// restore scenes
		SceneManager::Singleton()->RestoreScenes();
	}

	//! update panels
	Iterator<PanelType, Panel*> iter = _panels.GetIterator();
	while ( iter.HasNext() ) {
		iter.GetValue()->Update();
		iter.Next();
	}

	//! get and process events
	_qtApp->processEvents();
	InputManager::Singleton()->ProcessEvents();
}


Panel* EditorApp::_NewPanel( PanelType type, UInt viewportIndex )
{
	// new panel
	Panel* panel = NULL;

	switch ( type ) {
		case PanelType::SCENE_HIERARCHY:
			panel = Memory::Alloc<Panel_SceneHierarchy>();
			panel->Initialize();
			break;

		case PanelType::ATTRIBUTES:
			panel = Memory::Alloc<Panel_Attributes>();
			panel->Initialize();
			break;

		case PanelType::SCENE_VIEWPORT:
		{
			SceneViewport* viewportPanel = Memory::Alloc<SceneViewport>( viewportIndex );
			viewportPanel->Initialize( viewportIndex == 0 );
			viewportPanel->SetCamera( _cameras[ 0 ] );
			panel = viewportPanel;
			break;
		}

		case PanelType::PREFAB_VIEWPORT:
		{
			PrefabViewport* viewportPanel = Memory::Alloc<PrefabViewport>( viewportIndex );
			viewportPanel->Initialize( viewportIndex == 0 );
			viewportPanel->SetCamera( _cameras[ 1 ] );
			panel = viewportPanel;
			break;
		}

		case PanelType::ASSETS:
			panel = Memory::Alloc<Panel_Assets>();
			panel->Initialize();
			break;

		case PanelType::CONSOLE:
			panel = Memory::Alloc<Panel_Console>();
			panel->Initialize();
			break;

		case PanelType::PREFAB_HIERARCHY:
			panel = Memory::Alloc<Panel_PrefabHierarchy>();
			panel->Initialize();
			break;
	}

	// add to list
	_panels.Set( type, panel );
	// add to window
	_qtMainWindow->addDockWidget( Qt::LeftDockWidgetArea, panel );

	return panel;
}


Panel* EditorApp::_NewPanel( PanelType type, PanelType splitFrom, PanelSplitType splitType, 
							 UInt viewportIndex )
{
	// new panel
	Panel* panel = NULL;
	// get split from
	ASSERT( _panels.Has( splitFrom ) );
	Panel* panelFrom = _panels.Get( splitFrom );

	switch ( type ) {
		case PanelType::SCENE_HIERARCHY:
			panel = Memory::Alloc<Panel_SceneHierarchy>();
			panel->Initialize();
			break;

		case PanelType::ATTRIBUTES:
			panel = Memory::Alloc<Panel_Attributes>();
			panel->Initialize();
			break;

		case PanelType::SCENE_VIEWPORT:
		{
			SceneViewport* viewportPanel = Memory::Alloc<SceneViewport>( viewportIndex );
			viewportPanel->Initialize( (viewportIndex == 0) );
			viewportPanel->SetCamera( _cameras[ 0 ] );
			panel = viewportPanel;
			break;
		}

		case PanelType::PREFAB_VIEWPORT:
		{
			PrefabViewport* viewportPanel = Memory::Alloc<PrefabViewport>( viewportIndex );
			viewportPanel->Initialize( (viewportIndex == 0) );
			viewportPanel->SetCamera( _cameras[ 1 ] );
			panel = viewportPanel;
			break;
		}

		case PanelType::ASSETS:
			panel = Memory::Alloc<Panel_Assets>();
			panel->Initialize();
			break;

		case PanelType::CONSOLE:
			panel = Memory::Alloc<Panel_Console>();
			panel->Initialize();
			break;

		case PanelType::PREFAB_HIERARCHY:
			panel = Memory::Alloc<Panel_PrefabHierarchy>();
			panel->Initialize();
			break;
	}

	// add to list
	_panels.Set( type, panel );

	// add to window
	switch ( splitType ) {
		case PanelSplitType::HORIZONTAL:
			_qtMainWindow->splitDockWidget( panelFrom, panel, Qt::Horizontal );
			break;

		case PanelSplitType::VERTICAL:
			_qtMainWindow->splitDockWidget( panelFrom, panel, Qt::Vertical );
			break;
	}

	return panel;
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
	ScriptManager::CreateSingleton();
	ProjectBuilder::CreateSingleton();

	Random::Initialize();
}


void EditorApp::_InitializeManagers()
{
	_inputReceiver = Memory::Alloc<InputReceiverQT>();

	EventManager::Singleton()->Initialize();
	SceneManager::Singleton()->Initialize();
	ScriptManager::Singleton()->Initialize();
	AssetManager::Singleton()->Initialize();

	JsonSerializeSystem* jsonSystem = Memory::Alloc<JsonSerializeSystem>();
	jsonSystem->AddSerializer<EditorSettings>( Memory::Alloc<JsonSerializer_EditorConfig>() );
	jsonSystem->AddSerializer<ProjectSettings>( Memory::Alloc<JsonSerializer_CyredProj>() );

	FileManager::Singleton()->Initialize( Memory::Alloc<FileSystemWindows>(),
										  Memory::Alloc<MeshLoaderWindows>() );
	FileManager::Singleton()->SetSerializeSystem( jsonSystem );

	InputManager::Singleton()->Initialize( _inputReceiver );
	TimeManager::Singleton()->Initialize( EditorSettings::fps );
	DebugManager::Singleton()->Initialize();

	ProjectBuilder::Singleton()->Initialize();
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
	ScriptManager::Singleton()->Finalize();

	ProjectBuilder::Singleton()->Finalize();
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
	ScriptManager::DestroySingleton();

	ProjectBuilder::DestroySingleton();
}


void EditorApp::_FinalizePanels()
{
	Iterator<PanelType, Panel*> iter = _panels.GetIterator();
	while ( iter.HasNext() ) {
		iter.GetValue()->Finalize();
		iter.Next();
	}
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

	if ( EditorSettings::fullscreen ) {
		_qtMainWindow->showMaximized();
	}
	else {
		_qtMainWindow->showNormal();
	}
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
	// create status bar
	_qtStatusBar = Memory::Alloc<QStatusBar>();

	// add to window
	_qtMainWindow->setStatusBar( _qtStatusBar );

	// show first status
	ShowStatus( EditorSettings::MSG_EDITOR_STARTED );
}


void EditorApp::_CreateToolbar()
{
	_toolbar = Memory::Alloc<Toolbar>();
	_qtMainWindow->addToolBar( Qt::TopToolBarArea, _toolbar );
}


void EditorApp::_CreateSelectorPopup()
{
	_selectorPopup = Memory::Alloc<SelectorPopup>( _qtMainWindow );
	_selectorPopup->Initialize();
}


void EditorApp::_CreateCameras()
{
	for ( UInt i = 0; i < 2; i++ ) {
		GameObject* cameraGO = Memory::Alloc<GameObject>();
		cameraGO->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 10) );

		Camera* cameraComp1 = cameraGO->AddComponent<Camera>();
		cameraComp1->SetFovYAngle( 60 );
		cameraComp1->SetNearClipping( 0.1f );
		cameraComp1->SetFarClipping( 200.0f );

		FreeCameraControl* freeCamera = cameraGO->AddComponent<FreeCameraControl>();
		freeCamera->panSpeed	= 1.5f;
		freeCamera->rotateSpeed = 8.0f;
		freeCamera->zoomSpeed	= 80.0f;
		freeCamera->myWindows.Set( 0, TRUE );

		_cameras.Add( cameraGO );
	}
}


void EditorApp::_UpdateCameras()
{
	for ( UInt i = 0; i < _cameras.Size(); i++ ) {
		FreeCameraControl* freeCamera = _cameras[i]->GetComponent<FreeCameraControl>();
		freeCamera->OnUpdate( _isPlayMode );
	}
}


void EditorApp::_ReadConfigFile()
{
	Char* fileData = FileManager::Singleton()->ReadFile( EditorSettings::FILE_PATH_CONFIG );
	FileManager::Singleton()->Deserialize<EditorSettings>( fileData, NULL );

	QDir dir;
	QFileInfo fileInfo( EditorSettings::projectPath.GetChar() );

	QString projDir = dir.relativeFilePath( fileInfo.absolutePath() );
	projDir.append( "/" ).append( FileManager::DIR_ASSETS );

	ProjectSettings::dirPathAssets = projDir.toUtf8().constData();
}


void EditorApp::_ReadProjectFile()
{
	Char* fileData = FileManager::Singleton()->ReadFile( EditorSettings::projectPath.GetChar() );
	FileManager::Singleton()->Deserialize<ProjectSettings>( fileData, NULL );
}


void EditorApp::ShowStatus( const Char* message )
{
	_qtStatusBar->showMessage( message, 0 );
}


// the skin must exist in skins directory
void EditorApp::ApplySkin( const Char* skinName )
{
	FiniteString filePath( "%s%s%s", EditorSettings::DIR_PATH_SKINS,
									 skinName,
									 EditorSettings::FILE_FORMAT_SKINS );

	Char* stylesheet = FileManager::Singleton()->ReadFile( filePath.GetChar() );

	// apply skin to all elements
	if ( stylesheet != NULL )
	{
		// remove old skin
		Memory::FreeArray( _skinStylesheet );

		_skinStylesheet = stylesheet;

		_qtMainWindow->setStyleSheet( _skinStylesheet );
	}
}


void EditorApp::StartPlayMode()
{
	// ignore if already in play mode
	if ( _isPlayMode ) {
		return;
	}

	// activate play mode
	_isPlayMode = TRUE;
	_toolbar->SetPlayButton( TRUE );

	// no stopping needed yet
	_shouldStopPlay = FALSE;

	// store current scenes
	SceneManager::Singleton()->StoreScenes();
}


void EditorApp::StopPlayMode()
{
	// ignore if not in play mode
	if ( !_isPlayMode ) {
		return;
	}

	// do not stop play directly, but mark for stop
	_shouldStopPlay = TRUE;
}


void EditorApp::SetPlayPaused( Bool value )
{
	// pause / unpause play mode
	_isPlayPaused = value;
	_toolbar->SetPauseButton( value );
}


void EditorApp::ShowSelectorPopup( const Char* type, void* qtSelectorPtr )
{
	_selectorPopup->DisplayPopup( type, qtSelectorPtr );
}


InputReceiverQT* EditorApp::GetInputReceiver()
{
	ASSERT( _inputReceiver != NULL );
	return _inputReceiver;
}
