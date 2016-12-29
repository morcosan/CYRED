// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "ConsolePanel.h"
#include "../Settings/EditorSkin.h"

#include "QtWidgets\qboxlayout.h"


using namespace CYRED;


ConsolePanel::ConsolePanel()
{
	this->setWindowTitle( PANEL_TITLE );
	this->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
	this->setAllowedAreas( Qt::DockWidgetArea::AllDockWidgetAreas );
	this->setMinimumSize( MIN_SIZE.x, MIN_SIZE.y );

	QVBoxLayout* vLayout = Memory::Alloc<QVBoxLayout>();
	vLayout->setSpacing( 0 );
	vLayout->setContentsMargins( 0, 0, 0, 0 );

	QWidget* layoutWidget = Memory::Alloc<QWidget>();
	layoutWidget->setLayout( vLayout );
	layoutWidget->setObjectName( EditorSkin::PANEL_LAYOUT );

	this->setWidget( layoutWidget );
}


void ConsolePanel::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	// register console events
	EventManager::Singleton()->RegisterListener( EventType::CONSOLE_ERROR, this );
	EventManager::Singleton()->RegisterListener( EventType::CONSOLE_LOG, this );
}


void ConsolePanel::Finalize()
{
	ASSERT( _isInitialized );
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::CONSOLE_ERROR, this );
	EventManager::Singleton()->UnregisterListener( EventType::CONSOLE_LOG, this );
}


void ConsolePanel::OnEvent( EventType eType, void* eData )
{
	ASSERT( _isInitialized );

	switch ( eType ) {
		case EventType::CONSOLE_LOG:
		{
			break;
		}

		case EventType::CONSOLE_ERROR:
		{
			break;
		}
	}
}

