// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Menu_Prefab.h"

#include "CyredBuildingBlocks.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"
#include "CyredModule_File.h"
#include "CyredModule_Event.h"

#include "../../Utils/CustomTreeItem.h"
#include "../Settings/ProjectSettings.h"

#include "QtWidgets\qtreewidget.h"
#include "QtWidgets\qfiledialog.h"


using namespace CYRED;


Menu_Prefab::Menu_Prefab( QTreeWidget* qtTree )
	: QMenu( qtTree )
	, _qtTree( qtTree )
{
	// add actions

	QAction* actionSaveScene		= this->addAction( MENU_SAVE_PREFAB );
	QAction* actionSaveSceneAs		= this->addAction( MENU_SAVE_PREFAB_AS );
	QAction* actionCloseScene		= this->addAction( MENU_CLOSE_PREFAB );

	this->addSeparator();

	QAction* actionRename			= this->addAction( MENU_RENAME );

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


	// add callbacks
	QObject::connect( actionSaveScene,		&QAction::triggered, this, &Menu_Prefab::A_SavePrefab );
	QObject::connect( actionSaveSceneAs,	&QAction::triggered, this, &Menu_Prefab::A_SavePrefabAs );
	QObject::connect( actionCloseScene,		&QAction::triggered, this, &Menu_Prefab::A_ClosePrefab );
	QObject::connect( actionRename,			&QAction::triggered, this, &Menu_Prefab::A_Rename );
	QObject::connect( actionGO_Empty,		&QAction::triggered, this, &Menu_Prefab::A_GO_CreateEmpty );
	QObject::connect( actionGO_3D_Pivot,	&QAction::triggered, this, &Menu_Prefab::A_GO_Create3D_Pivot );
	QObject::connect( actionGO_3D_Camera,	&QAction::triggered, this, &Menu_Prefab::A_GO_Create3D_Camera );
	QObject::connect( actionGO_3D_Light,	&QAction::triggered, this, &Menu_Prefab::A_GO_Create3D_Light );
	QObject::connect( actionGO_3D_Mesh,		&QAction::triggered, this, &Menu_Prefab::A_GO_Create3D_Mesh );
	QObject::connect( actionGO_3D_Morph,	&QAction::triggered, this, &Menu_Prefab::A_GO_Create3D_Morph );
	QObject::connect( actionGO_PS_Emitter,	&QAction::triggered, this, &Menu_Prefab::A_GO_Particles_Emitter );
}


void Menu_Prefab::Open( const QPoint& pos )
{
	// display menu
	this->popup( _qtTree->mapToGlobal(pos) );
}


void Menu_Prefab::A_SavePrefab()
{
	Asset* prefab = CAST_S( CustomTreeItem*, _qtTree->currentItem() )->asset;
	ASSERT( prefab != NULL );

	// save file
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, prefab );
}


void Menu_Prefab::A_SavePrefabAs()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );

	Asset* prefab = treeItem->asset;
	ASSERT( prefab != NULL );

	// set file filter
	FiniteString fileFilter( FILE_FILTER_PREFAB, FileManager::FILE_FORMAT_SCENE );
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
	// create new asset
	const Char* newName = filePath.completeBaseName().toUtf8().constData();


	// create new prefab asset
	Prefab* newPrefab = Memory::Alloc<Prefab>();
	newPrefab->SetEmitEvents( FALSE );
	newPrefab->SetName( newName );
	newPrefab->SetDirPath( dirPath.toUtf8().constData() );
	newPrefab->CreateRoot();

	// get root of prefab
	Node* root = CAST_S( Prefab*, prefab )->GetRoot();
	// clone gameobjects to new prefab
	for ( UInt i = 0; i < root->GetChildNodeCount(); i++ ) {
		GameObject* newObject = _CreateGameObject( newPrefab->GetRoot() );
		treeItem->gameObject->Clone( newObject );
	}
	
	newPrefab->SetUniqueID( Random::GenerateUniqueID().GetChar() );
	newPrefab->SetIsTemporary( FALSE );
	newPrefab->SetEmitEvents( TRUE );
	// add to manager
	AssetManager::Singleton()->AddPrefab( newPrefab );
	// save file
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, newPrefab );

	// reload hierarchy
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_PREFAB_HIERARCHY, newPrefab );


	// update hierarchy panel
	_qtTree->blockSignals( true );
	treeItem->setText( 0, newPrefab->GetName() );
	_qtTree->blockSignals( false );
}


