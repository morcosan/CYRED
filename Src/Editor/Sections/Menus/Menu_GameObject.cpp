// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Menu_GameObject.h"

#include "CyredBuildingBlocks.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"
#include "CyredModule_File.h"
#include "CyredModule_Script.h"

#include "../../EditorApp.h"
#include "../../Utils/CustomTreeItem.h"
#include "../Settings/ProjectSettings.h"
#include "../Panels/Panel_Hierarchy.h"

#include "QtWidgets\qtreewidget.h"
#include "QtWidgets\qfiledialog.h"


using namespace CYRED;


Menu_GameObject::Menu_GameObject( QTreeWidget* qtTree, Panel_Hierarchy* panel, EventType eventType )
	: QMenu( qtTree )
	, _qtTree( qtTree )
	, _eventType( eventType )
	, _panel( panel )
{
}


void Menu_GameObject::Open( const QPoint& pos, bool isPrefab )
{
	// create custom menu
	this->clear();

	// add actions

	if ( isPrefab ) {
		QAction* actionSaveScene		= this->addAction( MENU_SAVE_PREFAB );
		QAction* actionSaveSceneAs		= this->addAction( MENU_SAVE_PREFAB_AS );
		QAction* actionCloseScene		= this->addAction( MENU_CLOSE_PREFAB );

		this->addSeparator();

		// add callbacks
		QObject::connect( actionSaveScene,	&QAction::triggered, this, &Menu_GameObject::A_SavePrefab );
		QObject::connect( actionSaveSceneAs,&QAction::triggered, this, &Menu_GameObject::A_SavePrefabAs );
		QObject::connect( actionCloseScene,	&QAction::triggered, this, &Menu_GameObject::A_ClosePrefab );
	}

	QAction* actionRename			= this->addAction( MENU_RENAME );
	if ( !isPrefab ) {
		QAction* actionDuplicate = this->addAction( MENU_DUPLICATE );
		QObject::connect( actionDuplicate, &QAction::triggered, this, &Menu_GameObject::A_Duplicate );
	}

	this->addSeparator();

	if ( !isPrefab ) {
		QAction* actionCreatePrefab = this->addAction( MENU_CREATE_PREFAB );
		this->addSeparator();
		QObject::connect( actionCreatePrefab, &QAction::triggered, this, &Menu_GameObject::A_CreatePrefab );
	}

	QMenu* menu_AddComp				= this->addMenu( MENU_ADD_COMPONENT );
	QAction* actionComp_Transform	= menu_AddComp->addAction( MENU_COMP_TRANSFORM );
	QAction* actionComp_Camera		= menu_AddComp->addAction( MENU_COMP_CAMERA );
	QAction* actionComp_Light		= menu_AddComp->addAction( MENU_COMP_LIGHT );
	QAction* actionComp_MeshRen		= menu_AddComp->addAction( MENU_COMP_MESH_REN );
	QAction* actionComp_MorphRen	= menu_AddComp->addAction( MENU_COMP_MORPH_REN );
	QAction* actionComp_PsEmitter	= menu_AddComp->addAction( MENU_COMP_PS_EMITTER );
	QAction* actionComp_Scripter	= menu_AddComp->addAction( MENU_COMP_SCRIPTER );

	this->addSeparator();

	QMenu* menuGO					= this->addMenu( MENU_GO );
	QAction* actionGO_Empty			= menuGO->addAction( MENU_GO_EMPTY );

	QMenu*	 menuGO_3D				= menuGO->addMenu( MENU_GO_3D );
	QAction* actionGO_3D_Pivot		= menuGO_3D->addAction( MENU_GO_3D_PIVOT );
	QAction* actionGO_3D_Camera		= menuGO_3D->addAction( MENU_GO_3D_CAMERA );
	QAction* actionGO_3D_Light		= menuGO_3D->addAction( MENU_GO_3D_LIGHT );
	QAction* actionGO_3D_Mesh		= menuGO_3D->addAction( MENU_GO_3D_MESH );
	QAction* actionGO_3D_Morph		= menuGO_3D->addAction( MENU_GO_3D_MORPH );

	QMenu* menuGO_PS				= menuGO->addMenu( MENU_GO_PS );
	QAction* actionGO_PS_Emitter	= menuGO_PS->addAction( MENU_GO_PS_EMITTER );

	if ( !isPrefab ) {
		this->addSeparator();
		QAction* actionDelete = this->addAction( MENU_DELETE );
		QObject::connect( actionDelete,	&QAction::triggered, this, &Menu_GameObject::A_Delete );
	}

	// add callbacks
	QObject::connect( actionRename,			&QAction::triggered, this, &Menu_GameObject::A_Rename );
	QObject::connect( actionComp_Transform,	&QAction::triggered, this, &Menu_GameObject::A_AddComp_Transform );
	QObject::connect( actionComp_Camera,	&QAction::triggered, this, &Menu_GameObject::A_AddComp_Camera );
	QObject::connect( actionComp_Light,		&QAction::triggered, this, &Menu_GameObject::A_AddComp_Light );
	QObject::connect( actionComp_MeshRen,	&QAction::triggered, this, &Menu_GameObject::A_AddComp_MeshRendering );
	QObject::connect( actionComp_MorphRen,	&QAction::triggered, this, &Menu_GameObject::A_AddComp_MorphRendering );
	QObject::connect( actionComp_PsEmitter,	&QAction::triggered, this, &Menu_GameObject::A_AddComp_ParticlesEmitter );
	QObject::connect( actionComp_Scripter,	&QAction::triggered, this, &Menu_GameObject::A_AddComp_Scripter );
	QObject::connect( actionGO_Empty,		&QAction::triggered, this, &Menu_GameObject::A_GO_CreateEmpty );
	QObject::connect( actionGO_3D_Pivot,	&QAction::triggered, this, &Menu_GameObject::A_GO_Create3D_Pivot );
	QObject::connect( actionGO_3D_Camera,	&QAction::triggered, this, &Menu_GameObject::A_GO_Create3D_Camera );
	QObject::connect( actionGO_3D_Light,	&QAction::triggered, this, &Menu_GameObject::A_GO_Create3D_Light );
	QObject::connect( actionGO_3D_Mesh,		&QAction::triggered, this, &Menu_GameObject::A_GO_Create3D_Mesh );
	QObject::connect( actionGO_3D_Morph,	&QAction::triggered, this, &Menu_GameObject::A_GO_Create3D_Morph );
	QObject::connect( actionGO_PS_Emitter,	&QAction::triggered, this, &Menu_GameObject::A_GO_Particles_Emitter );

	// display menu
	this->popup( _qtTree->mapToGlobal(pos) );
}



