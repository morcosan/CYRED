// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "ConsolePanel.h"
#include "../Settings/EditorSkin.h"
#include "CyredModule_Debug.h"

#include "QtWidgets\qboxlayout.h"
#include "QtWidgets\qlabel.h"
#include "QtWidgets\qscrollarea.h"
#include "QtWidgets\qpushbutton.h"


using namespace CYRED;


ConsolePanel::ConsolePanel()
{
	this->setWindowTitle( PANEL_TITLE );
	this->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
	this->setAllowedAreas( Qt::DockWidgetArea::AllDockWidgetAreas );
	this->setMinimumSize( MIN_SIZE.x, MIN_SIZE.y );

	// create top bar
	QHBoxLayout* qtTopBarLayout = Memory::Alloc<QHBoxLayout>();
	qtTopBarLayout->setAlignment( Qt::AlignLeft );
	qtTopBarLayout->setSpacing( 3 );

	// create buttons for top bar
	QPushButton* clearButton =  Memory::Alloc<QPushButton>();
	clearButton->setText( "Clear" );
	clearButton->setObjectName( EditorSkin::VIEWPORT_BUTTON );
	qtTopBarLayout->addWidget( clearButton );
	QObject::connect( clearButton,	&QPushButton::pressed, this, &ConsolePanel::A_ClearLogs );

	// create logs list
	_logsLayout = Memory::Alloc<QVBoxLayout>();
	_logsLayout->setSpacing( 0 );
	_logsLayout->setContentsMargins( 0, 0, 0, 0 );
	_logsLayout->setAlignment( Qt::AlignmentFlag::AlignTop );

	QWidget* logsWidget = Memory::Alloc<QWidget>();
	logsWidget->setLayout( _logsLayout );
	logsWidget->setObjectName( EditorSkin::CONSOLE_LAYOUT );

	// create scroll area
	QScrollArea* scrollArea = Memory::Alloc<QScrollArea>();
	scrollArea->setWidgetResizable( TRUE );
	scrollArea->setWidget( logsWidget );

	// create scroll area holder
	QHBoxLayout* scrollHolderLayout = new QHBoxLayout();
	scrollHolderLayout->setSpacing(0);
	scrollHolderLayout->setContentsMargins( 0, 0, 0, 0 );
	scrollHolderLayout->addWidget( scrollArea );
	QWidget* scrollHolderWidget = new QWidget();
	scrollHolderWidget->setLayout( scrollHolderLayout );

	// create layout for top bar + logs
	QVBoxLayout *vLayout = new QVBoxLayout();
	vLayout->setSpacing(0);
	vLayout->setContentsMargins( 0, 0, 0, 0 );
	vLayout->addLayout( qtTopBarLayout );
	vLayout->addWidget( scrollHolderWidget );

	QWidget* layoutWidget = new QWidget();
	layoutWidget->setLayout( vLayout );
	layoutWidget->setObjectName( EditorSkin::PANEL_LAYOUT );

	this->setWidget( layoutWidget );
}


void ConsolePanel::A_ClearLogs()
{
	// delete all items
	QLayoutItem *child;
	while ( (child = _logsLayout->takeAt(0)) !=  NULL ) {
		Memory::Free( child->widget() );
	}
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
			_AddLine( CAST_S(DebugInfo*, eData)->message, FALSE );
			break;
		}

		case EventType::CONSOLE_ERROR:
		{
			_AddLine( CAST_S(DebugInfo*, eData)->message, TRUE );
			break;
		}
	}
}


void ConsolePanel::_AddLine( const Char* message, Bool isError )
{
	// create layout
	QHBoxLayout* qtLineLayout = Memory::Alloc<QHBoxLayout>();
	// create widget for layout
	QWidget* qtLineWidget = Memory::Alloc<QWidget>();
	qtLineWidget->setLayout( qtLineLayout );
	qtLineWidget->setObjectName( EditorSkin::CONSOLE_LOG );

	// create label
	QLabel* qtMessage = Memory::Alloc<QLabel>( message );
	qtMessage->setTextInteractionFlags( Qt::TextSelectableByMouse );
	qtMessage->setWordWrap( TRUE );
	qtLineLayout->addWidget( qtMessage );

	// add line to console
	_logsLayout->addWidget( qtLineWidget );
}

