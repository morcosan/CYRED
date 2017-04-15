// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "HierarchyPanel.h"
#include "CyredModule_Scene.h"
#include "CyredModule_File.h"
#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Script.h"
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

		UInt indexInHierarchy = newParent->indexOfChild( movedItem );

		if ( newParent != NULL ) {
			if ( newParent->scene != NULL ) {
				// if scene
				movedItem->gameObject->SetParentNode( NULL );
				newParent->scene->GetRoot()->InsertChildNode( indexInHierarchy, movedItem->gameObject );
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


HierarchyPanel::HierarchyPanel()
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

	QObject::connect( _qtTree, &QTreeWidget::itemPressed, this, &HierarchyPanel::A_ItemClicked );
	QObject::connect( _qtTree, &QTreeWidget::itemChanged, this, &HierarchyPanel::A_ItemRenamed );
}


void HierarchyPanel::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	_CreateRightClickMenu();

	// register events
	EventManager::Singleton()->RegisterListener( EventType::CHANGE_HIERARCHY, this );
	EventManager::Singleton()->RegisterListener( EventType::RENAME_GAMEOBJECT, this );
	EventManager::Singleton()->RegisterListener( EventType::CHANGE_ASSET, this );
	EventManager::Singleton()->RegisterListener( EventType::SELECT_ASSET, this );
}


void HierarchyPanel::Finalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::CHANGE_HIERARCHY, this );
	EventManager::Singleton()->UnregisterListener( EventType::RENAME_GAMEOBJECT, this );
	EventManager::Singleton()->UnregisterListener( EventType::CHANGE_ASSET, this );
	EventManager::Singleton()->UnregisterListener( EventType::SELECT_ASSET, this );
}


