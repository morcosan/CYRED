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
		const char* const	MENU_RELOAD			= "Reload Asset";
		const char* const	MENU_OPEN_DISK		= "Open On Disk";
		const char* const	MENU_SHOW_DISK		= "Show On Disk";
		const char* const	MENU_RENAME			= "Rename";
		const char* const	MENU_DUPLICATE		= "Duplicate";
		const char* const	MENU_DELETE			= "Delete";
		const char* const	MENU_OPEN_SCENE		= "Open Scene";
		const char* const	MENU_LOAD_SCENE		= "Load Scene";

		const char* const	MENU_PREFAB_EDIT	= "Edit";
		const char* const	MENU_PREFAB_INST	= "Instantiate";


	public:
		Menu_Asset( QTreeWidget* qtTree, Panel_Assets* panel );
		virtual ~Menu_Asset() {}


	public:
		void Open( const QPoint& pos );


	public:
		void A_ReloadAsset	();
		void A_OpenOnDisk	();
		void A_ShowOnDisk	();
		void A_Rename		();
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