void Menu_GameObject::A_SavePrefab()
{
	Asset* prefab = CAST_S( CustomTreeItem*, _qtTree->currentItem() )->asset;
	ASSERT( prefab != NULL );

	// a new prefab
	if ( prefab->IsTemporary() )	{
		A_SavePrefabAs();
	}
	else {
		// save file
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, prefab );

		// colorize panel
		_panel->ColorizePanel( FALSE );
	}
}


void Menu_GameObject::A_SavePrefabAs()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );

	Asset* prefab = treeItem->asset;
	ASSERT( prefab != NULL );

	// set file filter
	FiniteString fileFilter( FILE_FILTER_PREFAB, FileManager::FILE_FORMAT_PREFAB );
	// open explorer popup
	QString newPath = QFileDialog::getSaveFileName( this, MSG_SAVE_PREFAB, 
													ProjectSettings::dirPathAssets.GetChar(), 
													fileFilter.GetChar() );
	if ( newPath != NULL ) {
		// get selected path
		cchar* paths = newPath.toUtf8().constData();
		QFileInfo filePath( newPath );
		// open directory
		QDir dir;
		QString dirPath = dir.relativeFilePath( filePath.absolutePath() );
		dirPath.append( "/" );
		// create new asset
		cchar* newName = filePath.completeBaseName().toUtf8().constData();


		// create new prefab asset
		Prefab* newPrefab = Memory::Alloc<Prefab>();
		newPrefab->SetEmitEvents( FALSE );
		newPrefab->SetName( newName );
		newPrefab->SetDirPath( dirPath.toUtf8().constData() );
		newPrefab->CreateRoot();

		// get root of prefab
		Node* root = CAST_S( Prefab*, prefab )->GetRoot();
		// clone gameobjects to new prefab
		for ( int i = 0; i < root->GetChildNodeCount(); i++ ) {
			GameObject* newObject = _CreateGameObject( newPrefab->GetRoot() );
			GameObject* prevObject = CAST_S( GameObject*, root->GetChildNodeAt( i ) );
			prevObject->Clone( newObject );
		}

		newPrefab->SetUniqueID( Random::GenerateUniqueID().GetChar() );
		newPrefab->SetIsTemporary( FALSE );
		newPrefab->SetEmitEvents( TRUE );
		// add to manager
		AssetManager::Singleton()->AddPrefab( newPrefab );
		// save file
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, newPrefab );

		// reload hierarchy
		EventManager::Singleton()->EmitEvent( EventType::OPEN_PREFAB, newPrefab );


		// update hierarchy panel
		_qtTree->blockSignals( true );
		treeItem->setText( 0, newPrefab->GetName() );
		_qtTree->blockSignals( false );


		// colorize panel
		_panel->ColorizePanel( FALSE );
	}
}


