// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Panel_PrefabHierarchy.h"

#include "CyredModule_Scene.h"
#include "CyredModule_File.h"
#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Script.h"

#include "../Settings/EditorSkin.h"
#include "../Settings/EditorSettings.h"
#include "../Settings/ProjectSettings.h"
#include "../../EditorApp.h"
#include "../../Utils/CustomTreeItem.h"

#include "QtWidgets\qtreewidget.h"
#include "QtGui\qevent.h"
#include "QtWidgets\qmenu.h"
#include "QtWidgets\qfiledialog.h"


using namespace CYRED;


class Panel_PrefabHierarchy::_QtTree : public QTreeWidget
{
public:
	void dropEvent( QDropEvent* e )
	{
		// get moved item
		CustomTreeItem* movedItem = CAST_S( CustomTreeItem*, this->currentItem() );
		// get old parent item
		CustomTreeItem* prevParent = CAST_S( CustomTreeItem*, movedItem->parent() );
		// get the order in the old hierarchy
		UInt prevIndexInHierarchy = prevParent->indexOfChild( movedItem );

		// apply the drop event
		QTreeWidget::dropEvent( e );	
		
		// get new parent item
		CustomTreeItem* newParent = CAST_S( CustomTreeItem*, movedItem->parent() );
		// check if drop is outside scene
		if ( newParent == NULL ) {
			// if so, reset drop
			// remove from tree
			UInt tmpIndex = this->indexOfTopLevelItem( movedItem );
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
		UInt indexInHierarchy = newParent->indexOfChild( movedItem );

		// apply changes to gameobjects
		if ( newParent != NULL ) {
			movedItem->gameObject->SetParentNode( NULL );
			newParent->gameObject->InsertChildNode( indexInHierarchy, movedItem->gameObject );
		}
		else {
			movedItem->gameObject->SetParentNode( NULL );
		}
	}
};


Panel_PrefabHierarchy::Panel_PrefabHierarchy()
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

	QObject::connect( _qtTree, &QTreeWidget::itemPressed, this, &Panel_PrefabHierarchy::A_ItemClicked );
	QObject::connect( _qtTree, &QTreeWidget::itemChanged, this, &Panel_PrefabHierarchy::A_ItemRenamed );
}


void Panel_PrefabHierarchy::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	_CreateRightClickMenu();

	// register events
	EventManager::Singleton()->RegisterListener( EventType::SELECT_ASSET, this );
	EventManager::Singleton()->RegisterListener( EventType::SELECT_GAMEOBJECT, this );
	EventManager::Singleton()->RegisterListener( EventType::RENAME_GAMEOBJECT, this );
	EventManager::Singleton()->RegisterListener( EventType::EDIT_PREFAB, this );
}


void Panel_PrefabHierarchy::Finalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::SELECT_ASSET, this );
	EventManager::Singleton()->UnregisterListener( EventType::SELECT_GAMEOBJECT, this );
	EventManager::Singleton()->UnregisterListener( EventType::RENAME_GAMEOBJECT, this );
	EventManager::Singleton()->UnregisterListener( EventType::EDIT_PREFAB, this );
}


void Panel_PrefabHierarchy::OnEvent( EventType eType, void* eData )
{
	switch ( eType ) {
		case EventType::EDIT_PREFAB:
		{
			Prefab* prefab = CAST_S( Prefab*, eData );

			// clear panel
			// delete all items
			while ( _qtTree->topLevelItemCount() > 0 ) {
				_qtTree->takeTopLevelItem( 0 );
			}

			_ResetHierarchy();
			break;
		}

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
		case EventType::SELECT_GAMEOBJECT:
			_qtTree->setCurrentItem( NULL );
			break;
	}
}


CustomTreeItem* Panel_PrefabHierarchy::_FindGameObjectItem( UInt uid )
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


void Panel_PrefabHierarchy::_CreateRightClickMenu()
{
	ASSERT( _isInitialized );

	_qtRightClickMenu = Memory::Alloc<QMenu>( this );

	_qtTree->setContextMenuPolicy( Qt::CustomContextMenu );
	QObject::connect( _qtTree, &QWidget::customContextMenuRequested, 
					  this, &Panel_PrefabHierarchy::A_RightClickMenu );
}


void Panel_PrefabHierarchy::_RecResetHierarchy( GameObject* gameObject, QTreeWidgetItem* parent )
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

	for ( UInt i = 0; i < gameObject->GetChildNodeCount(); ++i )
	{
		_RecResetHierarchy( CAST_S( GameObject*, gameObject->GetChildNodeAt(i) ), treeItem );
	}
}


void Panel_PrefabHierarchy::_ResetHierarchy()
{
	// delete all items
	while ( _qtTree->topLevelItemCount() > 0 ) {
		_qtTree->takeTopLevelItem( 0 );
	}

	// re-add all scenes
	QTreeWidgetItem* rootItem = _qtTree->invisibleRootItem();

	for ( UInt i = 0; i < SceneManager::Singleton()->CountLoadedScenes(); ++i )	{
		Scene* scene = SceneManager::Singleton()->GetScene( i );
		
		CustomTreeItem* treeItem = Memory::Alloc<CustomTreeItem>();
		//treeItem->scene = scene;
		//treeItem->sceneIndex = i;

		treeItem->setText( 0, scene->GetName() );
		treeItem->setFlags( Qt::ItemIsSelectable | 
							Qt::ItemIsEditable |
							Qt::ItemIsDropEnabled | 
							Qt::ItemIsEnabled );
		rootItem->addChild( treeItem );
		treeItem->setExpanded( TRUE );

		for ( UInt j = 0; j < scene->GetRoot()->GetChildNodeCount(); ++j )
		{
			_RecResetHierarchy( CAST_S( GameObject*, scene->GetRoot()->GetChildNodeAt(j) ), 
								treeItem );
		}
	}
}


void Panel_PrefabHierarchy::A_ItemClicked( QTreeWidgetItem* item, int column )
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, item );

	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, treeItem->gameObject );
}


void Panel_PrefabHierarchy::A_ItemRenamed( QTreeWidgetItem* item, int column )
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, item );

	treeItem->gameObject->SetName( item->text( 0 ).toUtf8().data() );
}


void Panel_PrefabHierarchy::A_RightClickMenu( const QPoint& pos )
{
	QTreeWidgetItem* item = _qtTree->itemAt( pos );

	if ( item != NULL )
	{

		_qtRightClickMenu->popup( _qtTree->mapToGlobal(pos) );
	}
}
