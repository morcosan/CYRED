// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Panel.h"

using namespace CYRED;



Panel::Panel()
	: _isInitialized( FALSE )
{
	//this->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
	this->setFeatures( 0 );
	this->setAllowedAreas( Qt::DockWidgetArea::AllDockWidgetAreas );
}


void Panel::resizeEvent( QResizeEvent * e )
{
	_OnResize();
}