void Menu_Prefab::A_ClosePrefab()
{
	Asset* prefab = CAST_S( CustomTreeItem*, _qtTree->currentItem() )->asset;
	ASSERT( prefab != NULL );

	// reload hierarchy
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_PREFAB_HIERARCHY, NULL );
}


void Menu_Prefab::A_Rename()
{
	QTreeWidgetItem* item = _qtTree->currentItem();
	_qtTree->editItem( item );
}


void Menu_Prefab::A_GO_CreateEmpty()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	// create gameobject
	Prefab* prefab = CAST_S( Prefab*, treeItem->asset );
	GameObject* newObject = _CreateGameObject( prefab->GetRoot() );

	// send events
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_PREFAB_HIERARCHY, prefab );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


void Menu_Prefab::A_GO_Create3D_Pivot()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	// create gameobject
	Prefab* prefab = CAST_S( Prefab*, treeItem->asset );
	GameObject* newObject = _CreateGameObject( prefab->GetRoot() );
	// add transform
	newObject->AddComponent<Transform>();
	newObject->SetName( MENU_GO_3D_PIVOT );

	// send events
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_PREFAB_HIERARCHY, prefab );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


void Menu_Prefab::A_GO_Create3D_Camera()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	// create gameobject
	Prefab* prefab = CAST_S( Prefab*, treeItem->asset );
	GameObject* newObject = _CreateGameObject( prefab->GetRoot() );
	// add transform
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 10) );
	newObject->SetName( MENU_GO_3D_CAMERA );
	// add camera
	Camera* camera = newObject->AddComponent<Camera>();
	camera->SetFovYAngle( 60 );
	camera->SetNearClipping( 0.1f );
	camera->SetFarClipping( 200.0f );

	// send events
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_PREFAB_HIERARCHY, prefab );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


void Menu_Prefab::A_GO_Create3D_Light()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	// create gameobject
	Prefab* prefab = CAST_S( Prefab*, treeItem->asset );
	GameObject* newObject = _CreateGameObject( prefab->GetRoot() );
	// add transform
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_LIGHT );
	// add light
	newObject->AddComponent<Light>();

	// send events
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_PREFAB_HIERARCHY, prefab );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


void Menu_Prefab::A_GO_Create3D_Mesh()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	// create gameobject
	Prefab* prefab = CAST_S( Prefab*, treeItem->asset );
	GameObject* newObject = _CreateGameObject( prefab->GetRoot() );
	// add transform
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_MESH );
	// add mesh render
	MeshRendering* meshRender = newObject->AddComponent<MeshRendering>();

	// send events
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_PREFAB_HIERARCHY, prefab );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


void Menu_Prefab::A_GO_Create3D_Morph()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	// create gameobject
	Prefab* prefab = CAST_S( Prefab*, treeItem->asset );
	GameObject* newObject = _CreateGameObject( prefab->GetRoot() );
	// add transform
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_MORPH );
	// add morph render
	MorphRendering* morphRender = newObject->AddComponent<MorphRendering>();

	// send events
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_PREFAB_HIERARCHY, prefab );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


void Menu_Prefab::A_GO_Particles_Emitter()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	// create gameobject
	Prefab* prefab = CAST_S( Prefab*, treeItem->asset );
	GameObject* newObject = _CreateGameObject( prefab->GetRoot() );
	// add transform
	newObject->AddComponent<Transform>()->RotateByWorld( Vector3( 90, 0, 0 ) );
	newObject->SetName( MENU_GO_PS_EMITTER );
	// add particle emitter
	newObject->AddComponent<ParticleEmitter>();

	// send events
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_PREFAB_HIERARCHY, prefab );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_GAMEOBJECT, newObject );
}


GameObject* Menu_Prefab::_CreateGameObject( Node* parentNode )
{
	// get new uid
	UInt uid = SceneManager::Singleton()->NextGameObjectUID();
	// create object
	GameObject* newObject = Memory::Alloc<GameObject>( MENU_GO_EMPTY, uid );
	// add to parent
	newObject->SetParentNode( parentNode );

	return newObject;
}
