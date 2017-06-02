// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Hierarchy_Prefab.h"

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


void Hierarchy_Prefab::_OnInitialize()
{
	// create menus
	_menuGameObject = new Menu_GameObject( _qtTree, this );

	// add menu to tree
	_qtTree->setContextMenuPolicy( Qt::CustomContextMenu );
	QObject::connect( _qtTree, &QWidget::customContextMenuRequested, 
					  this, &Hierarchy_Prefab::A_RightClickMenu );
}


void Hierarchy_Prefab::OnEvent( int eventType, void* eventData )
{
	switch ( eventType ) {
		case EditorEventType::PREFAB_OPEN:
			_targetPrefab = CAST_S( Prefab*, eventData );
			_ResetHierarchy();
			// change color
			ColorizePanel( FALSE );
			break;

		case EditorEventType::PREFAB_CLOSE:
			if ( _targetPrefab == eventData ) {
				_targetPrefab = NULL;
				_ResetHierarchy();
				// change color
				ColorizePanel( FALSE );
				// unselect
				EventManager::Singleton()->EmitEvent( EditorEventType::PREFAB_SELECT, NULL );
			}
			break;
		
		case EditorEventType::PREFAB_UPDATE:
		case EventType::GAMEOBJECT_UPDATE:
		{
			// check state
			bool wasEmpty = (_qtTree->topLevelItemCount() == 0);
			// update 
			_ResetHierarchy();
			// search for gameobject
			CustomTreeItem* treeItem = _FindGameObjectItem( CAST_S( GameObject*, eventData )->GetUniqueID() );
			if ( treeItem != NULL ) {
				// change color
				ColorizePanel( !wasEmpty );
			}
			break;
		}

		case EventType::COMPONENT_UPDATE:
		case EventType::COMPONENT_ADD:
		case EventType::COMPONENT_REMOVE:
		{
			Component* component = CAST_S( Component*, eventData );
			if ( component != NULL ) {
				CustomTreeItem* treeItem = _FindGameObjectItem( component->GetGameObject()->GetUniqueID() );
				if ( treeItem != NULL ) {
					// change color
					ColorizePanel( TRUE );
				}
			}
			break;
		}

		case EventType::GAMEOBJECT_RENAME:
		{
			GameObject* gameObject = CAST_S( GameObject*, eventData );
			CustomTreeItem* treeItem = _FindGameObjectItem( gameObject->GetUniqueID() );
			if ( treeItem != NULL ) {
				treeItem->setText( 0, gameObject->GetName() );

				// change color
				ColorizePanel( TRUE );
			}
			break;
		}

		case EditorEventType::ASSET_SELECT:
		case EditorEventType::SCENE_SELECT:
		case EditorEventType::ISOLATE_SELECT:
			_qtTree->setCurrentItem( NULL );
			break;

		case EditorEventType::GAMEOBJECT_SELECT:
			if ( eventData != NULL ) {
				GameObject* gameObject = CAST_S( GameObject*, eventData );
				CustomTreeItem* treeItem = _FindGameObjectItem( gameObject->GetUniqueID() );
				_qtTree->setCurrentItem( treeItem );
			}
			break;

		case EventType::ASSET_RENAME:
		{
			_qtTree->setCurrentItem( NULL );

			Asset* asset = CAST_S( Asset*, eventData );
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


CustomTreeItem* Hierarchy_Prefab::_FindGameObjectItem( int uid )
{
	if ( uid == EMPTY_OBJECT_UID ) {
		return NULL;
	}

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


CustomTreeItem* Hierarchy_Prefab::_FindPrefabItem( cchar* uid )
{
	if ( uid == EMPTY_OBJECT_UID ) {
		return NULL;
	}

	String temp( uid );

	for ( int i = 0; i < _qtTree->topLevelItemCount(); ++i ) {
		CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->topLevelItem(i) );
		Asset* prefab = treeItem->asset;

		if ( prefab != NULL && temp == prefab->GetUniqueID() ) {
			return treeItem;
		}
	}

	return NULL;
}


void Hierarchy_Prefab::_ResetHierarchy()
{
	// delete all items
	while ( _qtTree->topLevelItemCount() > 0 ) {
		_qtTree->takeTopLevelItem( 0 );
	}

	// re-add objects
	QTreeWidgetItem* rootItem = _qtTree->invisibleRootItem();

	// sanity check
	if ( _targetPrefab != NULL ) {
		CustomTreeItem* treeItem = new CustomTreeItem();
		treeItem->asset = _targetPrefab;
		treeItem->node = _targetPrefab->GetRoot();

		treeItem->setText( 0, _targetPrefab->GetName() );
		treeItem->setFlags( Qt::ItemIsSelectable | 
							Qt::ItemIsEditable |
							Qt::ItemIsDropEnabled | 
							Qt::ItemIsEnabled );
		rootItem->addChild( treeItem );
		treeItem->setExpanded( TRUE );

		for ( int j = 0; j < _targetPrefab->GetRoot()->GetChildNodeCount(); ++j ) {
			_RecResetHierarchy( CAST_S( GameObject*, _targetPrefab->GetRoot()->GetChildNodeAt(j) ), 
								treeItem );
		}
	}
}


void Hierarchy_Prefab::A_ItemClicked( QTreeWidgetItem* item, int column )
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, item );

	// show gameobject attributes
	EventManager::Singleton()->EmitEvent( EditorEventType::GAMEOBJECT_SELECT, treeItem->node );
}


void Hierarchy_Prefab::A_RightClickMenu( const QPoint& pos )
{
	// get tree item
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->itemAt( pos ) );
	if ( treeItem != NULL ) {
		// open menu
		_menuGameObject->Open( pos, (treeItem->asset != NULL) );
	}
}


void Hierarchy_Prefab::OnAction_Isolate( GameObject* gameObject )
{
	// isolate object
	EventManager::Singleton()->EmitEvent( EditorEventType::ISOLATE_OPEN_PREFAB, gameObject );
}


cchar* Hierarchy_Prefab::_GetPanelTitle()
{
	return PANEL_TITLE;
}

