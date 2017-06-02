// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Menu_AssetUnknown.h"

#include "CyredBuildingBlocks.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"
#include "CyredModule_File.h"
#include "CyredModule_Event.h"

#include "../../Utils/CustomTreeItem.h"

#include "QtWidgets\qtreewidget.h"
#include "QtWidgets\qfiledialog.h"
#include "QtGui\QDesktopServices"


using namespace CYRED;


Menu_AssetUnknown::Menu_AssetUnknown( QTreeWidget* qtTree )
	: QMenu( qtTree )
	, _qtTree( qtTree )
{
	// add actions

	QAction* actionRename = this->addAction( MENU_RENAME );

	// separator
	this->addSeparator();

	QAction* actionOpenOnDisk = this->addAction( MENU_OPEN_DISK );
	QAction* actionShowOnDisk = this->addAction( MENU_SHOW_DISK );

	// separator
	this->addSeparator();

	QAction* actionDelete = this->addAction( MENU_DELETE );

	QObject::connect( actionRename,		&QAction::triggered, this, &Menu_AssetUnknown::A_Rename );
	QObject::connect( actionOpenOnDisk, &QAction::triggered, this, &Menu_AssetUnknown::A_OpenOnDisk );
	QObject::connect( actionShowOnDisk, &QAction::triggered, this, &Menu_AssetUnknown::A_ShowOnDisk );
	QObject::connect( actionDelete,		&QAction::triggered, this, &Menu_AssetUnknown::A_Delete );
}


void Menu_AssetUnknown::Open( const QPoint& pos )
{
	// display menu
	this->popup( _qtTree->mapToGlobal(pos) );
}


void Menu_AssetUnknown::A_Rename()
{
	QTreeWidgetItem* item = _qtTree->currentItem();
	_qtTree->editItem( item );
}


void Menu_AssetUnknown::A_OpenOnDisk()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString filePath = item->whatsThis( 1 );
	QDesktopServices::openUrl( QUrl::fromLocalFile( filePath ) );
}


void Menu_AssetUnknown::A_ShowOnDisk()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString filePath = item->whatsThis( 1 );
	QFileInfo fileInfo( filePath );
	QDesktopServices::openUrl( QUrl::fromLocalFile( fileInfo.dir().path() ) );
}


void Menu_AssetUnknown::A_Delete()
{
	// get item
	QTreeWidgetItem* item = _qtTree->currentItem();

	// remove from disk
	QFileInfo fileInfo( item->whatsThis(1) );
	ASSERT( !fileInfo.isDir() );
	QDir().remove( item->whatsThis(1) );

	_qtTree->setCurrentItem( NULL );
	PTR_FREE( item );
}