// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "QtUtils.h"

#include "QtWidgets\qlayout.h"
#include "QtWidgets\qwidget.h"


using namespace CYRED;


void QtUtils::ClearLayout( QLayout* qtLayout )
{
	QLayoutItem *item;
	while ( item = qtLayout->takeAt(0) ) {
		if ( item->layout() ) {
			ClearLayout( item->layout() );
			Memory::Free( item->layout() );
		}

		if ( item->widget() ) {
			Memory::Free( item->widget() );
		}

		Memory::Free( item );
	}
}
