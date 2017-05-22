// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Menu_Asset.h"

#include "CyredBuildingBlocks.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"
#include "CyredModule_File.h"
#include "CyredModule_Event.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Script.h"

#include "../../Utils/CustomTreeItem.h"
#include "../../Utils/EditorUtils.h"
#include "../Panels/Panel_Assets.h"

#include "QtWidgets\qtreewidget.h"
#include "QtWidgets\qfiledialog.h"
#include "QtGui\QDesktopServices"


using namespace CYRED;


Menu_Asset::Menu_Asset( QTreeWidget* qtTree, Panel_Assets* panel )
	: QMenu( qtTree )
	, _qtTree( qtTree )
	, _panel( panel )
{
}


void Menu_Asset::Open( const QPoint& pos )
{
	// create custom menu
	this->clear();

	// get asset
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	Asset* asset = treeItem->asset;
	ASSERT( asset != NULL );

	// check if asset is scene
	if ( asset->GetAssetType() == AssetType::SCENE ) {
		QAction* actionOpenScene = this->addAction( MENU_OPEN_SCENE );
		QAction* actionLoadScene = this->addAction( MENU_LOAD_SCENE );
		this->addSeparator();

		QObject::connect( actionOpenScene, &QAction::triggered, this, &Menu_Asset::A_OpenScene );
		QObject::connect( actionLoadScene, &QAction::triggered, this, &Menu_Asset::A_LoadScene );
	}
	// check if asset is prefab
	else if ( asset->GetAssetType() == AssetType::PREFAB ) {
		QAction* actionEdit = this->addAction( MENU_PREFAB_EDIT );
		QObject::connect( actionEdit, &QAction::triggered, this, &Menu_Asset::A_EditPrefab );

		// add instantiate option if scene is open
		int totalScenes = SceneManager::Singleton()->CountLoadedScenes();
		if ( totalScenes > 0 ) {
			QAction* actionInstantiate = this->addAction( MENU_PREFAB_INST );
			QObject::connect( actionInstantiate, &QAction::triggered, this, &Menu_Asset::A_InstPrefab );
		}

		// separator
		this->addSeparator();
	}

	QAction* actionReload = this->addAction( MENU_RELOAD );
	QAction* actionRename = this->addAction( MENU_RENAME );

	if ( asset != NULL ) {
		QAction* actionDuplicate = this->addAction( MENU_DUPLICATE );
		QObject::connect( actionDuplicate,	&QAction::triggered, this, &Menu_Asset::A_Duplicate );
	}

	// separator
	this->addSeparator();

	QAction* actionOpenOnDisk = this->addAction( MENU_OPEN_DISK );
	QAction* actionShowOnDisk = this->addAction( MENU_SHOW_DISK );

	// separator
	this->addSeparator();

	QAction* actionDelete = this->addAction( MENU_DELETE );

	QObject::connect( actionReload,		&QAction::triggered, this, &Menu_Asset::A_ReloadAsset );
	QObject::connect( actionRename,		&QAction::triggered, this, &Menu_Asset::A_Rename );
	QObject::connect( actionOpenOnDisk, &QAction::triggered, this, &Menu_Asset::A_OpenOnDisk );
	QObject::connect( actionShowOnDisk, &QAction::triggered, this, &Menu_Asset::A_ShowOnDisk );
	QObject::connect( actionDelete,		&QAction::triggered, this, &Menu_Asset::A_Delete );


	// display menu
	this->popup( _qtTree->mapToGlobal(pos) );
}


