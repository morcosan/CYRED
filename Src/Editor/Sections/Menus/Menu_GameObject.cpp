// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Menu_GameObject.h"

#include "CyredBuildingBlocks.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"
#include "CyredModule_File.h"
#include "CyredModule_Script.h"
#include "CyredModule_Event.h"

#include "../../EditorApp.h"
#include "../../Utils/CustomTreeItem.h"
#include "../Settings/ProjectSettings.h"

#include "QtWidgets\qtreewidget.h"
#include "QtWidgets\qfiledialog.h"


using namespace CYRED;


Menu_GameObject::Menu_GameObject( QTreeWidget* qtTree )
	: QMenu( qtTree )
	, _qtTree( qtTree )
{
	// add actions

	QAction* actionRename			= this->addAction( MENU_RENAME );
	QAction* actionDuplicate		= this->addAction( MENU_DUPLICATE );

	this->addSeparator();

	QAction* actionCreatePrefab		= this->addAction( MENU_CREATE_PREFAB );

	this->addSeparator();

	QMenu* menu_AddComp				= this->addMenu( MENU_ADD_COMPONENT );
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

	this->addSeparator();

	QAction* actionDelete			= this->addAction( MENU_DELETE );

	
	// add callbacks
	QObject::connect( actionRename,			&QAction::triggered, this, &Menu_GameObject::A_Rename );
	QObject::connect( actionDuplicate,		&QAction::triggered, this, &Menu_GameObject::A_Duplicate );
	QObject::connect( actionComp_Camera,	&QAction::triggered, this, &Menu_GameObject::A_AddComp_Camera );
	QObject::connect( actionComp_Light,		&QAction::triggered, this, &Menu_GameObject::A_AddComp_Light );
	QObject::connect( actionComp_MeshRen,	&QAction::triggered, this, &Menu_GameObject::A_AddComp_MeshRendering );
	QObject::connect( actionComp_MorphRen,	&QAction::triggered, this, &Menu_GameObject::A_AddComp_MorphRendering );
	QObject::connect( actionComp_PsEmitter,	&QAction::triggered, this, &Menu_GameObject::A_AddComp_ParticlesEmitter );
	QObject::connect( actionComp_Scripter,	&QAction::triggered, this, &Menu_GameObject::A_AddComp_Scripter );
	QObject::connect( actionCreatePrefab,	&QAction::triggered, this, &Menu_GameObject::A_CreatePrefab );
	QObject::connect( actionDelete,			&QAction::triggered, this, &Menu_GameObject::A_Delete );
	QObject::connect( actionGO_Empty,		&QAction::triggered, this, &Menu_GameObject::A_GO_CreateEmpty );
	QObject::connect( actionGO_3D_Pivot,	&QAction::triggered, this, &Menu_GameObject::A_GO_Create3D_Pivot );
	QObject::connect( actionGO_3D_Camera,	&QAction::triggered, this, &Menu_GameObject::A_GO_Create3D_Camera );
	QObject::connect( actionGO_3D_Light,	&QAction::triggered, this, &Menu_GameObject::A_GO_Create3D_Light );
	QObject::connect( actionGO_3D_Mesh,		&QAction::triggered, this, &Menu_GameObject::A_GO_Create3D_Mesh );
	QObject::connect( actionGO_3D_Morph,	&QAction::triggered, this, &Menu_GameObject::A_GO_Create3D_Morph );
	QObject::connect( actionGO_PS_Emitter,	&QAction::triggered, this, &Menu_GameObject::A_GO_Particles_Emitter );
}


void Menu_GameObject::Open( const QPoint& pos )
{
	// display menu
	this->popup( _qtTree->mapToGlobal(pos) );
}


void Menu_GameObject::A_Rename()
{
	QTreeWidgetItem* item = _qtTree->currentItem();
	_qtTree->editItem( item );
}


void Menu_GameObject::A_Duplicate()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	if ( treeItem->gameObject != NULL ) {
		SceneManager::Singleton()->Duplicate( treeItem->gameObject );
	}
}


void Menu_GameObject::A_Delete()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	SceneManager::Singleton()->Destroy( treeItem->gameObject );
}


void Menu_GameObject::A_CreatePrefab()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
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


void Menu_GameObject::A_GO_CreateEmpty()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	SceneManager::Singleton()->NewGameObject( treeItem->sceneIndex );
}


void Menu_GameObject::A_GO_Create3D_Pivot()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject();
	newObject->AddComponent<Transform>();
	newObject->SetName( MENU_GO_3D_PIVOT );
}


void Menu_GameObject::A_GO_Create3D_Camera()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject();
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 10) );
	newObject->SetName( MENU_GO_3D_CAMERA );

	Camera* camera = newObject->AddComponent<Camera>();
	camera->SetFovYAngle( 60 );
	camera->SetNearClipping( 0.1f );
	camera->SetFarClipping( 200.0f );
}


void Menu_GameObject::A_GO_Create3D_Light()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject();
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_LIGHT );

	newObject->AddComponent<Light>();
}


void Menu_GameObject::A_GO_Create3D_Mesh()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject();
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_MESH );

	MeshRendering* meshRender = newObject->AddComponent<MeshRendering>();
}


void Menu_GameObject::A_GO_Create3D_Morph()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject();
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_MORPH );

	MorphRendering* morphRender = newObject->AddComponent<MorphRendering>();
}


void Menu_GameObject::A_GO_Particles_Emitter()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject();
	newObject->SetName( MENU_GO_PS_EMITTER );

	Transform* tran = newObject->AddComponent<Transform>();
	tran->RotateByWorld( Vector3( 90, 0, 0 ) );

	newObject->AddComponent<ParticleEmitter>();
}


void Menu_GameObject::A_AddComp_Camera()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	// add camera component
	treeItem->gameObject->AddComponent<Camera>();
}


void Menu_GameObject::A_AddComp_Light()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	// add light component
	treeItem->gameObject->AddComponent<Light>();
}


void Menu_GameObject::A_AddComp_MeshRendering()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	// add mesh rendering component
	treeItem->gameObject->AddComponent<MeshRendering>();
}


void Menu_GameObject::A_AddComp_MorphRendering()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	// add morph rendering component
	treeItem->gameObject->AddComponent<MorphRendering>();
}


void Menu_GameObject::A_AddComp_ParticlesEmitter()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	// add particles emitter component
	treeItem->gameObject->AddComponent<ParticleEmitter>();
}


void Menu_GameObject::A_AddComp_Scripter()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->gameObject != NULL );

	// add scripter component
	treeItem->gameObject->AddComponent<Scripter>();
}