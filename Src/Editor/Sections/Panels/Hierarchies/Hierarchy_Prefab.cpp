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


void Hierarchy_Prefab::Finalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::ALL, this );
}


void Hierarchy_Prefab::OnEvent( EventType eType, void* eData )
{
	switch ( eType ) {
		case EventType::OPEN_PREFAB:
			_targetPrefab = CAST_S( Prefab*, eData );
			_ResetHierarchy();
			// change color
			ColorizePanel( FALSE );
			break;

		case EventType::CLOSE_PREFAB:
			if ( _targetPrefab == eData ) {
				_targetPrefab = NULL;
				_ResetHierarchy();
				// change color
				ColorizePanel( FALSE );

				// send event
				EventManager::Singleton()->EmitEvent( EventType::SELECT_PREFAB, NULL );
			}
			break;
		
		case EventType::CHANGE_PREFAB_HIERARCHY:
		case EventType::CHANGE_GAMEOBJECT:
		{
			// check state
			bool wasEmpty = (_qtTree->topLevelItemCount() == 0);
			// update 
			_ResetHierarchy();
			// change color
			ColorizePanel( !wasEmpty && _qtTree->topLevelItemCount() > 0 );
			break;
		}

		case EventType::CHANGE_CAMERA:
		case EventType::CHANGE_LIGHT:
		case EventType::CHANGE_MESH_RENDERING:
		case EventType::CHANGE_MORPH_RENDERING:
		case EventType::CHANGE_PARTICLE_EMITTER:
		case EventType::CHANGE_SCRIPTER:
		case EventType::CHANGE_TRANSFORM:
		{
			Component* component = CAST_S( Component*, eData );
			if ( component != NULL ) {
				CustomTreeItem* treeItem = _FindGameObjectItem( component->GetGameObject()->GetUniqueID() );
				if ( treeItem != NULL ) {
					// change color
					ColorizePanel( TRUE );
				}
			}
			break;
		}

		case EventType::RENAME_GAMEOBJECT:
		{
			GameObject* gameObject = CAST_S( GameObject*, eData );
			CustomTreeItem* treeItem = _FindGameObjectItem( gameObject->GetUniqueID() );
			if ( treeItem != NULL ) {
				treeItem->setText( 0, gameObject->GetName() );

				// change color
				ColorizePanel( TRUE );
			}
			break;
		}

		case EventType::SELECT_ASSET:
		case EventType::SELECT_SCENE:
			_qtTree->setCurrentItem( NULL );
			break;

		case EventType::SELECT_GAMEOBJECT:
			if ( eData != NULL ) {
				GameObject* gameObject = CAST_S( GameObject*, eData );
				CustomTreeItem* treeItem = _FindGameObjectItem( gameObject->GetUniqueID() );
				_qtTree->setCurrentItem( treeItem );
			}
			break;

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


CustomTreeItem* Hierarchy_Prefab::_FindGameObjectItem( int uid )
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


CustomTreeItem* Hierarchy_Prefab::_FindPrefabItem( cchar* uid )
{
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


void Hierarchy_Prefab::_CreateRightClickMenu()
{
	ASSERT( _isInitialized );

	// create menus
	_menuGameObject = Memory::Alloc<Menu_GameObject>( _qtTree, this, EventType::CHANGE_PREFAB_HIERARCHY );

	// add menu to tree
	_qtTree->setContextMenuPolicy( Qt::CustomContextMenu );
	QObject::connect( _qtTree, &QWidget::customContextMenuRequested, this, &Hierarchy_Prefab::A_RightClickMenu );
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
		CustomTreeItem* treeItem = Memory::Alloc<CustomTreeItem>();
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
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, treeItem->node );
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


cchar* Hierarchy_Prefab::_GetPanelTitle()
{
	return PANEL_TITLE;
}


void Hierarchy_Prefab::_OnInitialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	_CreateRightClickMenu();

	// register events
	EventManager::Singleton()->RegisterListener( EventType::ALL, this );
}
