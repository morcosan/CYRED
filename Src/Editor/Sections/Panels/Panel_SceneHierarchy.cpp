// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Panel_SceneHierarchy.h"

#include "CyredModule_Scene.h"
#include "CyredModule_File.h"
#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Script.h"

#include "../Settings/EditorSkin.h"
#include "../Settings/EditorSettings.h"
#include "../../Utils/CustomTreeItem.h"
#include "../Menus/Menu_GameObject.h"
#include "../Menus/Menu_Scene.h"

#include "QtWidgets\qtreewidget.h"
#include "QtGui\qevent.h"
#include "QtWidgets\qmenu.h"
#include "QtWidgets\qfiledialog.h"


using namespace CYRED;


class Panel_SceneHierarchy::_QtTree : public QTreeWidget
{
public:
	void dropEvent( QDropEvent* e )
	{
		// get moved item
		CustomTreeItem* movedItem = CAST_S( CustomTreeItem*, this->currentItem() );
		// get old parent item
		CustomTreeItem* prevParent = CAST_S( CustomTreeItem*, movedItem->parent() );
		// get the order in the old hierarchy
		int prevIndexInHierarchy = prevParent->indexOfChild( movedItem );

		// apply the drop event
		QTreeWidget::dropEvent( e );	
		
		// get new parent item
		CustomTreeItem* newParent = CAST_S( CustomTreeItem*, movedItem->parent() );
		// check if drop is outside scene
		if ( newParent == NULL ) {
			// if so, reset drop
			// remove from tree
			int tmpIndex = this->indexOfTopLevelItem( movedItem );
			this->takeTopLevelItem( tmpIndex );
			// add back to old position
			prevParent->insertChild( prevIndexInHierarchy, movedItem );
			// select item
			this->setCurrentItem( movedItem );
			// exit
			return;
		}

		// select item
		this->setCurrentItem( movedItem );

		// get the order in the new hierarchy
		int indexInHierarchy = newParent->indexOfChild( movedItem );

		// apply changes to gameobjects
		if ( newParent != NULL ) {
			if ( newParent->asset != NULL ) {
				// if scene
				movedItem->gameObject->SetParentNode( NULL );
				Scene* scene = CAST_S( Scene*, newParent->asset );
				scene->GetRoot()->InsertChildNode( indexInHierarchy, movedItem->gameObject );
			}
			else {
				// if gameobject
				movedItem->gameObject->SetParentNode( NULL );
				newParent->gameObject->InsertChildNode( indexInHierarchy, movedItem->gameObject );
			}
		}
		else {
			movedItem->gameObject->SetParentNode( NULL );
		}
	}
};


Panel_SceneHierarchy::Panel_SceneHierarchy()
{
	this->setWindowTitle( PANEL_TITLE );
	this->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
	this->setAllowedAreas( Qt::DockWidgetArea::AllDockWidgetAreas );
	this->setMinimumSize( MIN_SIZE.x, MIN_SIZE.y );
	this->setMaximumSize( MAX_SIZE.x, MAX_SIZE.y );

	_qtTree = Memory::Alloc<_QtTree>();
	_qtTree->setHeaderHidden( true );
	_qtTree->setDragEnabled( true );
	_qtTree->setDragDropMode( QAbstractItemView::InternalMove );
	_qtTree->setObjectName( EditorSkin::HIERARCHY_TREE );
	_qtTree->setEditTriggers( QAbstractItemView::NoEditTriggers );

	this->setWidget( _qtTree );

	QObject::connect( _qtTree, &QTreeWidget::itemPressed, this, &Panel_SceneHierarchy::A_ItemClicked );
	QObject::connect( _qtTree, &QTreeWidget::itemChanged, this, &Panel_SceneHierarchy::A_ItemRenamed );
}


void Panel_SceneHierarchy::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	_CreateRightClickMenu();

	// register events
	EventManager::Singleton()->RegisterListener( EventType::ALL, this );
}


void Panel_SceneHierarchy::Finalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::ALL, this );
}


void Panel_SceneHierarchy::OnEvent( EventType eType, void* eData )
{
	switch ( eType ) {
		case EventType::CHANGE_SCENE_HIERARCHY:
			_ResetHierarchy();
			break;

		case EventType::RENAME_GAMEOBJECT:
		{
			GameObject* gameObject = CAST_S( GameObject*, eData );
			CustomTreeItem* treeItem = _FindGameObjectItem( gameObject->GetUniqueID() );
			if ( treeItem != NULL ) {
				treeItem->setText( 0, gameObject->GetName() );
			}

			break;
		}
		
		case EventType::SELECT_ASSET:
		case EventType::SELECT_PREFAB:
			_qtTree->setCurrentItem( NULL );
			break;

		case EventType::SELECT_GAMEOBJECT:
		{
			GameObject* gameObject = CAST_S( GameObject*, eData );
			CustomTreeItem* treeItem = _FindGameObjectItem( gameObject->GetUniqueID() );
			_qtTree->setCurrentItem( treeItem );
			break;
		}
			
		case EventType::CHANGE_ASSET:
		{
			_qtTree->setCurrentItem( NULL );

			Asset* asset = CAST_S( Asset*, eData );

			if ( asset != NULL && asset->GetAssetType() == AssetType::SCENE ) {
				CustomTreeItem* treeItem = _FindSceneItem( asset->GetUniqueID() );

				if ( treeItem != NULL )	{
					_qtTree->blockSignals( true );
					treeItem->setText( 0, asset->GetName() );
					_qtTree->blockSignals( false );
				}
			}

			break;
		}
	}
}


