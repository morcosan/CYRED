// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Menu_AssetFolder.h"

#include "CyredBuildingBlocks.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"
#include "CyredModule_File.h"
#include "CyredModule_Event.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Script.h"

#include "../../Utils/CustomTreeItem.h"
#include "../../Utils/EditorUtils.h"
#include "../Settings/ProjectSettings.h"
#include "../Panels/Panel_Assets.h"

#include "QtWidgets\qtreewidget.h"
#include "QtWidgets\qfiledialog.h"
#include "QtGui\QDesktopServices"


using namespace CYRED;


Menu_AssetFolder::Menu_AssetFolder( QTreeWidget* qtTree, Panel_Assets* panel )
	: QMenu( qtTree )
	, _qtTree( qtTree )
	, _panel( panel )
{
}


void Menu_AssetFolder::Open( const QPoint& pos )
{
	// create custom menu
	this->clear();

	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->itemAt( pos ) );
	// check if root folder
	if ( treeItem->parent() != NULL ) {
		QAction* actionRename = this->addAction( MENU_RENAME );

		// separator
		this->addSeparator();

		QAction* actionOpenOnDisk = this->addAction( MENU_OPEN_DISK );
		QAction* actionShowOnDisk = this->addAction( MENU_SHOW_DISK );

		// separator
		this->addSeparator();

		QAction* actionDelete = this->addAction( MENU_DELETE );

		// separator
		this->addSeparator();


		QObject::connect( actionRename,		&QAction::triggered, this, &Menu_AssetFolder::A_Rename );
		QObject::connect( actionOpenOnDisk, &QAction::triggered, this, &Menu_AssetFolder::A_OpenOnDisk );
		QObject::connect( actionShowOnDisk, &QAction::triggered, this, &Menu_AssetFolder::A_ShowOnDisk );
		QObject::connect( actionDelete,		&QAction::triggered, this, &Menu_AssetFolder::A_Delete );
	}


	QMenu* menuCreate = this->addMenu( MENU_CREATE );

	QAction* actionFolder = menuCreate->addAction( EditorUtils::NAME_FOLDER );
	menuCreate->addSeparator();

	QMenu* menuMaterial = menuCreate->addMenu( EditorUtils::NAME_MATERIAL );

	QAction* actionMatEmpty = menuMaterial->addAction( EditorUtils::NAME_MAT_EMPTY );
	QAction* actionMatPS = menuMaterial->addAction( EditorUtils::NAME_MAT_PS );

	QMenu* menuTexture = menuCreate->addMenu( EditorUtils::NAME_TEXTURE );
	QAction* actionTex2D = menuTexture->addAction( EditorUtils::NAME_TEX_2D );
	QAction* actionTexCM = menuTexture->addAction( EditorUtils::NAME_TEX_CM );

	QAction* actionShader	= menuCreate->addAction( EditorUtils::NAME_SHADER );
	QAction* actionMesh		= menuCreate->addAction( EditorUtils::NAME_MESH );
	QAction* actionMorph	= menuCreate->addAction( EditorUtils::NAME_MORPH );
	QAction* actionScript	= menuCreate->addAction( EditorUtils::NAME_SCRIPT );

	
	QObject::connect( actionFolder,		&QAction::triggered, this, &Menu_AssetFolder::A_Create_Folder );
	QObject::connect( actionMatEmpty,	&QAction::triggered, this, &Menu_AssetFolder::A_Create_Mat_Empty );
	QObject::connect( actionMatPS,		&QAction::triggered, this, &Menu_AssetFolder::A_Create_Mat_PS );
	QObject::connect( actionTex2D,		&QAction::triggered, this, &Menu_AssetFolder::A_Create_Tex_2D );
	QObject::connect( actionTexCM,		&QAction::triggered, this, &Menu_AssetFolder::A_Create_Tex_CM );
	QObject::connect( actionShader,		&QAction::triggered, this, &Menu_AssetFolder::A_Create_Shader );
	QObject::connect( actionMesh,		&QAction::triggered, this, &Menu_AssetFolder::A_Create_Mesh );
	QObject::connect( actionMorph,		&QAction::triggered, this, &Menu_AssetFolder::A_Create_Morph );
	QObject::connect( actionScript,		&QAction::triggered, this, &Menu_AssetFolder::A_Create_Script );


	// display menu
	this->popup( _qtTree->mapToGlobal(pos) );
}