void Menu_Asset::A_ReloadAsset()
{
	Asset* asset = CAST_S( CustomTreeItem*, _qtTree->currentItem() )->asset;
	ASSERT( asset != NULL );

	switch ( asset->GetAssetType() ) {
		case AssetType::MATERIAL:
		{
			Material* material = CAST_S( Material*, asset );
			material->LoadFullFile();
			break;
		}

		case AssetType::MESH:
		{
			Mesh* mesh = CAST_S( Mesh*, asset );
			mesh->LoadFullFile();
			break;
		}

		case AssetType::MORPH:
		{
			Morph* morph = CAST_S( Morph*, asset );
			morph->LoadFullFile();
			break;
		}

		case AssetType::TEXTURE:
		{
			Texture* texture = CAST_S( Texture*, asset );
			texture->LoadFullFile();
			break;
		}

		case AssetType::SHADER:
		{
			Shader* shader = CAST_S( Shader*, asset );
			shader->LoadFullFile();
			break;
		}

		case AssetType::SCRIPT:
		{
			Script* script = CAST_S( Script*, asset );
			script->LoadFullFile();
			script->LoadLuaFiles();
			break;
		}

		case AssetType::PREFAB:
		{
			// close prefab first
			EventManager::Singleton()->EmitEvent( EventType::CLOSE_PREFAB, asset );

			Prefab* prefab = CAST_S( Prefab*, asset );
			prefab->LoadFullFile();
			break;
		}
	}
}


void Menu_Asset::A_Rename()
{
	QTreeWidgetItem* item = _qtTree->currentItem();
	_qtTree->editItem( item );
}


void Menu_Asset::A_OpenOnDisk()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString filePath = item->whatsThis( 1 );
	QDesktopServices::openUrl( QUrl::fromLocalFile( filePath ) );
}


void Menu_Asset::A_ShowOnDisk()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString filePath = item->whatsThis( 1 );
	QFileInfo fileInfo( filePath );
	QDesktopServices::openUrl( QUrl::fromLocalFile( fileInfo.dir().path() ) );
}


void Menu_Asset::A_Duplicate()
{
	// get item selected
	QTreeWidgetItem* item = _qtTree->currentItem();
	// get asset
	Asset* asset = CAST_S( CustomTreeItem*, _qtTree->currentItem() )->asset;
	ASSERT( asset != NULL );

	// create new asset
	Asset* clone = asset->Clone();

	// create asset path for copying
	clone->SetEmitEvents( TRUE );
	EditorUtils::SetAvailableName( clone );

	// copy file
	FiniteString assetPath( "%s%s%s", asset->GetDirPath(), asset->GetName(), asset->GetExtension() );
	FiniteString clonePath( "%s%s%s", clone->GetDirPath(), clone->GetName(), clone->GetExtension() );
	FileManager::Singleton()->CopyFile( assetPath.GetChar(), clonePath.GetChar() );

	// load full file clone
	clone->LoadFullFile();
	// set new UID
	clone->SetUniqueID( Random::GenerateUniqueID().GetChar() );
	clone->SetIsTemporary( FALSE );
	clone->SetEmitEvents( TRUE );

	// add to asset pool and get icon
	const char* icon = NULL;

	switch ( asset->GetAssetType() ) {
		case AssetType::MATERIAL:
			AssetManager::Singleton()->AddMaterial( CAST_S( Material*, clone ) );
			icon = EditorUtils::ICON_MATERIAL;
			break;

		case AssetType::MESH:
			AssetManager::Singleton()->AddMesh( CAST_S( Mesh*, clone ) );
			icon = EditorUtils::ICON_MESH;
			break;

		case AssetType::MORPH:
			AssetManager::Singleton()->AddMorph( CAST_S( Morph*, clone ) );
			icon = EditorUtils::ICON_MORPH;
			break;

		case AssetType::SCENE:
			AssetManager::Singleton()->AddScene( CAST_S( Scene*, clone ) );
			icon = EditorUtils::ICON_SCENE;
			break;

		case AssetType::SCRIPT:
			AssetManager::Singleton()->AddScript( CAST_S( Script*, clone ) );
			icon = EditorUtils::ICON_SCRIPT;
			break;

		case AssetType::SHADER:
			AssetManager::Singleton()->AddShader( CAST_S( Shader*, clone ) );
			icon = EditorUtils::ICON_SHADER;
			break;

		case AssetType::TEXTURE:
			AssetManager::Singleton()->AddTexture( CAST_S( Texture*, clone ) );
			icon = EditorUtils::ICON_TEXTURE;
			break;

		case AssetType::PREFAB:
			AssetManager::Singleton()->AddPrefab( CAST_S( Prefab*, clone ) );
			icon = EditorUtils::ICON_PREFAB;
			break;
	}

	// add to tree
	QTreeWidgetItem* newItem = _panel->AddAssetToTree( clone, item->parent(), icon );
	// select it
	_qtTree->setCurrentItem( newItem );

	// send event
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, clone );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_ASSET, clone );
}


