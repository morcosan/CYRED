// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "HierarchyPanel.h"
#include "CyredModule_Scene.h"
#include "CyredModule_File.h"
#include "CyredModule_Render.h"
#include "../Settings/EditorSkin.h"
#include "../Settings/EditorSettings.h"
#include "../Settings/ProjectSettings.h"
#include "../../EditorApp.h"

#include "QtWidgets\qtreewidget.h"
#include "QtGui\qevent.h"
#include "QtWidgets\qmenu.h"
#include "QtWidgets\qfiledialog.h"


using namespace CYRED;



class HierarchyPanel::_QtTreeItem : public QTreeWidgetItem
{
public:
	Scene*		scene;
	UInt		sceneIndex;
	GameObject* gameObject;
};


class HierarchyPanel::_QtTree : public QTreeWidget
{
public:
	void dropEvent( QDropEvent* e )
	{
		// this will prevent dropping directly to root, which is outside scene
		QModelIndex droppedIndex = indexAt( e->pos() );
		if ( !droppedIndex.isValid() )
		{
			return;
		}

		_QtTreeItem* movedItem = CAST_S( _QtTreeItem*, this->currentItem() );
		QTreeWidget::dropEvent( e );	// apply the drop
		_QtTreeItem* newParent = CAST_S( _QtTreeItem*, movedItem->parent() );

		this->clearSelection(); // this will solve the QT 5.4.1 bug

		if ( newParent != NULL )
		{
			movedItem->gameObject->SetParentNode( newParent->gameObject );
		}
		else
		{
			movedItem->gameObject->SetParentNode( NULL );
		}
	}
};


HierarchyPanel::HierarchyPanel()
{
	this->setWindowTitle( PANEL_TITLE );
	this->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
	this->setAllowedAreas( Qt::DockWidgetArea::AllDockWidgetAreas );
	this->setMinimumSize( MIN_SIZE.x, MIN_SIZE.y );

	_qtTree = Memory::Alloc<_QtTree>();
	_qtTree->setHeaderHidden( true );
	_qtTree->setDragEnabled( true );
	_qtTree->setDragDropMode( QAbstractItemView::InternalMove );
	_qtTree->setObjectName( EditorSkin::HIERARCHY_TREE );
	_qtTree->setEditTriggers( QAbstractItemView::NoEditTriggers );

	this->setWidget( _qtTree );

	QObject::connect( _qtTree, &QTreeWidget::itemPressed, this, &HierarchyPanel::A_ItemClicked );
	QObject::connect( _qtTree, &QTreeWidget::itemChanged, this, &HierarchyPanel::A_ItemRenamed );
}


void HierarchyPanel::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	_CreateRightClickMenu();

	EventManager::Singleton()->RegisterListener( EventType::SCENE, this );
	EventManager::Singleton()->RegisterListener( EventType::ASSET, this );
}


void HierarchyPanel::OnEvent( EventType eType, EventName eName, void* eSource )
{
	switch ( eType )
	{
		case EventType::SCENE:
		{
			switch ( eName )
			{
				case EventName::HIERARCHY_CHANGED:
				{
					_ResetHierarchy();
					break;
				}

				case EventName::GAMEOBJECT_RENAMED:
				{
					GameObject* gameObject = CAST_S( GameObject*, eSource );
					_QtTreeItem* treeItem = _FindGameObjectItem( gameObject->GetUID() );
					if ( treeItem != NULL )
					{
						treeItem->setText( 0, gameObject->GetName() );
					}

					break;
				}
			}
			break;
		}

		case EventType::ASSET:
		{
			_qtTree->setCurrentItem( NULL );

			switch ( eName )
			{
				case EventName::ASSET_CHANGED:
				{
					Asset* asset = CAST_S( Asset*, eSource );

					if ( asset != NULL && asset->GetAssetType() == AssetType::SCENE )
					{
						_QtTreeItem* treeItem = _FindSceneItem( asset->GetUniqueID() );

						if ( treeItem != NULL )
						{
							_qtTree->blockSignals( true );
							treeItem->setText( 0, asset->GetName() );
							_qtTree->blockSignals( false );
						}
					}
				}
			}
			break;
		}
	}
}


