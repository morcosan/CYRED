// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"

#include "QtWidgets\qmenu.h"


class QTreeWidget;
class QTreeWidgetItem;

namespace CYRED
{
	class Panel_Assets;
}


namespace CYRED
{
	class Menu_AssetFolder : public QMenu
	{
		const char* const	MENU_RENAME		= "Rename";
		const char* const	MENU_OPEN_DISK	= "Open On Disk";
		const char* const	MENU_SHOW_DISK	= "Show On Disk";
		const char* const	MENU_DUPLICATE	= "Duplicate";
		const char* const	MENU_DELETE		= "Delete";
		const char* const	MENU_CREATE		= "Create";


	public:
		Menu_AssetFolder( QTreeWidget* qtTree, Panel_Assets* panel );
		virtual ~Menu_AssetFolder() {}


	public:
		void Open( const QPoint& pos );


	public:
		void A_Rename			();
		void A_OpenOnDisk		();
		void A_ShowOnDisk		();
		void A_Delete			();

		void A_Create_Folder	();
		void A_Create_Mat_Empty	();
		void A_Create_Mat_PS	();
		void A_Create_Tex_2D	();
		void A_Create_Tex_CM	();
		void A_Create_Shader	();
		void A_Create_Mesh		();
		void A_Create_Morph		();
		void A_Create_Script	();


	private:
		QTreeWidget*	_qtTree;
		Panel_Assets*	_panel;


	private:
		Asset* _AddNewAsset( const char* dirPath, QTreeWidgetItem* parentItem, AssetType assetType );
	};
}