void Menu_Asset::A_Delete()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	Asset* asset = CAST_S( CustomTreeItem*, _qtTree->currentItem() )->asset;

	if ( asset != NULL ) {
		// clear
		asset->ClearAsset();

		// remove from manager
		switch ( asset->GetAssetType() ) {
			case AssetType::MATERIAL:
				AssetManager::Singleton()->RemoveMaterial( CAST_S( Material*, asset ) );
				break;

			case AssetType::MESH:
				AssetManager::Singleton()->RemoveMesh( CAST_S( Mesh*, asset ) );
				break;

			case AssetType::MORPH:
				AssetManager::Singleton()->RemoveMorph( CAST_S( Morph*, asset ) );
				break;

			case AssetType::SHADER:
				AssetManager::Singleton()->RemoveShader( CAST_S( Shader*, asset ) );
				break;

			case AssetType::TEXTURE:
				AssetManager::Singleton()->RemoveTexture( CAST_S( Texture*, asset ) );
				break;

			case AssetType::SCENE:
				AssetManager::Singleton()->RemoveScene( CAST_S( Scene*, asset ) );
				break;

			case AssetType::SCRIPT:
				AssetManager::Singleton()->RemoveScript( CAST_S( Script*, asset ) );
				break;

			case AssetType::PREFAB:
				AssetManager::Singleton()->RemovePrefab( CAST_S( Prefab*, asset ) );
				// send event
				EventManager::Singleton()->EmitEvent( EventType::CLOSE_PREFAB, asset );
				break;
		}
	}

	QFileInfo fileInfo( item->whatsThis(1) );
	if ( fileInfo.isDir() ) {
		QDir( item->whatsThis(1) ).removeRecursively();
	}
	else {
		QDir().remove( item->whatsThis(1) );
	}

	_qtTree->setCurrentItem( NULL );
	Memory::Free( item );

	EventManager::Singleton()->EmitEvent( EventType::SELECT_ASSET, NULL );
}


void Menu_Asset::A_EditPrefab()
{
	Asset* asset = CAST_S( CustomTreeItem*, _qtTree->currentItem() )->asset;
	ASSERT( asset->GetAssetType() == AssetType::PREFAB );

	EventManager::Singleton()->EmitEvent( EventType::OPEN_PREFAB, asset );
}


void Menu_Asset::A_InstPrefab()
{
	Asset* asset = CAST_S( CustomTreeItem*, _qtTree->currentItem() )->asset;
	ASSERT( asset->GetAssetType() == AssetType::PREFAB );

	Prefab* prefab = CAST_S( Prefab*, asset );
	SceneManager::Singleton()->Instantiate( prefab );
}


void Menu_Asset::A_OpenScene()
{
	Asset* asset = CAST_S( CustomTreeItem*, _qtTree->currentItem() )->asset;
	ASSERT( asset->GetAssetType() == AssetType::SCENE );

	SceneManager::Singleton()->OpenScene( asset->GetUniqueID() );
}


void Menu_Asset::A_LoadScene()
{
	Asset* asset = CAST_S( CustomTreeItem*, _qtTree->currentItem() )->asset;
	ASSERT( asset->GetAssetType() == AssetType::SCENE );

	SceneManager::Singleton()->LoadScene( asset->GetUniqueID() );
}
