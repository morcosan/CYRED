// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Event.h"
#include "CyredBuildingBlocks.h"
#include "../Panel.h"

#include "QtGui\qicon.h"

class QTreeWidget;
class QTreeWidgetItem;
class QMenu;


namespace CYRED
{
	class AssetsPanel : public Panel, public EventListener
	{
		const Char* const	PANEL_TITLE = "Assets Panel";
		const Vector2		MIN_SIZE	= Vector2( 200, 250 );

		const Char* const	ICON_SCENE		= "scene_icon";
		const Char* const	ICON_FOLDER		= "folder_icon";
		const Char* const	ICON_MATERIAL	= "material_icon";
		const Char* const	ICON_MESH		= "mesh_icon";
		const Char* const	ICON_TEXTURE	= "texture_icon";
		const Char* const	ICON_SHADER		= "shader_icon";
		const Char* const	ICON_UNKNOWN	= "unknown_icon";

		const Char* const	TYPE_FOLDER		= "Folder";

		const Char* const	MENU_RELOAD		= "Reload Asset";
		const Char* const	MENU_OPEN_DISK	= "Open On Disk";
		const Char* const	MENU_SHOW_DISK	= "Show On Disk";
		const Char* const	MENU_RENAME		= "Rename";
		const Char* const	MENU_OPEN_SCENE	= "Open Scene";
		const Char* const	MENU_LOAD_SCENE	= "Load Scene";

		const Char* const	DEBUG_EXISTING_ASSET		= "WARNING: Asset %s not loaded. Already existing.";
		const Char* const	DEBUG_INVALID_UID			= "WARNING: Asset %s has invalid UID. New UID generated.";
		const Char* const	DEBUG_DUPLICATED_FILE_PATH	= "WARNING: File rename failed. File path already exists.";



	public:
		AssetsPanel();
		virtual ~AssetsPanel() {}


	public:
		void Initialize	()	override;

		void OnEvent	( EventType eType, EventName eName, void* eSource )	override;


	public:
		void A_ItemClicked		( QTreeWidgetItem* item, int column );
		void A_Item2xClicked	( QTreeWidgetItem* item, int column );
		void A_ItemRenamed		( QTreeWidgetItem* item, int column );
		void A_RightClickMenu	( const QPoint& pos );
		void A_ReloadAsset		();
		void A_OpenOnDisk		();
		void A_ShowOnDisk		();
		void A_Rename			();
		void A_OpenScene		();
		void A_LoadScene		();

		void ReloadAllAssets	();


	protected:
		class _QtTreeItem;

		static QTreeWidget*		_firstTreeInstance;

		QTreeWidget*			_qtTree;
		DataMap<String, QIcon>	_icons;
		QMenu*					_qtRightClickMenu;

		void _LoadIcons				();
		void _ParseDirectory		( const Char* dirPath, QTreeWidgetItem* parentItem );
		void _CreateRightClickMenu	();
		void _AddRightClickActions	( QTreeWidgetItem* item );
		void _SaveAssetToFile		( Asset* asset, const Char* oldName );
		Bool _IsFilePathDuplicate	( Asset* asset );

		_QtTreeItem* _FindTreeItem	( Asset* asset );
	};
}