void HierarchyPanel::OnEvent( EventType eType, void* eData )
{
	switch ( eType ) {
		case EventType::CHANGE_HIERARCHY:
			_ResetHierarchy();
			break;

		case EventType::RENAME_GAMEOBJECT:
		{
			GameObject* gameObject = CAST_S( GameObject*, eData );
			_QtTreeItem* treeItem = _FindGameObjectItem( gameObject->GetUniqueID() );
			if ( treeItem != NULL ) {
				treeItem->setText( 0, gameObject->GetName() );
			}

			break;
		}
		
		case EventType::SELECT_ASSET:
			_qtTree->setCurrentItem( NULL );
			break;

		case EventType::CHANGE_ASSET:
		{
			_qtTree->setCurrentItem( NULL );

			Asset* asset = CAST_S( Asset*, eData );

			if ( asset != NULL && asset->GetAssetType() == AssetType::SCENE ) {
				_QtTreeItem* treeItem = _FindSceneItem( asset->GetUniqueID() );

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


HierarchyPanel::_QtTreeItem* HierarchyPanel::_FindGameObjectItem( UInt uid )
{
	QTreeWidgetItemIterator it( _qtTree );
	while ( *it != NULL ) {
		_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, *it );
		GameObject* gameObject = treeItem->gameObject;

		if ( gameObject != NULL && gameObject->GetUniqueID() == uid ) {
			return treeItem;
		}
		// next
		++it;
	}

	return NULL;
}


HierarchyPanel::_QtTreeItem* HierarchyPanel::_FindSceneItem( const Char* uid )
{
	String temp( uid );

	for ( Int i = 0; i < _qtTree->topLevelItemCount(); ++i ) {
		_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->topLevelItem(i) );
		Scene* scene = treeItem->scene;

		if ( scene != NULL && temp == scene->GetUniqueID() ) {
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

	// scene only
	if ( treeItem->scene != NULL && treeItem->gameObject == NULL ) {
		QAction* actionSaveScene	= _qtRightClickMenu->addAction( MENU_SAVE_SCENE );
		QAction* actionSaveSceneAs	= _qtRightClickMenu->addAction( MENU_SAVE_SCENE_AS );
		QAction* actionCloseScene	= _qtRightClickMenu->addAction( MENU_CLOSE_SCENE );

		QObject::connect( actionSaveScene,		&QAction::triggered, this, &HierarchyPanel::A_SaveScene );
		QObject::connect( actionSaveSceneAs,	&QAction::triggered, this, &HierarchyPanel::A_SaveSceneAs );
		QObject::connect( actionCloseScene,		&QAction::triggered, this, &HierarchyPanel::A_CloseScene );
	}

	// both
	if ( treeItem->gameObject != NULL || treeItem->scene != NULL ) {
		_qtRightClickMenu->addSeparator();
		QAction* actionRename = _qtRightClickMenu->addAction( MENU_RENAME );

		QObject::connect( actionRename,	&QAction::triggered, this, &HierarchyPanel::A_Rename );
	}

	// gameobject only
	if ( treeItem->gameObject != NULL && treeItem->scene == NULL) {
		_qtRightClickMenu->addSeparator();
		QAction* actionDuplicate = _qtRightClickMenu->addAction( MENU_DUPLICATE );

		_qtRightClickMenu->addSeparator();
		QMenu* menu_AddComp = _qtRightClickMenu->addMenu( MENU_ADD_COMPONENT );
		QAction* actionComp_Camera		= menu_AddComp->addAction( MENU_COMP_CAMERA );
		QAction* actionComp_Light		= menu_AddComp->addAction( MENU_COMP_LIGHT );
		QAction* actionComp_MeshRen		= menu_AddComp->addAction( MENU_COMP_MESH_REN );
		QAction* actionComp_MorphRen	= menu_AddComp->addAction( MENU_COMP_MORPH_REN );
		QAction* actionComp_PsEmitter	= menu_AddComp->addAction( MENU_COMP_PS_EMITTER );
		QAction* actionComp_Scripter	= menu_AddComp->addAction( MENU_COMP_SCRIPTER );

		_qtRightClickMenu->addSeparator();
		QAction* actionCreatePrefab = _qtRightClickMenu->addAction( MENU_CREATE_PREFAB );

		_qtRightClickMenu->addSeparator();
		QAction* actionDelete = _qtRightClickMenu->addAction( MENU_DELETE );

		QObject::connect( actionDuplicate,		&QAction::triggered, this, &HierarchyPanel::A_Duplicate );
		QObject::connect( actionComp_Camera,	&QAction::triggered, this, &HierarchyPanel::A_AddComp_Camera );
		QObject::connect( actionComp_Light,		&QAction::triggered, this, &HierarchyPanel::A_AddComp_Light );
		QObject::connect( actionComp_MeshRen,	&QAction::triggered, this, &HierarchyPanel::A_AddComp_MeshRendering );
		QObject::connect( actionComp_MorphRen,	&QAction::triggered, this, &HierarchyPanel::A_AddComp_MorphRendering );
		QObject::connect( actionComp_PsEmitter,	&QAction::triggered, this, &HierarchyPanel::A_AddComp_ParticlesEmitter );
		QObject::connect( actionComp_Scripter,	&QAction::triggered, this, &HierarchyPanel::A_AddComp_Scripter );
		QObject::connect( actionCreatePrefab,	&QAction::triggered, this, &HierarchyPanel::A_CreatePrefab );
		QObject::connect( actionDelete,			&QAction::triggered, this, &HierarchyPanel::A_Delete );
	}

	// scene only
	if ( treeItem->gameObject == NULL && treeItem->scene != NULL ) {
		_qtRightClickMenu->addSeparator();
		QMenu* menuGO = _qtRightClickMenu->addMenu( MENU_GO );

		QAction* actionGO_Empty = menuGO->addAction( MENU_GO_EMPTY );

		QMenu* menuGO_3D = menuGO->addMenu( MENU_GO_3D );
		QAction* actionGO_3D_Pivot = menuGO_3D->addAction( MENU_GO_3D_PIVOT );
		QAction* actionGO_3D_Camera = menuGO_3D->addAction( MENU_GO_3D_CAMERA );
		QAction* actionGO_3D_Light = menuGO_3D->addAction( MENU_GO_3D_LIGHT );
		QAction* actionGO_3D_Mesh = menuGO_3D->addAction( MENU_GO_3D_MESH );
		QAction* actionGO_3D_Morph = menuGO_3D->addAction( MENU_GO_3D_MORPH );

		QMenu* menuGO_PS = menuGO->addMenu( MENU_GO_PS );
		QAction* actionGO_PS_Emitter = menuGO_PS->addAction( MENU_GO_PS_EMITTER );

		QObject::connect( actionGO_Empty,		&QAction::triggered, this, &HierarchyPanel::A_GO_CreateEmpty );
		QObject::connect( actionGO_3D_Pivot,	&QAction::triggered, this, &HierarchyPanel::A_GO_Create3D_Pivot );
		QObject::connect( actionGO_3D_Camera,	&QAction::triggered, this, &HierarchyPanel::A_GO_Create3D_Camera );
		QObject::connect( actionGO_3D_Light,	&QAction::triggered, this, &HierarchyPanel::A_GO_Create3D_Light );
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
	while ( _qtTree->topLevelItemCount() > 0 ) {
		_qtTree->takeTopLevelItem( 0 );
	}

	// re-add all scenes
	QTreeWidgetItem* rootItem = _qtTree->invisibleRootItem();

	for ( UInt i = 0; i < SceneManager::Singleton()->CountLoadedScenes(); ++i )	{
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

	if ( treeItem->gameObject != NULL ) {
		EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, treeItem->gameObject );
	}
	if ( treeItem->scene != NULL ) {
		EventManager::Singleton()->EmitEvent( EventType::SELECT_SCENE, NULL );
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

	// a new scene
	if ( scene->IsTemporary() )	{
		A_SaveSceneAs();
	}
	else {
		// write file
		SceneManager::Singleton()->SaveScene( scene->GetUniqueID() );
	}
}


void HierarchyPanel::A_SaveSceneAs()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );

	Scene* scene = treeItem->scene;
	ASSERT( scene != NULL );

	// set file filter
	FiniteString fileFilter( FILE_FILTER_SCENE, FileManager::FILE_FORMAT_SCENE );
	// open explorer popup
	QString newPath = QFileDialog::getSaveFileName( this, 
													MSG_SAVE_SCENE, 
													ProjectSettings::dirPathAssets.GetChar(), 
													fileFilter.GetChar() );
	// get selected path
	const Char* paths = newPath.toUtf8().constData();
	QFileInfo filePath( newPath );
	// open directory
	QDir dir;
	QString dirPath = dir.relativeFilePath( filePath.absolutePath() );
	dirPath.append( "/" );
	// create new asset
	const Char* newName = filePath.completeBaseName().toUtf8().constData();
	Scene* newScene = SceneManager::Singleton()->SaveSceneAs( scene->GetUniqueID(),
															  newName,
															  dirPath.toUtf8().constData() );
	// add new scene to manager
	AssetManager::Singleton()->AddScene( newScene );
	
	// update assets panel
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, newScene );

	// use new scene
	treeItem->scene = newScene;
	SceneManager::Singleton()->OpenScene( newScene->GetUniqueID() );

	// update hierarchy panel
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


void HierarchyPanel::A_Duplicate()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	if ( treeItem->gameObject != NULL ) {
		SceneManager::Singleton()->Duplicate( treeItem->gameObject );
	}
}


void HierarchyPanel::A_Delete()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	SceneManager::Singleton()->Destroy( treeItem->gameObject );
}


void HierarchyPanel::A_CreatePrefab()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	// set file filter
	FiniteString fileFilter( FILE_FILTER_PREFAB, FileManager::FILE_FORMAT_PREFAB );
	// open explorer popup
	QString newPath = QFileDialog::getSaveFileName( this, 
													MSG_SAVE_PREFAB, 
													ProjectSettings::dirPathAssets.GetChar(), 
													fileFilter.GetChar() );
	// get selected path
	const Char* paths = newPath.toUtf8().constData();
	QFileInfo filePath( newPath );
	// open directory
	QDir dir;
	QString dirPath = dir.relativeFilePath( filePath.absolutePath() );
	dirPath.append( "/" );
	// write file
	const Char* newName = filePath.completeBaseName().toUtf8().constData();
	// create new prefab asset
	Prefab* prefab = Memory::Alloc<Prefab>();
	prefab->SetEmitEvents( FALSE );
	prefab->SetName( newName );
	prefab->SetDirPath( dirPath.toUtf8().constData() );
	prefab->SetGameObject( treeItem->gameObject );
	prefab->SetUniqueID( Random::GenerateUniqueID().GetChar() );
	prefab->SetIsTemporary( FALSE );
	prefab->SetEmitEvents( TRUE );
	// add to manager
	AssetManager::Singleton()->AddPrefab( prefab );
	// save file
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, prefab );
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
	newObject->AddComponent<Transform>();
	newObject->SetName( MENU_GO_3D_PIVOT );

	EditorApp::Singleton()->ShowStatus( STATUS_NEW_GO );
}


