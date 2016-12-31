// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "ViewportPanel.h"
#include "../../EngineOverride/OpenGL/GLContextImpl.h"
#include "../../EngineOverride/Input/InputReceiverQT.h"
#include "../../EditorApp.h"
#include "CyredModule_Render.h"

#include "QtWidgets\QVBoxLayout"


using namespace CYRED;


void ViewportPanel::_QtWindow::keyPressEvent( QKeyEvent* e )
{
	_inputReceiver->OnKeyPress( e );
}
void ViewportPanel::_QtWindow::keyReleaseEvent( QKeyEvent* e )
{
	_inputReceiver->OnKeyRelease( e );
}
void ViewportPanel::_QtWindow::mouseMoveEvent( QMouseEvent* e )
{
	_inputReceiver->OnMouseMove( e, _windowIndex );
}
void ViewportPanel::_QtWindow::mousePressEvent( QMouseEvent* e )
{
	_inputReceiver->OnMousePress( e, _windowIndex );
}
void ViewportPanel::_QtWindow::mouseReleaseEvent( QMouseEvent* e )
{
	_inputReceiver->OnMouseRelease( e, _windowIndex );
}
void ViewportPanel::_QtWindow::wheelEvent( QWheelEvent* e )
{
	_inputReceiver->OnWheelMove( e, _windowIndex );
}
void ViewportPanel::_QtWindow::focusOutEvent( QFocusEvent* e )
{
	_inputReceiver->OnFocusOut( _windowIndex );
}


ViewportPanel::ViewportPanel( UInt panelIndex )
	: _mustResize( TRUE )
	, _panelIndex( panelIndex )
	, _canvasSlot( INVALID_CANVAS_SLOT )
	, _isFirstUpdate( TRUE )
	, _cameraGO( NULL )
{
}


void ViewportPanel::Initialize()
{
	ASSERT( FALSE );	// use Initialize with Bool instead
}


void ViewportPanel::Finalize()
{
	ASSERT( _isInitialized );
}


void ViewportPanel::Initialize( Bool isPrimary )
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

	QWidget* newWidget = new QWidget();
	newWidget->setLayout( vLayout );

	this->setWidget( newWidget );

	_glContext = Memory::Alloc<GLContextImpl>( _qtWindow );

	if ( isPrimary )
	{
		_canvasSlot = 0;
	}

	_OnInitialize();
}


void ViewportPanel::Update()
{
	ASSERT( _isInitialized );

	if ( !_qtWindow->isExposed() )
	{
		return;
	}

	if ( _canvasSlot == INVALID_CANVAS_SLOT )
	{
		_CreateCanvasSlot();
	}

	_OnUpdate();

	_isFirstUpdate = FALSE;
}


GLContext* ViewportPanel::GetGLContext()
{
	ASSERT( _isInitialized );
	return _glContext;
}


void ViewportPanel::_OnResize()
{
	_mustResize = TRUE;
}


void ViewportPanel::_CreateCanvasSlot()
{
	ASSERT( _isInitialized );

	_canvasSlot = RenderManager::Singleton()->NewCanvas( _glContext );
}
