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
		const char* const	MENU_RENAME		= "Rename";
		const char* const	MENU_OPEN_DISK	= "Open On Disk";
		const char* const	MENU_SHOW_DISK	= "Show On Disk";
		const char* const	MENU_DUPLICATE	= "Duplicate";
		const char* const	MENU_DELETE		= "Delete";


	public:
		Menu_AssetUnknown( QTreeWidget* qtTree );
		virtual ~Menu_AssetUnknown() {}


	public:
		void Open( const QPoint& pos );


	public:
		void A_Rename		();
		void A_OpenOnDisk	();
		void A_ShowOnDisk	();
		void A_Delete		();


	private:
		QTreeWidget*	_qtTree;
	};
}