HierarchyPanel::_QtTreeItem* HierarchyPanel::_FindGameObjectItem( UInt uid )
{
	QTreeWidgetItemIterator it( _qtTree );
	while ( *it != NULL ) 
	{
		_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, *it );
		GameObject* gameObject = treeItem->gameObject;

		if ( gameObject != NULL && gameObject->GetUID() == uid )
		{
			return treeItem;
		}

		++it;
	}

	return NULL;
}


HierarchyPanel::_QtTreeItem* HierarchyPanel::_FindSceneItem( const Char* uid )
{
	String temp( uid );

	for ( Int i = 0; i < _qtTree->topLevelItemCount(); ++i )
	{
		_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->topLevelItem(i) );
		Scene* scene = treeItem->scene;

		if ( scene != NULL && temp == scene->GetUniqueID() )
		{
			return treeItem;
		}
	}

	return NULL;
}


void HierarchyPanel::_CreateRightClickMenu()
{
	ASSERT( _isInitialized );

	_qtRightClickMenu = Memory::Alloc<QMenu>( this );

	_qtTree->setContextMenuPolicy( Qt::CustomContextMenu );
	QObject::connect( _qtTree, &QWidget::customContextMenuRequested, 
					  this, &HierarchyPanel::A_RightClickMenu );
}


void HierarchyPanel::_AddRightClickActions( QTreeWidgetItem* item )
{
	ASSERT( _isInitialized );

	_qtRightClickMenu->clear();

	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );

	if ( treeItem->scene != NULL )
	{
		QAction* actionSaveScene	= _qtRightClickMenu->addAction( MENU_SAVE_SCENE );
		QAction* actionSaveSceneAs	= _qtRightClickMenu->addAction( MENU_SAVE_SCENE_AS );
		QAction* actionCloseScene	= _qtRightClickMenu->addAction( MENU_CLOSE_SCENE );

		QObject::connect( actionSaveScene,		&QAction::triggered, this, &HierarchyPanel::A_SaveScene );
		QObject::connect( actionSaveSceneAs,	&QAction::triggered, this, &HierarchyPanel::A_SaveSceneAs );
		QObject::connect( actionCloseScene,		&QAction::triggered, this, &HierarchyPanel::A_CloseScene );
	}

	if ( treeItem->gameObject != NULL || treeItem->scene != NULL )
	{
		_qtRightClickMenu->addSeparator();
		QAction* actionRename = _qtRightClickMenu->addAction( MENU_RENAME );

		QObject::connect( actionRename,	&QAction::triggered, this, &HierarchyPanel::A_Rename );
	}
	if ( treeItem->gameObject != NULL )
	{
		_qtRightClickMenu->addSeparator();
		QAction* actionDelete = _qtRightClickMenu->addAction( MENU_DELETE );

		QObject::connect( actionDelete,	&QAction::triggered, this, &HierarchyPanel::A_Delete );
	}

	if ( treeItem->gameObject == NULL )
	{
		_qtRightClickMenu->addSeparator();
		QMenu* menuGO = _qtRightClickMenu->addMenu( MENU_GO );

		QAction* actionGO_Empty = menuGO->addAction( MENU_GO_EMPTY );

		QMenu* menuGO_3D = menuGO->addMenu( MENU_GO_3D );
		QAction* actionGO_3D_Pivot = menuGO_3D->addAction( MENU_GO_3D_PIVOT );
		QAction* actionGO_3D_Camera = menuGO_3D->addAction( MENU_GO_3D_CAMERA );
		QAction* actionGO_3D_Mesh = menuGO_3D->addAction( MENU_GO_3D_MESH );
		QAction* actionGO_3D_Morph = menuGO_3D->addAction( MENU_GO_3D_MORPH );

		QMenu* menuGO_PS = menuGO->addMenu( MENU_GO_PS );
		QAction* actionGO_PS_Emitter = menuGO_PS->addAction( MENU_GO_PS_EMITTER );

		QObject::connect( actionGO_Empty,		&QAction::triggered, this, &HierarchyPanel::A_GO_CreateEmpty );
		QObject::connect( actionGO_3D_Pivot,	&QAction::triggered, this, &HierarchyPanel::A_GO_Create3D_Pivot );
		QObject::connect( actionGO_3D_Camera,	&QAction::triggered, this, &HierarchyPanel::A_GO_Create3D_Camera );
		QObject::connect( actionGO_3D_Mesh,		&QAction::triggered, this, &HierarchyPanel::A_GO_Create3D_Mesh );
		QObject::connect( actionGO_3D_Morph,	&QAction::triggered, this, &HierarchyPanel::A_GO_Create3D_Morph );
		QObject::connect( actionGO_PS_Emitter,	&QAction::triggered, this, &HierarchyPanel::A_GO_Particles_Emitter );
	}
}