void HierarchyPanel::A_GO_Create3D_Camera()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->scene != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->sceneIndex );
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 10) );
	newObject->SetName( MENU_GO_3D_CAMERA );

	Camera* camera = newObject->AddComponent<Camera>();
	camera->SetFovYAngle( 60 );
	camera->SetNearClipping( 0.1f );
	camera->SetFarClipping( 200.0f );

	EditorApp::Singleton()->ShowStatus( STATUS_NEW_GO );
}


void HierarchyPanel::A_GO_Create3D_Light()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->scene != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->sceneIndex );
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_LIGHT );

	newObject->AddComponent<Light>();

	EditorApp::Singleton()->ShowStatus( STATUS_NEW_GO );
}


void HierarchyPanel::A_GO_Create3D_Mesh()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->scene != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->sceneIndex );
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_MESH );

	MeshRendering* meshRender = newObject->AddComponent<MeshRendering>();

	EditorApp::Singleton()->ShowStatus( STATUS_NEW_GO );
}


void HierarchyPanel::A_GO_Create3D_Morph()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->scene != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->sceneIndex );
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_MORPH );

	MorphRendering* morphRender = newObject->AddComponent<MorphRendering>();

	EditorApp::Singleton()->ShowStatus( STATUS_NEW_GO );
}


