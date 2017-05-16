// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Panel_PrefabHierarchy.h"

#include "CyredModule_File.h"
#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Script.h"

#include "../Settings/EditorSkin.h"
#include "../Settings/EditorSettings.h"
#include "../../Utils/CustomTreeItem.h"
#include "../Menus/Menu_GameObject.h"
#include "../Menus/Menu_Prefab.h"

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
		// check if drop is outside prefab
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
			if ( newParent->asset != NULL ) {
				// if prefab
				movedItem->gameObject->SetParentNode( NULL );
				Prefab* prefab = CAST_S( Prefab*, newParent->asset );
				prefab->GetRoot()->InsertChildNode( indexInHierarchy, movedItem->gameObject );
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
	EventManager::Singleton()->RegisterListener( EventType::ALL, this );
}


void Panel_PrefabHierarchy::Finalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::ALL, this );
}


void Panel_PrefabHierarchy::OnEvent( EventType eType, void* eData )
{
	switch ( eType ) {
		case EventType::OPEN_PREFAB:
			_targetPrefab = CAST_S( Prefab*, eData );
			_ResetHierarchy();
			break;
		
		case EventType::CHANGE_PREFAB_HIERARCHY:
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
		case EventType::SELECT_SCENE:
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

			if ( asset != NULL && asset->GetAssetType() == AssetType::PREFAB ) {
				CustomTreeItem* treeItem = _FindPrefabItem( asset->GetUniqueID() );

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


CustomTreeItem* Panel_PrefabHierarchy::_FindPrefabItem( const Char* uid )
{
	String temp( uid );

	for ( Int i = 0; i < _qtTree->topLevelItemCount(); ++i ) {
		CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->topLevelItem(i) );
		Asset* prefab = treeItem->asset;

		if ( prefab != NULL && temp == prefab->GetUniqueID() ) {
			return treeItem;
		}
	}

	return NULL;
}


void Panel_PrefabHierarchy::_CreateRightClickMenu()
{
	ASSERT( _isInitialized );

	// create menus
	_menuGameObject = Memory::Alloc<Menu_GameObject>( _qtTree, EventType::CHANGE_PREFAB_HIERARCHY );
	_menuPrefab		= Memory::Alloc<Menu_Prefab>( _qtTree );

	// add menu to tree
	_qtTree->setContextMenuPolicy( Qt::CustomContextMenu );
	QObject::connect( _qtTree, &QWidget::customContextMenuRequested, this, &Panel_PrefabHierarchy::A_RightClickMenu );
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

	// re-add objects
	QTreeWidgetItem* rootItem = _qtTree->invisibleRootItem();

	// sanity check
	if ( _targetPrefab != NULL ) {
		CustomTreeItem* treeItem = Memory::Alloc<CustomTreeItem>();
		treeItem->asset = _targetPrefab;

		treeItem->setText( 0, _targetPrefab->GetName() );
		treeItem->setFlags( Qt::ItemIsSelectable | 
							Qt::ItemIsEditable |
							Qt::ItemIsDropEnabled | 
							Qt::ItemIsEnabled );
		rootItem->addChild( treeItem );
		treeItem->setExpanded( TRUE );

		for ( UInt j = 0; j < _targetPrefab->GetRoot()->GetChildNodeCount(); ++j ) {
			_RecResetHierarchy( CAST_S( GameObject*, _targetPrefab->GetRoot()->GetChildNodeAt(j) ), 
								treeItem );
		}
	}
}


void Panel_PrefabHierarchy::A_ItemClicked( QTreeWidgetItem* item, int column )
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, item );

	if ( treeItem->gameObject != NULL ) {
		EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, treeItem->gameObject );
	}
	else if ( treeItem->asset != NULL ) {
		EventManager::Singleton()->EmitEvent( EventType::SELECT_PREFAB, treeItem->asset );
	}
}


void Panel_PrefabHierarchy::A_ItemRenamed( QTreeWidgetItem* item, int column )
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


void Panel_PrefabHierarchy::A_RightClickMenu( const QPoint& pos )
{
	// get tree item
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->itemAt( pos ) );
	if ( treeItem != NULL ) {
		if ( treeItem->gameObject != NULL ) {
			// open gameobject menu
			_menuGameObject->Open( pos );
		}
		else if ( treeItem->asset != NULL ) {
			// open prefab menu
			_menuPrefab->Open( pos );
		}
	}
}