void HierarchyPanel::_RecResetHierarchy( GameObject* gameObject, QTreeWidgetItem* parent )
{
	ASSERT( gameObject != NULL );

	_QtTreeItem* treeItem = Memory::Alloc<_QtTreeItem>();
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


void HierarchyPanel::_ResetHierarchy()
{
	// delete all items
	while ( _qtTree->topLevelItemCount() > 0 )
	{
		_qtTree->takeTopLevelItem( 0 );
	}

	// re-add all scenes
	QTreeWidgetItem* rootItem = _qtTree->invisibleRootItem();

	for ( UInt i = 0; i < SceneManager::Singleton()->CountLoadedScenes(); ++i )
	{
		Scene* scene = SceneManager::Singleton()->GetScene( i );
		
		_QtTreeItem* treeItem = Memory::Alloc<_QtTreeItem>();
		treeItem->scene = scene;
		treeItem->sceneIndex = i;

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


void HierarchyPanel::A_ItemClicked( QTreeWidgetItem* item, int column )
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, item );

	if ( treeItem->gameObject != NULL )
	{
		EventManager::Singleton()->EmitEvent( EventType::SCENE, 
											  EventName::GAMEOBJECT_SELECTED, 
											  treeItem->gameObject );
	}

	if ( treeItem->scene != NULL )
	{
		EventManager::Singleton()->EmitEvent( EventType::SCENE, 
											  EventName::SCENE_SELECTED, 
											  NULL );
	}
}


void HierarchyPanel::A_ItemRenamed( QTreeWidgetItem* item, int column )
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, item );

	if ( treeItem->gameObject != NULL )
	{
		treeItem->gameObject->SetName( item->text( 0 ).toUtf8().data() );
	}
	if ( treeItem->scene != NULL )
	{
		FiniteString newName( item->text(0).toUtf8().constData() );

		_qtTree->blockSignals( true );
		item->setText( 0, treeItem->scene->GetName() );
		_qtTree->blockSignals( false );

		treeItem->scene->SetName( newName.GetChar() );
	}
}


void HierarchyPanel::A_RightClickMenu( const QPoint& pos )
{
	QTreeWidgetItem* item = _qtTree->itemAt( pos );

	if ( item != NULL )
	{
		_AddRightClickActions( item );

		_qtRightClickMenu->popup( _qtTree->mapToGlobal(pos) );
	}
}


void HierarchyPanel::A_SaveScene()
{
	Scene* scene = CAST_S( _QtTreeItem*, _qtTree->currentItem() )->scene;
	ASSERT( scene != NULL );

	if ( scene->IsTemporary() )		// a new scene
	{
		A_SaveSceneAs();
	}
	else
	{
		SceneManager::Singleton()->SaveScene( scene->GetUniqueID() );
	}
}