void Menu_GameObject::A_ClosePrefab()
{
	Asset* prefab = CAST_S( CustomTreeItem*, _qtTree->currentItem() )->asset;
	ASSERT( prefab != NULL );

	// reload hierarchy
	EventManager::Singleton()->EmitEvent( EventType::OPEN_PREFAB, NULL );
}


void Menu_GameObject::A_Rename()
{
	QTreeWidgetItem* item = _qtTree->currentItem();
	_qtTree->editItem( item );
}


void Menu_GameObject::A_Duplicate()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	if ( treeItem->node != NULL ) {
		// create gameobject
		GameObject* clone = _CreateGameObject( treeItem->node->GetParentNode() );
		// do the cloning
		CAST_S(GameObject*, treeItem->node)->Clone( clone );
		// update name
		clone->SetName( treeItem->node->GetName() );

		// send event
		EventManager::Singleton()->EmitEvent( _eventType, NULL );
		EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, clone );
	}
}


void Menu_GameObject::A_Delete()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// destroy object
	_DestroyGameObject( CAST_S( GameObject*, treeItem->node ) );

	// send event
	EventManager::Singleton()->EmitEvent( _eventType, NULL );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, NULL );
}


void Menu_GameObject::A_CreatePrefab()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// set file filter
	FiniteString fileFilter( FILE_FILTER_PREFAB, FileManager::FILE_FORMAT_PREFAB );
	// open explorer popup
	QString newPath = QFileDialog::getSaveFileName( this, MSG_SAVE_PREFAB, 
													ProjectSettings::dirPathAssets.GetChar(), 
													fileFilter.GetChar() );
	if ( newPath != NULL ) {
		// get selected path
		cchar* paths = newPath.toUtf8().constData();
		QFileInfo filePath( newPath );
		// open directory
		QDir dir;
		QString dirPath = dir.relativeFilePath( filePath.absolutePath() );
		dirPath.append( "/" );
		// write file
		cchar* newName = filePath.completeBaseName().toUtf8().constData();
		// create new prefab asset
		Prefab* prefab = Memory::Alloc<Prefab>();
		prefab->SetEmitEvents( FALSE );
		prefab->SetName( newName );
		prefab->SetDirPath( dirPath.toUtf8().constData() );
		prefab->CreateRoot();

		// clone gameobject to prefab
		GameObject* prefabObject = _CreateGameObject( prefab->GetRoot() );
		CAST_S( GameObject*, treeItem->node )->Clone( prefabObject );

		prefab->SetUniqueID( Random::GenerateUniqueID().GetChar() );
		prefab->SetIsTemporary( FALSE );
		prefab->SetEmitEvents( TRUE );
		// add to manager
		AssetManager::Singleton()->AddPrefab( prefab );
		// save file
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, prefab );
	}
}


void Menu_GameObject::A_GO_CreateEmpty()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// create gameobject
	GameObject* newObject = _CreateGameObject( treeItem->node );

	// send event
	EventManager::Singleton()->EmitEvent( _eventType, NULL );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