CustomTreeItem* Panel_SceneHierarchy::_FindGameObjectItem( int uid )
{
	QTreeWidgetItemIterator it( _qtTree );
	while ( *it != NULL ) {
		CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, *it );
		GameObject* gameObject = treeItem->gameObject;

		if ( gameObject != NULL && gameObject->GetUniqueID() == uid ) {
			return treeItem;
		}
		// next
		++it;
	}

	return NULL;
}


CustomTreeItem* Panel_SceneHierarchy::_FindSceneItem( const char* uid )
{
	String temp( uid );

	for ( int i = 0; i < _qtTree->topLevelItemCount(); ++i ) {
		CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->topLevelItem(i) );
		Asset* scene = treeItem->asset;

		if ( scene != NULL && temp == scene->GetUniqueID() ) {
			return treeItem;
		}
	}

	return NULL;
}


void Panel_SceneHierarchy::_CreateRightClickMenu()
{
	ASSERT( _isInitialized );

	// create menus
	_menuGameObject = Memory::Alloc<Menu_GameObject>( _qtTree, EventType::CHANGE_SCENE_HIERARCHY );
	_menuScene		= Memory::Alloc<Menu_Scene>( _qtTree );

	// add menu to tree
	_qtTree->setContextMenuPolicy( Qt::CustomContextMenu );
	QObject::connect( _qtTree, &QWidget::customContextMenuRequested, this, &Panel_SceneHierarchy::A_RightClickMenu );
}


void Panel_SceneHierarchy::_RecResetHierarchy( GameObject* gameObject, QTreeWidgetItem* parent )
{
	ASSERT( gameObject != NULL );

	CustomTreeItem* treeItem = Memory::Alloc<CustomTreeItem>();
	treeItem->gameObject = gameObject;
	
	treeItem->setText( 0, gameObject->GetName() );
	treeItem->setFlags( Qt::ItemIsSelectable |
						Qt::ItemIsEditable |
						Qt::ItemIsDragEnabled |
						Qt::ItemIsDropEnabled |
						Qt::ItemIsEnabled );
	parent->addChild( treeItem );

	for ( int i = 0; i < gameObject->GetChildNodeCount(); ++i )
	{
		_RecResetHierarchy( CAST_S( GameObject*, gameObject->GetChildNodeAt(i) ), treeItem );
	}
}


void Panel_SceneHierarchy::_ResetHierarchy()
{
	// delete all items
	while ( _qtTree->topLevelItemCount() > 0 ) {
		_qtTree->takeTopLevelItem( 0 );
	}

	// re-add all scenes
	QTreeWidgetItem* rootItem = _qtTree->invisibleRootItem();

	for ( int i = 0; i < SceneManager::Singleton()->CountLoadedScenes(); ++i )	{
		Scene* scene = SceneManager::Singleton()->GetScene( i );
		
		CustomTreeItem* treeItem = Memory::Alloc<CustomTreeItem>();
		treeItem->asset = scene;
		treeItem->assetIndex = i;

		treeItem->setText( 0, scene->GetName() );
		treeItem->setFlags( Qt::ItemIsSelectable | 
							Qt::ItemIsEditable |
							Qt::ItemIsDropEnabled | 
							Qt::ItemIsEnabled );
		rootItem->addChild( treeItem );
		treeItem->setExpanded( TRUE );

		for ( int j = 0; j < scene->GetRoot()->GetChildNodeCount(); ++j ) {
			_RecResetHierarchy( CAST_S( GameObject*, scene->GetRoot()->GetChildNodeAt(j) ), 
								treeItem );
		}
	}
}


void Panel_SceneHierarchy::A_ItemClicked( QTreeWidgetItem* item, int column )
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, item );

	if ( treeItem->gameObject != NULL ) {
		EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, treeItem->gameObject );
	}
	else if ( treeItem->asset != NULL ) {
		EventManager::Singleton()->EmitEvent( EventType::SELECT_SCENE, treeItem->asset );
	}
}


void Panel_SceneHierarchy::A_ItemRenamed( QTreeWidgetItem* item, int column )
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, item );

	if ( treeItem->gameObject != NULL ) {
		treeItem->gameObject->SetName( item->text( 0 ).toUtf8().data() );
	}
	else if ( treeItem->asset != NULL ) {
		FiniteString newName( item->text(0).toUtf8().constData() );

		_qtTree->blockSignals( true );
		item->setText( 0, treeItem->asset->GetName() );
		_qtTree->blockSignals( false );

		treeItem->asset->SetName( newName.GetChar() );
	}
}


void Panel_SceneHierarchy::A_RightClickMenu( const QPoint& pos )
{
	// get tree item
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->itemAt( pos ) );
	if ( treeItem != NULL ) {
		if ( treeItem->gameObject != NULL ) {
			// open gameobject menu
			_menuGameObject->Open( pos );
		}
		else if ( treeItem->asset != NULL ) {
			// open gameobject menu
			_menuScene->Open( pos );
		}
	}
}
