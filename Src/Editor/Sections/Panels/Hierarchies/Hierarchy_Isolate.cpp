// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Hierarchy_Isolate.h"

#include "CyredModule_File.h"
#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Script.h"

#include "../../Settings/EditorSkin.h"
#include "../../Settings/EditorSettings.h"
#include "../../../Utils/CustomTree.h"
#include "../../../Utils/CustomTreeItem.h"
#include "../../Menus/Menu_GameObject.h"

#include "QtWidgets\qtreewidget.h"
#include "QtGui\qevent.h"
#include "QtWidgets\qmenu.h"
#include "QtWidgets\qfiledialog.h"


using namespace CYRED;


void Hierarchy_Isolate::_OnInitialize()
{
	// create menus
	_menuGameObject = Memory::Alloc<Menu_GameObject>( _qtTree, this, EditorEventType::PREFAB_UPDATE );

	// add menu to tree
	_qtTree->setContextMenuPolicy( Qt::CustomContextMenu );
	QObject::connect( _qtTree, &QWidget::customContextMenuRequested, 
					  this, &Hierarchy_Isolate::A_RightClickMenu );
}


void Hierarchy_Isolate::OnEvent( int eventType, void* eventData )
{
	switch ( eventType ) {
		case EditorEventType::ISOLATE_OPEN_SCENE:
		case EditorEventType::ISOLATE_OPEN_PREFAB:
			// store data
			_target = CAST_S( GameObject*, eventData );
			_openEventType = eventType;
			// update hierarchy
			_ResetHierarchy();
			break;

		case EventType::GAMEOBJECT_UPDATE:
			// update 
			_ResetHierarchy();
			break;

		case EventType::SCENE_CLOSE:
			if ( _openEventType == EditorEventType::ISOLATE_OPEN_SCENE ) {
				_target = NULL;
				// update 
				_ResetHierarchy();
			}
			break;

		case EditorEventType::PREFAB_CLOSE:
			if ( _openEventType == EditorEventType::ISOLATE_OPEN_PREFAB ) {
				_target = NULL;
				// update 
				_ResetHierarchy();
			}
			break;

		case EventType::GAMEOBJECT_RENAME:
		{
			GameObject* gameObject = CAST_S( GameObject*, eventData );
			CustomTreeItem* treeItem = _FindGameObjectItem( gameObject->GetUniqueID() );
			if ( treeItem != NULL ) {
				treeItem->setText( 0, gameObject->GetName() );
			}
			break;
		}

		case EditorEventType::ASSET_SELECT:
		case EditorEventType::SCENE_SELECT:
		case EditorEventType::PREFAB_SELECT:
		case EditorEventType::GAMEOBJECT_SELECT:
			_qtTree->setCurrentItem( NULL );
			break;

		case EditorEventType::ISOLATE_SELECT:
			if ( eventData != NULL ) {
				GameObject* gameObject = CAST_S( GameObject*, eventData );
				CustomTreeItem* treeItem = _FindGameObjectItem( gameObject->GetUniqueID() );
				_qtTree->setCurrentItem( treeItem );
			}
			break;
	}
}


CustomTreeItem* Hierarchy_Isolate::_FindGameObjectItem( int uid )
{
	QTreeWidgetItemIterator it( _qtTree );
	while ( *it != NULL ) {
		CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, *it );
		GameObject* gameObject = CAST_S( GameObject*, treeItem->node );

		if ( gameObject != NULL && gameObject->GetUniqueID() == uid ) {
			return treeItem;
		}
		// next
		++it;
	}

	return NULL;
}


void Hierarchy_Isolate::_ResetHierarchy()
{
	// delete all items
	while ( _qtTree->topLevelItemCount() > 0 ) {
		_qtTree->takeTopLevelItem( 0 );
	}

	// re-add objects
	QTreeWidgetItem* rootItem = _qtTree->invisibleRootItem();

	// sanity check
	if ( _target != NULL ) {
		CustomTreeItem* treeItem = Memory::Alloc<CustomTreeItem>();
		treeItem->node = _target;

		treeItem->setText( 0, _target->GetName() );
		treeItem->setFlags( Qt::ItemIsSelectable | 
							Qt::ItemIsEditable |
							Qt::ItemIsDropEnabled | 
							Qt::ItemIsEnabled );
		rootItem->addChild( treeItem );
		treeItem->setExpanded( TRUE );

		for ( int j = 0; j < _target->GetChildNodeCount(); ++j ) {
			_RecResetHierarchy( CAST_S( GameObject*, _target->GetChildNodeAt(j) ), treeItem );
		}
	}
}


void Hierarchy_Isolate::A_ItemClicked( QTreeWidgetItem* item, int column )
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, item );

	// show gameobject attributes
	EventManager::Singleton()->EmitEvent( EditorEventType::ISOLATE_SELECT, treeItem->node );
}


void Hierarchy_Isolate::A_RightClickMenu( const QPoint& pos )
{
	// get tree item
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->itemAt( pos ) );
	if ( treeItem != NULL ) {
		// open menu
		_menuGameObject->Open( pos, (treeItem->asset != NULL) );
	}
}


cchar* Hierarchy_Isolate::_GetPanelTitle()
{
	return PANEL_TITLE;
}
