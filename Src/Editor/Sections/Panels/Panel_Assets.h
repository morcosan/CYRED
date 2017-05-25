// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Event.h"
#include "CyredBuildingBlocks.h"
#include "../Panel.h"

class QTreeWidgetItem;
class QMenu;
class QFileSystemWatcher;

namespace CYRED
{
	class CustomTreeItem;
	class Menu_Asset;
	class Menu_AssetFolder;
	class Menu_AssetUnknown;
	class Prefab;
}


namespace CYRED
{
	class Panel_Assets : public Panel, public IEventListener
	{
		cchar* const	PANEL_TITLE		= "Assets";
		const Vector2		MIN_SIZE		= Vector2( 200, 100 );
		const Vector2		MAX_SIZE		= Vector2( 300, 1000 );

		cchar* const	BUTTON_RELOAD	= "Reload";

		cchar* const	DEBUG_EXISTING_ASSET		= "WARNING: Asset %s%s not loaded. Already existing.";
		cchar* const	DEBUG_INVALID_UID			= "WARNING: Asset %s%s has invalid UID. New UID generated.";
		cchar* const	DEBUG_DUPLICATED_FILE_PATH	= "WARNING: File rename failed. File path already exists.";
		cchar* const	DEBUG_FOLDER_NOT_FOUND		= "ERROR: Directory %s not found.";


	public:
		Panel_Assets();
		virtual ~Panel_Assets() {}


	public:
		void Initialize	()	override;
		void Finalize	()	override;

		void OnEvent	( EventType eType, void* eData )	override;


	public:
		void			A_ItemClicked	( QTreeWidgetItem* item, int column );
		void			A_Item2xClicked	( QTreeWidgetItem* item, int column );
		void			A_ItemRenamed	( QTreeWidgetItem* item, int column );
		void			A_RightClickMenu( const QPoint& pos );
		void			A_DirChanged	( const QString& path );
		void			A_ReloadAll		();

		void			ReloadAllAssets	();
		CustomTreeItem*	AddAssetToTree	( Asset* asset, QTreeWidgetItem* parentItem, 
										  cchar* icon );


	private:
		class _QtTree;

		_QtTree*				_qtTree;
		QFileSystemWatcher*		_qtFileWatcher;

		Menu_Asset*				_menuAsset;
		Menu_AssetFolder*		_menuAssetFolder;
		Menu_AssetUnknown*		_menuAssetUnknown;
		Prefab*					_openedPrefab;


	private:
		void			_ParseDirectory			( cchar* dirName, cchar* dirPath, 
												  QTreeWidgetItem* parentItem );
		void			_CreateRightClickMenu	();
		void			_SaveAssetToFile		( Asset* asset, cchar* oldName );
		CustomTreeItem*	_FindTreeItem			( Asset* asset );
		CustomTreeItem*	_FindFolderItem			( cchar* dirPath );
	};
}