void Menu_AssetFolder::A_Rename()
{
	QTreeWidgetItem* item = _qtTree->currentItem();
	_qtTree->editItem( item );
}


void Menu_AssetFolder::A_OpenOnDisk()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString filePath = item->whatsThis( 1 );
	QDesktopServices::openUrl( QUrl::fromLocalFile( filePath ) );
}


void Menu_AssetFolder::A_ShowOnDisk()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString filePath = item->whatsThis( 1 );
	QFileInfo fileInfo( filePath );
	QDesktopServices::openUrl( QUrl::fromLocalFile( fileInfo.dir().path() ) );
}



void Menu_AssetFolder::A_Delete()
{
	// get item
	QTreeWidgetItem* item = _qtTree->currentItem();

	// remove from disk
	QFileInfo fileInfo( item->whatsThis(1) );
	ASSERT( fileInfo.isDir() );
	QDir( item->whatsThis(1) ).removeRecursively();

	_qtTree->setCurrentItem( NULL );
	Memory::Free( item );
}


void Menu_AssetFolder::A_Create_Folder()
{
	QTreeWidgetItem* parentItem = _qtTree->currentItem();

	QString dirPath = (parentItem != NULL) ? parentItem->whatsThis( 1 ).append( "/" ) 
										   : ProjectSettings::dirPathAssets.GetChar();

	// generate name
	// find new name
	int nextIndex = -1;
	FiniteString folderName;
	FiniteString folderPath;

	do {
		nextIndex++;
		folderName.Set( "%s%d", EditorUtils::NAME_FOLDER, nextIndex );
		folderPath.Set( "%s%s", dirPath.toUtf8().constData(), folderName.GetChar() );

		// check if exists
		QDir dir( folderPath.GetChar() );

		// if not existing, create dir and break loop
		if ( !dir.exists() ) {
			dir.mkpath( "." );
			break;
		}
	} 
	while ( true );

	// create folder
	CustomTreeItem* treeItem = Memory::Alloc<CustomTreeItem>();
	treeItem->setText( 0, folderName.GetChar() );
	treeItem->setWhatsThis( 0, EditorUtils::NAME_FOLDER );  // we use this field to store data
	treeItem->setWhatsThis( 1, folderPath.GetChar() ); 
	treeItem->setFlags( Qt::ItemIsSelectable | 
						Qt::ItemIsEnabled | 
						Qt::ItemIsDragEnabled |
						Qt::ItemIsDropEnabled | 
						Qt::ItemIsEditable );
	treeItem->setIcon( 0, *EditorUtils::GetIcon( EditorUtils::ICON_FOLDER ) );

	if ( parentItem != NULL ) {
		parentItem->addChild( treeItem );
		parentItem->setExpanded( TRUE );
	}
	else {
		_qtTree->addTopLevelItem( treeItem );
	}

	// select item
	_qtTree->setCurrentItem( treeItem );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_ASSET, NULL );
}