void HierarchyPanel::A_GO_Particles_Emitter()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->scene != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->sceneIndex );
	newObject->SetName( MENU_GO_PS_EMITTER );
	
	Transform* tran = newObject->AddComponent<Transform>();
	tran->RotateByWorld( Vector3( 90, 0, 0 ) );

	newObject->AddComponent<ParticleEmitter>();

	EditorApp::Singleton()->ShowStatus( STATUS_NEW_GO );
}


void HierarchyPanel::A_AddComp_Camera()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	// add camera component
	treeItem->gameObject->AddComponent<Camera>();
}


void HierarchyPanel::A_AddComp_Light()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	// add light component
	treeItem->gameObject->AddComponent<Light>();
}


void HierarchyPanel::A_AddComp_MeshRendering()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	// add mesh rendering component
	treeItem->gameObject->AddComponent<MeshRendering>();
}


void HierarchyPanel::A_AddComp_MorphRendering()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	// add morph rendering component
	treeItem->gameObject->AddComponent<MorphRendering>();
}


void HierarchyPanel::A_AddComp_ParticlesEmitter()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	// add particles emitter component
	treeItem->gameObject->AddComponent<ParticleEmitter>();
}


void HierarchyPanel::A_AddComp_Scripter()
{
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	// add scripter component
	treeItem->gameObject->AddComponent<Scripter>();
}

