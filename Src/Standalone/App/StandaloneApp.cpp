// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "StandaloneApp.h"
#include "CyredEngine.h"

#include <QtWidgets/QApplication>
#include <QtGui\qwindow.h>

#include "../OpenGL/GLImpl_3_0.h"
#include "../OpenGL/GLContextImpl.h"

#include "../GameController.h"


using namespace CYRED;
using namespace NotAPI;



class StandaloneApp::_QtMainWindow : public QMainWindow
{
public:
	_QtMainWindow( StandaloneApp* appImpl )
	{
		_appImpl = appImpl;
	}

	virtual void closeEvent( QCloseEvent *event )
	{
		_appImpl->Exit();
	}

protected:
	StandaloneApp* _appImpl;
};


class StandaloneApp::_QtWindow : public QWindow
{
public:

};



void StandaloneApp::Run( Int& argc, Char* argv[], GameController* gameController )
{
	_gameController =  gameController;

	_Run( argc, argv );
}


void StandaloneApp::Run( Int& argc, Char* argv[], Char* configFilePath )
{
	_Run( argc, argv );
}


void StandaloneApp::_Run( Int& argc, Char* argv[] )
{
	_CreateManagers();


	_qtApp = Memory::Alloc<QApplication>( argc, argv );

	_qtMainWindow = Memory::Alloc<_QtMainWindow>( this );
	_qtMainWindow->resize( 300, 300 );
	_qtMainWindow->move( 100, 100 );
	_qtMainWindow->setWindowTitle( "CYRED-Standalone" );
	_qtMainWindow->show();

	_qtWindow = Memory::Alloc<_QtWindow>();
	_qtWindow->setSurfaceType( QWindow::OpenGLSurface );

	// we need a container, it cannot add directly
	QWidget* container = QWidget::createWindowContainer( _qtWindow );
	_qtMainWindow->setCentralWidget( container );


	

	_qtApp->processEvents(); // first time show all
	_shouldExit = FALSE;


	_InitializeManagers();

	_canvasID = 0; // the first canvas
	UInt techID = RenderManager::Singleton()->NewTechnique( TechniqueType::FORWARD_BASIC );
	RenderManager::Singleton()->ChangeRenderer( _canvasID, RendererType::GL_FORWARD );
	RenderManager::Singleton()->ChangeTechnique( _canvasID, techID );


	if ( _gameController != NULL )
	{
		_gameController->Start();
	}

	while ( !_shouldExit ) // will be changed from other thread
	{
		if ( _gameController != NULL )
		{
			_gameController->Update();
		}
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

	Memory::Free( _qtMainWindow );
	Memory::Free( _qtApp );
}


void StandaloneApp::_UpdateLoop()
{
	RenderManager::Singleton()->RenderScene( _canvasID );

	_qtApp->processEvents();
}


void StandaloneApp::_CreateManagers()
{
	SceneManager::CreateSingleton();
	RenderManager::CreateSingleton();
}


void StandaloneApp::_InitializeManagers()
{
	SceneManager::Singleton()->Initialize();
	RenderManager::Singleton()->Initialize( Memory::Alloc<GLContextImpl>( _qtWindow ), Memory::Alloc<GLImpl_3_0>() );
}


void StandaloneApp::_FinalizeManagers()
{
	SceneManager::Singleton()->Finalize();
	RenderManager::Singleton()->Finalize();
}


void StandaloneApp::_DestroyManagers()
{
	SceneManager::DestroySingleton();
	RenderManager::DestroySingleton();
}




