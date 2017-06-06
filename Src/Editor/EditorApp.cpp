// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
#include "CyredModule_Physics.h"

#include "Sections\MenuBar.h"
#include "Sections\Toolbar.h"
#include "Sections\Builders\ProjectBuilder.h"
#include "Sections\Panels\Panel_Attributes.h"
#include "Sections\Panels\Panel_Assets.h"
#include "Sections\Panels\Panel_Console.h"
#include "Sections\Panels\Hierarchies\Hierarchy_Scene.h"
#include "Sections\Panels\Hierarchies\Hierarchy_Prefab.h"
#include "Sections\Panels\Hierarchies\Hierarchy_Isolate.h"
#include "Sections\Panels\Viewports\Viewport_Scene.h"
#include "Sections\Panels\Viewports\Viewport_Prefab.h"
#include "Sections\Panels\Viewports\Viewport_Game.h"
#include "Sections\Panels\Viewports\Viewport_Isolate.h"
#include "Sections\SelectorPopup.h"
#include "Sections\Settings\ProjectSettings.h"
#include "Sections\Serialize\JSON\JsonSerializer_EditorConfig.h"
#include "Sections\Serialize\JSON\JsonSerializer_CyredProj.h"

#include "Utils\EditorUtils.h"

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
#include "QtGui\qevent.h"


using namespace CYRED;



class EditorApp::_QtMainWindow : public QMainWindow
{
public:
	_QtMainWindow( EditorApp* appImpl )
	{
		_appImpl = appImpl;
	}

	virtual void closeEvent( QCloseEvent* event )
	{
		// call for exit
		_appImpl->Exit();
		// ignore qt close
		event->ignore();
	}

private:
	EditorApp* _appImpl;
};


class EditorApp::_QtWindow : public QWindow
{
public:

};


DEFINE_REMOTE_SINGLETON_IMPL( EditorApp )


void EditorApp::Run( int& argc, char* argv[] )
{
	_qtApp = new QApplication( argc, argv );

	_CreateManagers();
	_InitializeManagers();

	_ReadConfigFile();

	_CreateMainWindow();
	_CreateMenuBar();
	_CreateToolbar();
	_CreateStatusBar();
	_CreateCameras();
	_CreateSelectorPopup();


	// a viewport is needed t initialize rendering
	Panel_Viewport*	mainViewport = NULL;

	// create panels
	for ( int i = 0; i < EditorSettings::panels.Size(); i++ ) {
		Panel* panel = NULL;
		if ( i == 0 ) {
			panel = _NewPanel( EditorSettings::panels[i].type, EditorSettings::panels[i].viewportIndex );
		}
		else {
			panel = _NewPanel( EditorSettings::panels[i].type, EditorSettings::panels[i].splitFrom,
								EditorSettings::panels[i].splitType, EditorSettings::panels[i].viewportIndex );

		}

		// resize
		if ( EditorSettings::panels[i].width > 0 ) {
			panel->setMaximumWidth( EditorSettings::panels[i].width );
		}
		if ( EditorSettings::panels[i].height > 0 ) {
			panel->setMaximumHeight( EditorSettings::panels[i].height );
		}

		if ( EditorSettings::panels[i].viewportIndex == 0 ) {
			mainViewport = CAST_S( Panel_Viewport*, panel );
		}
	}

	// change skin after everything is created
	_skinStylesheet = NULL;
	ApplySkin( EditorSettings::SKIN_DEFAULT_BLACK );


	// the window is first displayed after processing events
	// it is required for rendering to have displayed context
	// must be called after creating the main viewport
	_qtApp->processEvents();


	// it requires to be initialized after viewport is shown
	ASSERT( mainViewport != NULL );
	RenderManager::Singleton()->Initialize( mainViewport->GetGLContext(), new GLImpl_3_0() );

	// load assets, after GL init
	if ( _panels.Has( PanelType::ASSETS ) ) {
		Panel_Assets* assetsPanel = CAST_S( Panel_Assets*, _panels.Get( PanelType::ASSETS ) );
		assetsPanel->ReloadAllAssets();
	}

	// load gizmo and refresh panels
	Iterator<PanelType, Panel*> iter = _panels.GetIterator();
	while ( iter.HasNext() ) {
		Panel* panel = iter.GetValue();
		Panel_Viewport* viewportPanel = CAST_D( Panel_Viewport*, panel );
		if ( viewportPanel != NULL ) {
			viewportPanel->LoadGizmo();
		}

		// refresh max size
		panel->setMaximumSize( 999999, 999999 );

		// next
		iter.Next();
	}

	// must be called after all scenes assets are loaded
	_ReadProjectFile();

	// mark flags
	_isPlayMode = FALSE;
	_isPlayPaused = FALSE;
	_shouldStopPlay = FALSE;

	//! start the main loop
	_qtTime = new QTime();
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
	_WriteConfigFile();

	_FinalizePanels();
	_FinalizeManagers();
	_DestroyManagers();

	PTR_FREE( _inputReceiver );
	PTR_FREE( _selectorPopup );
	PTR_FREE( _qtTime );
	PTR_FREE( _qtMainWindow );
	PTR_FREE( _qtApp );
}


