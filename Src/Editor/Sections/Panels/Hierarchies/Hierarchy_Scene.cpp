// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Hierarchy_Scene.h"

#include "CyredModule_Scene.h"
#include "CyredModule_File.h"
#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"

#include "../../Settings/EditorSkin.h"
#include "../../Settings/EditorSettings.h"
#include "../../../Utils/CustomTree.h"
#include "../../../Utils/CustomTreeItem.h"
#include "../../Menus/Menu_GameObject.h"
#include "../../Menus/Menu_Scene.h"
#include "../../../Utils/EditorEvents.h"

#include "QtWidgets\qtreewidget.h"
#include "QtGui\qevent.h"
#include "QtWidgets\qmenu.h"
#include "QtWidgets\qfiledialog.h"


using namespace CYRED;


void Hierarchy_Scene::_OnInitialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	_CreateRightClickMenu();

	// register events
	EventManager::Singleton()->RegisterListener( this, EventType::SCENE_UPDATE );
	EventManager::Singleton()->RegisterListener( this, EventType::SCENE_OPEN );
	EventManager::Singleton()->RegisterListener( this, EventType::SCENE_CLOSE );
	EventManager::Singleton()->RegisterListener( this, EventType::GAMEOBJECT_UPDATE );
	EventManager::Singleton()->RegisterListener( this, EventType::GAMEOBJECT_RENAME );
	EventManager::Singleton()->RegisterListener( this, EventType::COMPONENT_UPDATE );
	EventManager::Singleton()->RegisterListener( this, EventType::ASSET_RENAME );
	EventManager::Singleton()->RegisterListener( this, EditorEventType::ASSET_SELECT );
	EventManager::Singleton()->RegisterListener( this, EditorEventType::GAMEOBJECT_SELECT );
	EventManager::Singleton()->RegisterListener( this, EditorEventType::PREFAB_SELECT );
}


void Hierarchy_Scene::Finalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( this, EventType::SCENE_UPDATE );
	EventManager::Singleton()->UnregisterListener( this, EventType::SCENE_OPEN );
	EventManager::Singleton()->UnregisterListener( this, EventType::SCENE_CLOSE );
	EventManager::Singleton()->UnregisterListener( this, EventType::GAMEOBJECT_UPDATE );
	EventManager::Singleton()->UnregisterListener( this, EventType::GAMEOBJECT_RENAME );
	EventManager::Singleton()->UnregisterListener( this, EventType::COMPONENT_UPDATE );
	EventManager::Singleton()->UnregisterListener( this, EventType::ASSET_RENAME );
	EventManager::Singleton()->UnregisterListener( this, EditorEventType::ASSET_SELECT );
	EventManager::Singleton()->UnregisterListener( this, EditorEventType::GAMEOBJECT_SELECT );
	EventManager::Singleton()->UnregisterListener( this, EditorEventType::PREFAB_SELECT );
}


void Hierarchy_Scene::OnEvent( int eventType, void* eventData )
{
	switch ( eventType ) {
		case EventType::SCENE_UPDATE:
		case EventType::SCENE_OPEN:
		case EventType::SCENE_CLOSE:
		case EventType::GAMEOBJECT_UPDATE:
		{
			// check state
			bool wasEmpty = (_qtTree->topLevelItemCount() == 0);
			// update 
			_ResetHierarchy();
			// change color
			ColorizePanel( !wasEmpty && _qtTree->topLevelItemCount() > 0 );
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

		case EventType::COMPONENT_UPDATE:
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
		
		case EditorEventType::ASSET_SELECT:
		case EditorEventType::PREFAB_SELECT:
			_qtTree->setCurrentItem( NULL );
			break;

		case EditorEventType::GAMEOBJECT_SELECT:
		{
			if ( eventData != NULL ) {
				GameObject* gameObject = CAST_S( GameObject*, eventData );
				CustomTreeItem* treeItem = _FindGameObjectItem( gameObject->GetUniqueID() );
				_qtTree->setCurrentItem( treeItem );
			}
			break;
		}
			
		case EventType::ASSET_RENAME:
		{
			_qtTree->setCurrentItem( NULL );

			Asset* asset = CAST_S( Asset*, eventData );

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


CustomTreeItem* Hierarchy_Scene::_FindGameObjectItem( int uid )
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


CustomTreeItem* Hierarchy_Scene::_FindSceneItem( cchar* uid )
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


void Hierarchy_Scene::_CreateRightClickMenu()
{
	ASSERT( _isInitialized );

	// create menus
	_menuGameObject = Memory::Alloc<Menu_GameObject>( _qtTree, this, EventType::SCENE_UPDATE );
	_menuScene		= Memory::Alloc<Menu_Scene>( _qtTree, this );

	// add menu to tree
	_qtTree->setContextMenuPolicy( Qt::CustomContextMenu );
	QObject::connect( _qtTree, &QWidget::customContextMenuRequested, this, &Hierarchy_Scene::A_RightClickMenu );
}


void Hierarchy_Scene::_ResetHierarchy()
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
		treeItem->node = scene->GetRoot();

		treeItem->setText( 0, scene->GetName() );
		treeItem->setFlags( Qt::ItemIsSelectable | 
							Qt::ItemIsEditable |
							Qt::ItemIsDropEnabled | 
							Qt::ItemIsEnabled );
		rootItem->addChild( treeItem );
		treeItem->setExpanded( TRUE );

		for ( int j = 0; j < scene->GetRoot()->GetChildNodeCount(); ++j ) {
			_RecResetHierarchy( scene->GetRoot()->GetChildNodeAt(j), treeItem );
		}
	}
}


void Hierarchy_Scene::A_ItemClicked( QTreeWidgetItem* item, int column )
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, item );

	if ( treeItem->asset != NULL ) {
		EventManager::Singleton()->EmitEvent( EditorEventType::SCENE_SELECT, treeItem->asset );
	}
	else {
		EventManager::Singleton()->EmitEvent( EditorEventType::GAMEOBJECT_SELECT, treeItem->node );
	}
}


void Hierarchy_Scene::A_RightClickMenu( const QPoint& pos )
{
	// get tree item
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->itemAt( pos ) );
	if ( treeItem != NULL ) {
		if ( treeItem->asset != NULL ) {
			// open gameobject menu
			_menuScene->Open( pos );
		}
		else {
			// open gameobject menu
			_menuGameObject->Open( pos, FALSE );
		}
	}
}


cchar* Hierarchy_Scene::_GetPanelTitle()
{
	return PANEL_TITLE;
}
