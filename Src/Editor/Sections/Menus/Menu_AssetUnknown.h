// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"

#include "QtWidgets\qmenu.h"


class QTreeWidget;


namespace CYRED
{
	class Menu_AssetUnknown : public QMenu
	{
		


	public:
		Menu_AssetUnknown( QTreeWidget* qtTree );
		virtual ~Menu_AssetUnknown() {}


	public:
		void Open( const QPoint& pos );


	public:



	private:
		QTreeWidget*	_qtTree;
	};
}