// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"

#include "QtWidgets\qmenu.h"


class QTreeWidget;


namespace CYRED
{
	class Menu_AssetFolder : public QMenu
	{
	


	public:
		Menu_AssetFolder( QTreeWidget* qtTree );
		virtual ~Menu_AssetFolder() {}


	public:
		void Open( const QPoint& pos );


	public:



	private:
		QTreeWidget*	_qtTree;
	};
}