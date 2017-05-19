// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Panel.h"

using namespace CYRED;



Panel::Panel()
	: _isInitialized( FALSE )
{
}


void Panel::resizeEvent( QResizeEvent * e )
{
	_OnResize();
}

