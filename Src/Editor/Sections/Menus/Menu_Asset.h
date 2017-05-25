// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"

#include "QtWidgets\qmenu.h"


class QTreeWidget;

namespace CYRED
{
	class Panel_Assets;
}


namespace CYRED
{
	class Menu_Asset : public QMenu
	{
		cchar* const	MENU_RELOAD			= "Reload Asset";

		cchar* const	MENU_RENAME			= "Rename";
		cchar* const	MENU_OPEN_DISK		= "Open On Disk";
		cchar* const	MENU_SHOW_DISK		= "Show On Disk";
		cchar* const	MENU_DUPLICATE		= "Duplicate";
		cchar* const	MENU_DELETE			= "Delete";

		cchar* const	MENU_OPEN_SCENE		= "Open Scene";
		cchar* const	MENU_LOAD_SCENE		= "Load Scene";

		cchar* const	MENU_PREFAB_EDIT	= "Edit";
		cchar* const	MENU_PREFAB_INST	= "Instantiate";


	public:
		Menu_Asset( QTreeWidget* qtTree, Panel_Assets* panel );
		virtual ~Menu_Asset() {}


	public:
		void Open( const QPoint& pos );


	public:
		void A_ReloadAsset	();
		void A_Rename		();
		void A_OpenOnDisk	();
		void A_ShowOnDisk	();
		void A_Duplicate	();
		void A_Delete		();

		void A_EditPrefab	();
		void A_InstPrefab	();

		void A_OpenScene	();
		void A_LoadScene	();


	private:
		QTreeWidget*	_qtTree;
		Panel_Assets*	_panel;
	};
}