void EditorApp::_UpdateLoop()
{
	TimeManager* timeManager = TimeManager::Singleton();

	double realGameTime = CAST_S( double, _qtTime->elapsed() ) / 1000;
	timeManager->RenderUpdate( realGameTime );

	while ( timeManager->GetGameTime() < realGameTime ) {
		// time update
		timeManager->Update();

		// update cameras
		_UpdateCameras();

		// update scripts
		SceneManager::Singleton()->Update( _isPlayMode && !_isPlayPaused );

		// update physics
		if ( _isPlayMode && !_isPlayPaused ) {
			PhysicsManager::Singleton()->Update();
		}
	}

	// update panels
	Iterator<PanelType, Panel*> iter = _panels.GetIterator();
	while ( iter.HasNext() ) {
		iter.GetValue()->Update( _isPlayMode && !_isPlayPaused );
		iter.Next();
	}


	// stop play mode if needed
	if ( _shouldStopPlay ) {
		// disable play mode
		_isPlayMode = FALSE;
		_shouldStopPlay = FALSE;
		_toolbar->SetPlayButton( FALSE );

		// restore scenes
		SceneManager::Singleton()->RestoreScenes();

		// announce stop
		EventManager::Singleton()->EmitEvent( EditorEventType::PLAY_MODE_STOP, NULL );
	}


	// get and process events
	_qtApp->processEvents();
	InputManager::Singleton()->ProcessEvents();
}