void Menu_GameObject::A_GO_Create3D_Pivot()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// create gameobject
	GameObject* newObject = _CreateGameObject( treeItem->node );
	// add transform
	newObject->AddComponent<Transform>();
	newObject->SetName( MENU_GO_3D_PIVOT );

	// send event
	EventManager::Singleton()->EmitEvent( _eventType, NULL );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


void Menu_GameObject::A_GO_Create3D_Camera()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// create gameobject
	GameObject* newObject = _CreateGameObject( treeItem->node );
	// add transform
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 10) );
	newObject->SetName( MENU_GO_3D_CAMERA );
	// add camera
	Camera* camera = newObject->AddComponent<Camera>();
	camera->SetFovYAngle( 60 );
	camera->SetNearClipping( 0.1f );
	camera->SetFarClipping( 200.0f );

	// send event
	EventManager::Singleton()->EmitEvent( _eventType, NULL );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


void Menu_GameObject::A_GO_Create3D_Light()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// create gameobject
	GameObject* newObject = _CreateGameObject( treeItem->node );
	// add transform
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_LIGHT );
	// add light
	newObject->AddComponent<Light>();

	// send event
	EventManager::Singleton()->EmitEvent( _eventType, NULL );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


void Menu_GameObject::A_GO_Create3D_Mesh()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// create gameobject
	GameObject* newObject = _CreateGameObject( treeItem->node );
	// add transform
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_MESH );
	// add mesh render
	MeshRendering* meshRender = newObject->AddComponent<MeshRendering>();

	// send event
	EventManager::Singleton()->EmitEvent( _eventType, NULL );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


void Menu_GameObject::A_GO_Create3D_Morph()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// create gameobject
	GameObject* newObject = _CreateGameObject( treeItem->node );
	// add transform
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_MORPH );
	// add morph render
	MorphRendering* morphRender = newObject->AddComponent<MorphRendering>();

	// send event
	EventManager::Singleton()->EmitEvent( _eventType, NULL );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


void Menu_GameObject::A_GO_Particles_Emitter()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// create gameobject
	GameObject* newObject = _CreateGameObject( treeItem->node );
	// add transform
	newObject->AddComponent<Transform>()->RotateByWorld( Vector3( 90, 0, 0 ) );
	newObject->SetName( MENU_GO_PS_EMITTER );
	// add particle emitter
	newObject->AddComponent<ParticleEmitter>();

	// send event
	EventManager::Singleton()->EmitEvent( _eventType, NULL );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


void Menu_GameObject::A_AddComp_Transform()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// add transform component
	CAST_S( GameObject*, treeItem->node )->AddComponent<Transform>();
}


void Menu_GameObject::A_AddComp_Camera()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// add camera component
	CAST_S( GameObject*, treeItem->node )->AddComponent<Camera>();
}


void Menu_GameObject::A_AddComp_Light()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// add light component
	CAST_S( GameObject*, treeItem->node )->AddComponent<Light>();
}


void Menu_GameObject::A_AddComp_MeshRendering()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// add mesh rendering component
	CAST_S( GameObject*, treeItem->node )->AddComponent<MeshRendering>();
}


void Menu_GameObject::A_AddComp_MorphRendering()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// add morph rendering component
	CAST_S( GameObject*, treeItem->node )->AddComponent<MorphRendering>();
}


void Menu_GameObject::A_AddComp_ParticlesEmitter()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// add particles emitter component
	CAST_S( GameObject*, treeItem->node )->AddComponent<ParticleEmitter>();
}


void Menu_GameObject::A_AddComp_Scripter()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->node != NULL );

	// add scripter component
	CAST_S( GameObject*, treeItem->node )->AddComponent<Scripter>();
}


GameObject* Menu_GameObject::_CreateGameObject( Node* parentNode )
{
	// get new uid
	int uid = SceneManager::Singleton()->NextGameObjectUID();
	// create object
	GameObject* newObject = Memory::Alloc<GameObject>( MENU_GO_EMPTY, uid );
	// add to parent
	newObject->SetParentNode( parentNode );

	return newObject;
}


void Menu_GameObject::_DestroyGameObject( GameObject* gameObject )
{
	if ( gameObject != NULL ) {
		gameObject->SetParentNode( NULL );
		Memory::Free( gameObject );
	}
}