void Menu_AssetFolder::A_Create_Mat_Empty()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
		ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::MATERIAL );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Menu_AssetFolder::A_Create_Mat_PS()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
		ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::MATERIAL );

	Material* material = CAST_S( Material*, asset );
	material->SetEmitEvents( FALSE );
	material->SetProperty( "particleTexture", NULL );
	material->SetProperty( "colorStart", Vector4(1, 1, 1, 1) );
	material->SetProperty( "colorEnd", Vector4(1, 1, 1, 1) );
	material->SetEmitEvents( TRUE );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Menu_AssetFolder::A_Create_Tex_2D()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
		ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::TEXTURE );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Menu_AssetFolder::A_Create_Tex_CM()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
		ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::TEXTURE );

	Texture* texture = CAST_S( Texture*, asset );
	texture->SetEmitEvents( FALSE );
	texture->SetTextureType( TextureType::CUBE_MAP );
	texture->SetEmitEvents( TRUE );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Menu_AssetFolder::A_Create_Shader()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
		ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::SHADER );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Menu_AssetFolder::A_Create_Mesh()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
		ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::MESH );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Menu_AssetFolder::A_Create_Morph()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
		ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::MORPH );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Menu_AssetFolder::A_Create_Script()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
		ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::SCRIPT );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}



Asset* Menu_AssetFolder::_AddNewAsset( cchar* dirPath, QTreeWidgetItem* parentItem,
									   AssetType assetType )
{
	Asset*		asset		= NULL;
	cchar* icon		= NULL;

	switch ( assetType ) {
		case AssetType::MATERIAL:
		{
			Material* material = Memory::Alloc<Material>();
			material->SetEmitEvents( FALSE );
			material->SetDirPath( dirPath );
			material->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			material->SetIsTemporary( FALSE );
			EditorUtils::SetAvailableName( material );
			material->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddMaterial( material );

			asset = material;
			icon = EditorUtils::ICON_MATERIAL;
			break;
		}

		case AssetType::MESH:
		{
			Mesh* mesh = Memory::Alloc<Mesh>();
			mesh->SetEmitEvents( FALSE );
			mesh->SetDirPath( dirPath );
			mesh->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			mesh->SetIsTemporary( FALSE );
			EditorUtils::SetAvailableName( mesh );
			mesh->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddMesh( mesh );

			asset = mesh;
			icon = EditorUtils::ICON_MESH;
			break;
		}

		case AssetType::MORPH:
		{
			Morph* morph = Memory::Alloc<Morph>();
			morph->SetEmitEvents( FALSE );
			morph->SetDirPath( dirPath );
			morph->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			morph->SetIsTemporary( FALSE );
			EditorUtils::SetAvailableName( morph );
			morph->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddMorph( morph );

			asset = morph;
			icon = EditorUtils::ICON_MORPH;
			break;
		}

		case AssetType::SCRIPT:
		{
			Script* script = Memory::Alloc<Script>();
			script->SetEmitEvents( FALSE );
			script->SetDirPath( dirPath );
			script->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			script->SetIsTemporary( FALSE );
			EditorUtils::SetAvailableName( script );
			script->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddScript( script );

			asset = script;
			icon = EditorUtils::ICON_SCRIPT;
			break;
		}

		case AssetType::SHADER:
		{
			Shader* shader = Memory::Alloc<Shader>();
			shader->SetEmitEvents( FALSE );
			shader->SetDirPath( dirPath );
			shader->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			shader->SetIsTemporary( FALSE );
			EditorUtils::SetAvailableName( shader );
			shader->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddShader( shader );

			asset = shader;
			icon = EditorUtils::ICON_SHADER;
			break;
		}

		case AssetType::TEXTURE:
		{
			Texture* texture = Memory::Alloc<Texture>();
			texture->SetEmitEvents( FALSE );
			texture->SetDirPath( dirPath );
			texture->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			texture->SetIsTemporary( FALSE );
			EditorUtils::SetAvailableName( texture );
			texture->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddTexture( texture );

			asset = texture;
			icon = EditorUtils::ICON_TEXTURE;
			break;
		}
	}

	// add to tree
	QTreeWidgetItem* newItem = _panel->AddAssetToTree( asset, parentItem, icon );

	// select item
	_qtTree->setCurrentItem( newItem );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_ASSET, asset );

	return asset;
}