void HierarchyPanel::A_SaveSceneAs()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );

	Scene* scene = treeItem->scene;
	ASSERT( scene != NULL );

	FiniteString fileFilter( FILE_FILTER_SCENE, FileManager::FILE_FORMAT_SCENE );

	QString newPath = QFileDialog::getSaveFileName( this, 
													MSG_SAVE_SCENE, 
													ProjectSettings::dirPathAssets.GetChar(), 
													fileFilter.GetChar() );
	
	const char* paths = newPath.toUtf8().constData();
	QFileInfo filePath( newPath );

	QDir dir;
	QString dirPath = dir.relativeFilePath( filePath.absolutePath() );
	dirPath.append( "/" );

	const Char* newName = filePath.completeBaseName().toUtf8().constData();
	Scene* newScene = SceneManager::Singleton()->SaveSceneAs( scene->GetUniqueID(),
															  newName,
															  dirPath.toUtf8().constData() );

	treeItem->scene = newScene;
	
	_qtTree->blockSignals( true );
	treeItem->setText( 0, newScene->GetName() );
	_qtTree->blockSignals( false );
}


void HierarchyPanel::A_CloseScene()
{
	Scene* scene = CAST_S( _QtTreeItem*, _qtTree->currentItem() )->scene;
	ASSERT( scene != NULL );

	SceneManager::Singleton()->CloseScene( scene->GetUniqueID() );
}


void HierarchyPanel::A_Rename()
{
	QTreeWidgetItem* item = _qtTree->currentItem();
	_qtTree->editItem( item );
}


void HierarchyPanel::A_Delete()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	SceneManager::Singleton()->Destroy( treeItem->gameObject );
}


void HierarchyPanel::A_GO_CreateEmpty()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->scene != NULL );

	SceneManager::Singleton()->NewGameObject( treeItem->sceneIndex );
	EditorApp::Singleton()->ShowStatus( STATUS_NEW_GO );
}


void HierarchyPanel::A_GO_Create3D_Pivot()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->scene != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->sceneIndex );
	newObject->AddComponent<COMP::Transform>();
	newObject->SetName( MENU_GO_3D_PIVOT );

	EditorApp::Singleton()->ShowStatus( STATUS_NEW_GO );
}


void HierarchyPanel::A_GO_Create3D_Camera()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->scene != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->sceneIndex );
	newObject->AddComponent<COMP::Transform>()->SetPositionWorld( Vector3(0, 0, 10) );
	newObject->SetName( MENU_GO_3D_CAMERA );

	COMP::Camera* camera = newObject->AddComponent<COMP::Camera>();
	camera->SetFovYAngle( 60 );
	camera->SetNearClipping( 0.1f );
	camera->SetFarClipping( 200.0f );

	EditorApp::Singleton()->ShowStatus( STATUS_NEW_GO );
}


void HierarchyPanel::A_GO_Create3D_Mesh()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->scene != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->sceneIndex );
	newObject->AddComponent<COMP::Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_MESH );

	COMP::MeshRendering* meshRender = newObject->AddComponent<COMP::MeshRendering>();

	EditorApp::Singleton()->ShowStatus( STATUS_NEW_GO );
}


void HierarchyPanel::A_GO_Create3D_Morph()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->scene != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->sceneIndex );
	newObject->AddComponent<COMP::Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_MORPH );

	COMP::MorphRendering* morphRender = newObject->AddComponent<COMP::MorphRendering>();

	EditorApp::Singleton()->ShowStatus( STATUS_NEW_GO );
}


void HierarchyPanel::A_GO_Particles_Emitter()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->scene != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->sceneIndex );
	newObject->SetName( MENU_GO_PS_EMITTER );
	
	COMP::Transform* tran = newObject->AddComponent<COMP::Transform>();
	tran->RotateByWorld( Vector3( 90, 0, 0 ) );

	newObject->AddComponent<COMP::ParticleEmitter>();

	EditorApp::Singleton()->ShowStatus( STATUS_NEW_GO );
}