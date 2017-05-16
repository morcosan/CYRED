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
class QFileSystemWatcher;


namespace CYRED
{
	class Panel_Assets : public Panel, public IEventListener
	{
		const Char* const	PANEL_TITLE		= "Assets";
		const Vector2		MIN_SIZE		= Vector2( 200, 100 );
		const Vector2		MAX_SIZE		= Vector2( 300, 1000 );

		const Char* const	ICON_SCENE		= "scene";
		const Char* const	ICON_FOLDER		= "folder";
		const Char* const	ICON_MATERIAL	= "material";
		const Char* const	ICON_MESH		= "mesh";
		const Char* const	ICON_MORPH		= "morph";
		const Char* const	ICON_TEXTURE	= "texture";
		const Char* const	ICON_SHADER		= "shader";
		const Char* const	ICON_SCRIPT		= "script";
		const Char* const	ICON_PREFAB		= "prefab";
		const Char* const	ICON_UNKNOWN	= "unknown";

		const Char* const	BUTTON_RELOAD	= "Reload";

		const Char* const	TYPE_FOLDER		= "Folder";

		const Char* const	MENU_RELOAD		= "Reload Asset";
		const Char* const	MENU_OPEN_DISK	= "Open On Disk";
		const Char* const	MENU_SHOW_DISK	= "Show On Disk";
		const Char* const	MENU_RENAME		= "Rename";
		const Char* const	MENU_DUPLICATE	= "Duplicate";
		const Char* const	MENU_DELETE		= "Delete";
		const Char* const	MENU_OPEN_SCENE	= "Open Scene";
		const Char* const	MENU_LOAD_SCENE	= "Load Scene";

		const Char* const	MENU_PREFAB_EDIT	= "Edit";
		const Char* const	MENU_PREFAB_INST	= "Instantiate";

		const Char* const	MENU_CREATE			= "Create";
		const Char* const	MENU_C_FOLDER		= "Folder";
		const Char* const	MENU_C_MATERIAL		= "Material";
		const Char* const	MENU_C_MAT_EMPTY	= "Empty";
		const Char* const	MENU_C_MAT_PS		= "Particles";
		const Char* const	MENU_C_TEXTURE		= "Texture";
		const Char* const	MENU_C_TEX_2D		= "Texture2D";
		const Char* const	MENU_C_TEX_CM		= "CubeMap";
		const Char* const	MENU_C_SHADER		= "Shader";
		const Char* const	MENU_C_MESH			= "Mesh";
		const Char* const	MENU_C_MORPH		= "Morph";
		const Char* const	MENU_C_SCRIPT		= "Script";
		const Char* const	MENU_C_SCENE		= "Scene";

		const Char* const	DEBUG_EXISTING_ASSET		= "WARNING: Asset %s%s not loaded. Already existing.";
		const Char* const	DEBUG_INVALID_UID			= "WARNING: Asset %s%s has invalid UID. New UID generated.";
		const Char* const	DEBUG_DUPLICATED_FILE_PATH	= "WARNING: File rename failed. File path already exists.";
		const Char* const	DEBUG_FOLDER_NOT_FOUND		= "ERROR: Directory %s not found.";


	public:
		Panel_Assets();
		virtual ~Panel_Assets() {}


	public:
		void Initialize	()	override;
		void Finalize	()	override;

		void OnEvent	( EventType eType, void* eData )	override;


	public:
		void A_ItemClicked		( QTreeWidgetItem* item, int column );
		void A_Item2xClicked	( QTreeWidgetItem* item, int column );
		void A_ItemRenamed		( QTreeWidgetItem* item, int column );
		void A_RightClickMenu	( const QPoint& pos );

		void A_ReloadAsset		();
		void A_OpenOnDisk		();
		void A_ShowOnDisk		();
		void A_Rename			();
		void A_Duplicate		();
		void A_Delete			();

		void A_EditPrefab		();
		void A_InstPrefab		();

		void A_OpenScene		();
		void A_LoadScene		();

		void A_Create_Folder	();
		void A_Create_Mat_Empty	();
		void A_Create_Mat_PS	();
		void A_Create_Tex_2D	();
		void A_Create_Tex_CM	();
		void A_Create_Shader	();
		void A_Create_Mesh		();
		void A_Create_Morph		();
		void A_Create_Script	();

		void A_DirChanged		( const QString& path );

		void A_ReloadAll		();

		void ReloadAllAssets	();


	protected:
		class _QtTreeItem;

		QTreeWidget*			_qtTree;
		DataMap<String, QIcon>	_icons;
		QMenu*					_qtRightClickMenu;

		QFileSystemWatcher*		_qtFileWatcher;


		void		_LoadIcons				();
		void		_ParseDirectory			( const Char* dirName, const Char* dirPath, 
											  QTreeWidgetItem* parentItem );
		Asset*		_AddNewAsset			( const Char* dirPath, QTreeWidgetItem* parentItem,
											  AssetType assetType );
		void		_CreateRightClickMenu	();
		void		_AddRightClickActions	( QTreeWidgetItem* item );
		void		_SaveAssetToFile		( Asset* asset, const Char* oldName );
		Bool		_IsFilePathDuplicate	( Asset* asset );
		void		_SetAvailableName		( Asset* asset );
		_QtTreeItem* _AddAssetToTree		( Asset* asset, QTreeWidgetItem* parentItem, 
											  const Char* icon );

		_QtTreeItem* _FindTreeItem			( Asset* asset );
		_QtTreeItem* _FindFolderItem		( const Char* dirPath );
	};
}