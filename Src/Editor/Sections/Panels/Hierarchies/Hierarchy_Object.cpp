// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Hierarchy_Object.h"

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


void Hierarchy_Object::Finalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::ALL, this );
}


void Hierarchy_Object::OnEvent( EventType eType, void* eData )
{
	switch ( eType ) {
		case EventType::ISOLATE_OBJECT:
			_target = CAST_S( GameObject*, eData );
			// update hierarchy
			_ResetHierarchy();
			// change color
			ColorizePanel( FALSE );
			break;

		case EventType::CHANGE_GAMEOBJECT:
		{
			// check state
			bool wasEmpty = (_qtTree->topLevelItemCount() == 0);
			// update 
			_ResetHierarchy();
			// search for gameobject
			CustomTreeItem* treeItem = _FindGameObjectItem( CAST_S( GameObject*, eData )->GetUniqueID() );
			if ( treeItem != NULL ) {
				// change color
				ColorizePanel( !wasEmpty );
			}
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
	}
}


CustomTreeItem* Hierarchy_Object::_FindGameObjectItem( int uid )
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


void Hierarchy_Object::_CreateRightClickMenu()
{
	ASSERT( _isInitialized );

	// create menus
	_menuGameObject = Memory::Alloc<Menu_GameObject>( _qtTree, this, EventType::CHANGE_PREFAB_HIERARCHY );

	// add menu to tree
	_qtTree->setContextMenuPolicy( Qt::CustomContextMenu );
	QObject::connect( _qtTree, &QWidget::customContextMenuRequested, this, &Hierarchy_Object::A_RightClickMenu );
}


void Hierarchy_Object::_ResetHierarchy()
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


void Hierarchy_Object::A_ItemClicked( QTreeWidgetItem* item, int column )
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, item );

	// show gameobject attributes
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, treeItem->node );
}


void Hierarchy_Object::A_RightClickMenu( const QPoint& pos )
{
	// get tree item
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->itemAt( pos ) );
	if ( treeItem != NULL ) {
		// open menu
		_menuGameObject->Open( pos, (treeItem->asset != NULL) );
	}
}


cchar* Hierarchy_Object::_GetPanelTitle()
{
	return PANEL_TITLE;
}


void Hierarchy_Object::_OnInitialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	_CreateRightClickMenu();

	// register events
	EventManager::Singleton()->RegisterListener( EventType::ALL, this );
}
