// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Toolbar.h"

#include "../EditorApp.h"
#include "../Utils/EditorEvents.h"
#include "../Utils/EditorUtils.h"

#include "QtWidgets\qlayout.h"
#include "QtWidgets\qtoolbutton.h"


using namespace CYRED;


Toolbar::Toolbar()
{
	this->setFloatable( false );
	this->setMovable( false );
	this->setMinimumHeight( 34 );
	this->setMaximumHeight( 34 );

	QHBoxLayout* boxLayout = new QHBoxLayout();
	boxLayout->setAlignment( Qt::AlignHCenter );
	boxLayout->setContentsMargins( 0, 0, 0, 0 );

	_playButton = new QToolButton();
	_playButton->setIcon( *EditorUtils::GetIcon( EditorUtils::ICON_PLAY ) );
	_playButton->setMinimumWidth( 24 );
	_playButton->setMinimumHeight( 24 );
	_playButton->setIconSize( QSize( 12, 12 ) );
	_playButton->setCheckable( true );
	boxLayout->addWidget( _playButton );

	_pauseButton = new QToolButton();
	_pauseButton->setIcon( *EditorUtils::GetIcon( EditorUtils::ICON_PAUSE ) );
	_pauseButton->setMinimumWidth( 24 );
	_pauseButton->setMinimumHeight( 24 );
	_pauseButton->setIconSize( QSize( 10, 10 ) );
	_pauseButton->setCheckable( true );
	boxLayout->addWidget( _pauseButton );

	QWidget* layoutWidget = new QWidget();
	layoutWidget->setLayout( boxLayout );
	layoutWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

	this->addWidget( layoutWidget );

	// add actions
	QObject::connect( _playButton,	&QToolButton::clicked, this, &Toolbar::A_PlayButton );
	QObject::connect( _pauseButton,	&QToolButton::clicked, this, &Toolbar::A_PauseButton );
}


void Toolbar::A_PlayButton( bool checked )
{
	// start / stop the play ode
	if ( checked ) {
		EditorApp::Singleton()->StartPlayMode();
	}
	else {
		EditorApp::Singleton()->StopPlayMode();
	}

	// unselect everything
	EventManager::Singleton()->EmitEvent( EditorEventType::ASSET_SELECT, NULL );
}


void Toolbar::A_PauseButton( bool checked )
{
	// pause the play
	EditorApp::Singleton()->SetPlayPaused( checked );
}


void Toolbar::SetPlayButton( bool isActive )
{
	// update button
	_playButton->setChecked( isActive );
}


void Toolbar::SetPauseButton( bool isActive )
{
	// update button
	_pauseButton->setChecked( isActive );
}