Panel* EditorApp::_NewPanel( PanelType type, int viewportIndex )
{
	// new panel
	Panel* panel = NULL;

	switch ( type ) {
		case PanelType::SCENE_HIERARCHY:
			panel = new Hierarchy_Scene();
			panel->Initialize();
			break;

		case PanelType::ATTRIBUTES:
			panel = new Panel_Attributes();
			panel->Initialize();
			break;

		case PanelType::SCENE_VIEWPORT:
		{
			Viewport_Scene* viewportPanel = new Viewport_Scene( viewportIndex );
			viewportPanel->Initialize( viewportIndex == 0 );
			viewportPanel->SetCamera( _cameras[ viewportIndex ] );
			panel = viewportPanel;
			break;
		}

		case PanelType::PREFAB_VIEWPORT:
		{
			Viewport_Prefab* viewportPanel = new Viewport_Prefab( viewportIndex );
			viewportPanel->Initialize( viewportIndex == 0 );
			viewportPanel->SetCamera( _cameras[ viewportIndex ] );
			panel = viewportPanel;
			break;
		}

		case PanelType::ISOLATE_VIEWPORT:
		{
			Viewport_Isolate* viewportPanel = new Viewport_Isolate( viewportIndex );
			viewportPanel->Initialize( viewportIndex == 0 );
			viewportPanel->SetCamera( _cameras[ viewportIndex ] );
			panel = viewportPanel;
			break;
		}

		case PanelType::ASSETS:
			panel = new Panel_Assets();
			panel->Initialize();
			break;

		case PanelType::CONSOLE:
			panel = new Panel_Console();
			panel->Initialize();
			break;

		case PanelType::PREFAB_HIERARCHY:
			panel = new Hierarchy_Prefab();
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
							 int viewportIndex )
{
	// new panel
	Panel* panel = NULL;
	// get split from
	ASSERT( _panels.Has( splitFrom ) );
	Panel* panelFrom = _panels.Get( splitFrom );

	switch ( type ) {
		case PanelType::SCENE_HIERARCHY:
			panel = new Hierarchy_Scene();
			panel->Initialize();
			break;

		case PanelType::ATTRIBUTES:
			panel = new Panel_Attributes();
			panel->Initialize();
			break;

		case PanelType::SCENE_VIEWPORT:
		{
			Viewport_Scene* viewportPanel = new Viewport_Scene( viewportIndex );
			viewportPanel->Initialize( (viewportIndex == 0) );
			viewportPanel->SetCamera( _cameras[ viewportIndex ] );
			panel = viewportPanel;
			break;
		}

		case PanelType::PREFAB_VIEWPORT:
		{
			Viewport_Prefab* viewportPanel = new Viewport_Prefab( viewportIndex );
			viewportPanel->Initialize( (viewportIndex == 0) );
			viewportPanel->SetCamera( _cameras[ viewportIndex ] );
			panel = viewportPanel;
			break;
		}

		case PanelType::GAME_VIEWPORT:
		{
			Viewport_Game* viewportPanel = new Viewport_Game( viewportIndex );
			viewportPanel->Initialize( (viewportIndex == 0) );
			panel = viewportPanel;
			break;
		}

		case PanelType::ISOLATE_VIEWPORT:
		{
			Viewport_Isolate* viewportPanel = new Viewport_Isolate( viewportIndex );
			viewportPanel->Initialize( (viewportIndex == 0) );
			viewportPanel->SetCamera( _cameras[ viewportIndex ] );
			panel = viewportPanel;
			break;
		}

		case PanelType::ASSETS:
			panel = new Panel_Assets();
			panel->Initialize();
			break;

		case PanelType::CONSOLE:
			panel = new Panel_Console();
			panel->Initialize();
			break;

		case PanelType::PREFAB_HIERARCHY:
			panel = new Hierarchy_Prefab();
			panel->Initialize();
			break;

		case PanelType::ISOLATE_HIERARCHY:
			panel = new Hierarchy_Isolate();
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

		case PanelSplitType::NEW_TAB:
			_qtMainWindow->tabifyDockWidget( panelFrom, panel );
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
	PhysicsManager::CreateSingleton();

	ProjectBuilder::CreateSingleton();

	Random::Initialize();
}


void EditorApp::_InitializeManagers()
{
	EditorUtils::Initialize();

	_inputReceiver = new InputReceiverQT();

	EventManager::Singleton()->Initialize();
	SceneManager::Singleton()->Initialize();
	ScriptManager::Singleton()->Initialize();
	AssetManager::Singleton()->Initialize();

	JsonSerializeSystem* jsonSystem = new JsonSerializeSystem();
	jsonSystem->AddSerializer<EditorSettings>( new JsonSerializer_EditorConfig() );
	jsonSystem->AddSerializer<ProjectSettings>( new JsonSerializer_CyredProj() );

	FileManager::Singleton()->Initialize( new FileSystemWindows(),
										  new MeshLoaderWindows() );
	FileManager::Singleton()->SetSerializeSystem( jsonSystem );

	InputManager::Singleton()->Initialize( _inputReceiver );
	TimeManager::Singleton()->Initialize( EditorSettings::fps );
	DebugManager::Singleton()->Initialize();
	PhysicsManager::Singleton()->Initialize();

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
	PhysicsManager::Singleton()->Finalize();

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
	PhysicsManager::DestroySingleton();

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
	_qtMainWindow = new _QtMainWindow( this );
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
	DataArray<cchar*> skinsName;
	skinsName.Add( EditorSettings::SKIN_DEFAULT_BLACK );
	skinsName.Add( EditorSettings::SKIN_DEFAULT_WHITE );
	_menuBar->AddSkins( skinsName, 0 );

	_qtMainWindow->setMenuBar( _menuBar );
}


void EditorApp::_CreateStatusBar()
{
	// create status bar
	_qtStatusBar = new QStatusBar();

	// add to window
	_qtMainWindow->setStatusBar( _qtStatusBar );

	// show first status
	ShowStatus( EditorSettings::MSG_EDITOR_STARTED );
}


void EditorApp::_CreateToolbar()
{
	_toolbar = new Toolbar();
	_qtMainWindow->addToolBar( Qt::TopToolBarArea, _toolbar );
}


void EditorApp::_CreateSelectorPopup()
{
	_selectorPopup = new SelectorPopup( _qtMainWindow );
	_selectorPopup->Initialize();
}


void EditorApp::_CreateCameras()
{
	for ( int i = 0; i < 3; i++ ) {
		GameObject* cameraGO = new GameObject();
		cameraGO->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 10) );

		Camera* cameraComp1 = cameraGO->AddComponent<Camera>();
		cameraComp1->SetFovYAngle( 60 );
		cameraComp1->SetNearClipping( 0.1f );
		cameraComp1->SetFarClipping( 500.0f );

		FreeCameraControl* freeCamera = cameraGO->AddComponent<FreeCameraControl>();
		freeCamera->panSpeed	= 1.5f;
		freeCamera->rotateSpeed = 8.0f;
		freeCamera->zoomSpeed	= 80.0f;
		freeCamera->myWindows.Set( i, TRUE );

		_cameras.Add( cameraGO );
	}

	// add light for camera 1 and 2
	_cameras[1]->AddComponent<Light>();
	_cameras[2]->AddComponent<Light>();
}


