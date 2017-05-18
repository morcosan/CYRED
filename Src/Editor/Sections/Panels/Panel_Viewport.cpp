// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Panel_Viewport.h"
#include "../../EngineOverride/OpenGL/GLContextImpl.h"
#include "../../EngineOverride/Input/InputReceiverQT.h"
#include "../../EditorApp.h"
#include "CyredModule_Render.h"

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
	this->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
	this->setAllowedAreas( Qt::DockWidgetArea::AllDockWidgetAreas );
	Vector2 minSize = _GetPanelMinSize();
	this->setMinimumSize( minSize.x, minSize.y );

	_qtWindow = Memory::Alloc<_QtWindow>( _panelIndex, EditorApp::Singleton()->GetInputReceiver() );
	_qtWindow->setSurfaceType( QWindow::OpenGLSurface );

	QWidget *container = QWidget::createWindowContainer( _qtWindow );

	_qtTopBarLayout = Memory::Alloc<QHBoxLayout>();
	_qtTopBarLayout->setAlignment( Qt::AlignLeft );
	_qtTopBarLayout->setSpacing( 3 );

	QVBoxLayout *vLayout = new QVBoxLayout();
	vLayout->setSpacing(0);
	vLayout->setContentsMargins( 0, 0, 0, 0 );
	vLayout->addLayout( _qtTopBarLayout );
	vLayout->addWidget( container );

	QWidget* layoutWidget = new QWidget();
	layoutWidget->setLayout( vLayout );

	this->setWidget( layoutWidget );

	_glContext = Memory::Alloc<GLContextImpl>( _qtWindow );

	if ( isPrimary ) {
		_canvasSlot = 0;
	}

	_OnInitialize();
}


void Panel_Viewport::Update()
{
	ASSERT( _isInitialized );

	if ( !_qtWindow->isExposed() ) {
		return;
	}

	if ( _canvasSlot == INVALID_CANVAS_SLOT ) {
		_CreateCanvasSlot();
	}

	_OnUpdate();

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
