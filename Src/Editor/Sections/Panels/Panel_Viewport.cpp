// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Panel_Viewport.h"
#include "CyredModule_Render.h"

#include "../../EngineOverride/OpenGL/GLContextImpl.h"
#include "../../EngineOverride/Input/InputReceiverQT.h"
#include "../../EditorApp.h"
#include "../Settings/EditorSkin.h"

#include "QtWidgets\QVBoxLayout"


using namespace CYRED;


void Panel_Viewport::_QtWindow::keyPressEvent( QKeyEvent* e )
{
	_inputReceiver->OnKeyPress( e );
}
void Panel_Viewport::_QtWindow::keyReleaseEvent( QKeyEvent* e )
{
	_inputReceiver->OnKeyRelease( e );
}
void Panel_Viewport::_QtWindow::mouseMoveEvent( QMouseEvent* e )
{
	_inputReceiver->OnMouseMove( e, _windowIndex );
}
void Panel_Viewport::_QtWindow::mousePressEvent( QMouseEvent* e )
{
	_inputReceiver->OnMousePress( e, _windowIndex );
}
void Panel_Viewport::_QtWindow::mouseReleaseEvent( QMouseEvent* e )
{
	_inputReceiver->OnMouseRelease( e, _windowIndex );
}
void Panel_Viewport::_QtWindow::wheelEvent( QWheelEvent* e )
{
	_inputReceiver->OnWheelMove( e, _windowIndex );
}
void Panel_Viewport::_QtWindow::focusOutEvent( QFocusEvent* e )
{
	_inputReceiver->OnFocusOut( _windowIndex );
}


Panel_Viewport::Panel_Viewport( int panelIndex )
	: _mustResize( TRUE )
	, _panelIndex( panelIndex )
	, _canvasSlot( INVALID_CANVAS_SLOT )
	, _isFirstUpdate( TRUE )
	, _cameraGO( NULL )
{
}


void Panel_Viewport::Initialize()
{
	ASSERT( FALSE );	// use Initialize with bool instead
}


void Panel_Viewport::Finalize()
{
	ASSERT( _isInitialized );
}


void Panel_Viewport::Initialize( bool isPrimary )
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	this->setWindowTitle( _GetPanelTitle() );
	this->setMinimumSize( MIN_SIZE.x, MIN_SIZE.y );

	_qtWindow = new _QtWindow( _panelIndex, EditorApp::Singleton()->GetInputReceiver() );
	_qtWindow->setSurfaceType( QWindow::OpenGLSurface );

	_qtContainer = QWidget::createWindowContainer( _qtWindow );

	_qtTopBarLayout = new QHBoxLayout();
	_qtTopBarLayout->setAlignment( Qt::AlignLeft );
	_qtTopBarLayout->setSpacing( 3 );

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->setSpacing(0);
	vLayout->setContentsMargins( 0, 0, 0, 0 );
	vLayout->addLayout( _qtTopBarLayout );
	vLayout->addWidget( _qtContainer );

	QWidget* layoutWidget = new QWidget();
	layoutWidget->setLayout( vLayout );

	this->setWidget( layoutWidget );

	_glContext = new GLContextImpl( _qtWindow );

	if ( isPrimary ) {
		_canvasSlot = 0;
	}

	_OnInitialize();
}


void Panel_Viewport::Update( bool isRuntime )
{
	ASSERT( _isInitialized );

	// check if window is visible
	if ( !_qtWindow->isExposed() || _qtContainer->visibleRegion().isEmpty() ) {
		return;
	}

	// chekc if canvas exists
	if ( _canvasSlot == INVALID_CANVAS_SLOT ) {
		_CreateCanvasSlot();
	}

	// render
	_OnUpdate( isRuntime );

	// mark flag
	_isFirstUpdate = FALSE;
}


GLContext* Panel_Viewport::GetGLContext()
{
	ASSERT( _isInitialized );
	return _glContext;
}


void Panel_Viewport::_OnResize()
{
	_mustResize = TRUE;
}


void Panel_Viewport::_CreateCanvasSlot()
{
	ASSERT( _isInitialized );

	_canvasSlot = RenderManager::Singleton()->CreateCanvas( _glContext );
}