void EditorApp::_UpdateCameras()
{
	for ( int i = 0; i < _cameras.Size(); i++ ) {
		FreeCameraControl* freeCamera = _cameras[i]->GetComponent<FreeCameraControl>();
		freeCamera->OnUpdate( FALSE );
	}
}


void EditorApp::_ReadConfigFile()
{
	char* fileData = FileManager::Singleton()->ReadFile( EditorSettings::FILE_PATH_CONFIG );
	FileManager::Singleton()->Deserialize<EditorSettings>( fileData, NULL );

	QDir dir;
	QFileInfo fileInfo( EditorSettings::projectPath.GetChar() );

	QString projDir = dir.relativeFilePath( fileInfo.absolutePath() );
	projDir.append( "/" ).append( FileManager::DIR_ASSETS );

	ProjectSettings::dirPathAssets = projDir.toUtf8().constData();
}


void EditorApp::_ReadProjectFile()
{
	char* fileData = FileManager::Singleton()->ReadFile( EditorSettings::projectPath.GetChar() );
	FileManager::Singleton()->Deserialize<ProjectSettings>( fileData, NULL );
}


void EditorApp::_WriteConfigFile()
{
	// collect window size
	EditorSettings::width = _qtMainWindow->width();
	EditorSettings::height = _qtMainWindow->height();

	// collect panels size
	for ( int i = 0; i < EditorSettings::panels.Size(); i++ ) {
		Panel* panel = _panels.Get( EditorSettings::panels[i].type );
		EditorSettings::panels[i].width = panel->width();
		EditorSettings::panels[i].height = panel->height();
	}

	// serialize
	String data = FileManager::Singleton()->Serialize<EditorSettings>( NULL );
	// write file
	FileManager::Singleton()->WriteFile( EditorSettings::FILE_PATH_CONFIG, data.GetChar() );
}


void EditorApp::ShowStatus( cchar* message )
{
	_qtStatusBar->showMessage( message, 0 );
}


// the skin must exist in skins directory
void EditorApp::ApplySkin( cchar* skinName )
{
	if ( skinName != NULL ) {
		FiniteString filePath( "%s%s%s", EditorSettings::DIR_PATH_SKINS, skinName,
							   EditorSettings::FILE_FORMAT_SKINS );

		char* stylesheet = FileManager::Singleton()->ReadFile( filePath.GetChar() );

		// apply skin to all elements
		if ( stylesheet != NULL ) {
			// remove old skin
			ARRAY_FREE( _skinStylesheet );
			// change stylesheet
			_skinStylesheet = stylesheet;
			// apply style
			_qtMainWindow->setStyleSheet( _skinStylesheet );
		}
	}
	else {
		// just refresh
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

	// announce play
	EventManager::Singleton()->EmitEvent( EditorEventType::PLAY_MODE_START, NULL );
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


void EditorApp::SetPlayPaused( bool value )
{
	// pause / unpause play mode
	_isPlayPaused = value;
	_toolbar->SetPauseButton( value );
}


void EditorApp::ShowSelectorPopup( cchar* type, void* qtSelectorPtr )
{
	_selectorPopup->DisplayPopup( type, qtSelectorPtr );
}


InputReceiverQT* EditorApp::GetInputReceiver()
{
	ASSERT( _inputReceiver != NULL );
	return _inputReceiver;
}
