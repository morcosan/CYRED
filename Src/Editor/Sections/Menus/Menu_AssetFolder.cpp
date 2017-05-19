// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Menu_AssetFolderFolder.h"

#include "CyredBuildingBlocks.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"
#include "CyredModule_File.h"
#include "CyredModule_Event.h"

#include "../../Utils/CustomTreeItem.h"

#include "QtWidgets\qtreewidget.h"
#include "QtWidgets\qfiledialog.h"


using namespace CYRED;


Menu_AssetFolder::Menu_AssetFolder( QTreeWidget* qtTree )
	: QMenu( qtTree )
	, _qtTree( qtTree )
{
	// add actions

	QAction* actionSaveScene		= this->addAction( MENU_SAVE_SCENE );
	QAction* actionSaveSceneAs		= this->addAction( MENU_SAVE_SCENE_AS );
	QAction* actionCloseScene		= this->addAction( MENU_CLOSE_SCENE );

	this->addSeparator();

	QAction* actionRename			= this->addAction( MENU_RENAME );

	this->addSeparator();

	QMenu* menuGO					= this->addMenu( MENU_GO );
	QAction* actionGO_Empty			= menuGO->addAction( MENU_GO_EMPTY );

	QMenu*	 menuGO_3D				= menuGO->addMenu( MENU_GO_3D );
	QAction* actionGO_3D_Pivot		= menuGO_3D->addAction( MENU_GO_3D_PIVOT );
	QAction* actionGO_3D_Camera		= menuGO_3D->addAction( MENU_GO_3D_CAMERA );
	QAction* actionGO_3D_Light		= menuGO_3D->addAction( MENU_GO_3D_LIGHT );
	QAction* actionGO_3D_Mesh		= menuGO_3D->addAction( MENU_GO_3D_MESH );
	QAction* actionGO_3D_Morph		= menuGO_3D->addAction( MENU_GO_3D_MORPH );

	QMenu* menuGO_PS				= menuGO->addMenu( MENU_GO_PS );
	QAction* actionGO_PS_Emitter	= menuGO_PS->addAction( MENU_GO_PS_EMITTER );


	// add callbacks
	QObject::connect( actionSaveScene,		&QAction::triggered, this, &Menu_AssetFolder::A_SaveScene );
}


void Menu_AssetFolder::Open( const QPoint& pos )
{
	// display menu
	this->popup( _qtTree->mapToGlobal(pos) );
}
