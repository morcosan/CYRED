// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Menu_Scene.h"

#include "CyredBuildingBlocks.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"
#include "CyredModule_File.h"

#include "../../Utils/CustomTreeItem.h"
#include "../Settings/ProjectSettings.h"
#include "../Panels/Panel_Hierarchy.h"
#include "../../Utils/EditorEvents.h"

#include "QtWidgets\qtreewidget.h"
#include "QtWidgets\qfiledialog.h"


using namespace CYRED;


Menu_Scene::Menu_Scene( QTreeWidget* qtTree, Panel_Hierarchy* panel )
	: QMenu( qtTree )
	, _qtTree( qtTree )
	, _panel( panel )
{
	// add actions

	QAction* actionSaveScene		= this->addAction( MENU_SAVE_SCENE );
	QAction* actionSaveSceneAs		= this->addAction( MENU_SAVE_SCENE_AS );
	QAction* actionCloseScene		= this->addAction( MENU_CLOSE_SCENE );

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
	QObject::connect( actionSaveScene,		&QAction::triggered, this, &Menu_Scene::A_SaveScene );
	QObject::connect( actionSaveSceneAs,	&QAction::triggered, this, &Menu_Scene::A_SaveSceneAs );
	QObject::connect( actionCloseScene,		&QAction::triggered, this, &Menu_Scene::A_CloseScene );
	QObject::connect( actionRename,			&QAction::triggered, this, &Menu_Scene::A_Rename );
	QObject::connect( actionGO_Empty,		&QAction::triggered, this, &Menu_Scene::A_GO_CreateEmpty );
	QObject::connect( actionGO_3D_Pivot,	&QAction::triggered, this, &Menu_Scene::A_GO_Create3D_Pivot );
	QObject::connect( actionGO_3D_Camera,	&QAction::triggered, this, &Menu_Scene::A_GO_Create3D_Camera );
	QObject::connect( actionGO_3D_Light,	&QAction::triggered, this, &Menu_Scene::A_GO_Create3D_Light );
	QObject::connect( actionGO_3D_Mesh,		&QAction::triggered, this, &Menu_Scene::A_GO_Create3D_Mesh );
	QObject::connect( actionGO_3D_Morph,	&QAction::triggered, this, &Menu_Scene::A_GO_Create3D_Morph );
	QObject::connect( actionGO_PS_Emitter,	&QAction::triggered, this, &Menu_Scene::A_GO_Particles_Emitter );
}


void Menu_Scene::Open( const QPoint& pos )
{
	// display menu
	this->popup( _qtTree->mapToGlobal(pos) );
}


void Menu_Scene::A_SaveScene()
{
	Asset* scene = CAST_S( CustomTreeItem*, _qtTree->currentItem() )->asset;
	ASSERT( scene != NULL );

	// a new scene
	if ( scene->IsTemporary() )	{
		A_SaveSceneAs();
	}
	else {
		// write file
		SceneManager::Singleton()->SaveScene( scene->GetUniqueID() );

		// colorize panel
		_panel->UpdateNeedsSave( FALSE );
	}
}


void Menu_Scene::A_SaveSceneAs()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );

	Asset* scene = treeItem->asset;
	ASSERT( scene != NULL );

	// set file filter
	FiniteString fileFilter( FILE_FILTER_SCENE, FileManager::FILE_FORMAT_SCENE );
	// open explorer popup
	QString newPath = QFileDialog::getSaveFileName( this, MSG_SAVE_SCENE, 
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
		Scene* newScene = SceneManager::Singleton()->SaveSceneAs( scene->GetUniqueID(),
																  newName,
																  dirPath.toUtf8().constData() );
		// add new scene to manager
		AssetManager::Singleton()->AddScene( newScene );

		// update assets panel
		EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, newScene );

		// use new scene
		treeItem->asset = newScene;
		SceneManager::Singleton()->OpenScene( newScene->GetUniqueID() );

		// update hierarchy panel
		_qtTree->blockSignals( true );
		treeItem->setText( 0, newScene->GetName() );
		_qtTree->blockSignals( false );

		// colorize panel
		_panel->UpdateNeedsSave( FALSE );
	}
}


void Menu_Scene::A_CloseScene()
{
	Asset* scene = CAST_S( CustomTreeItem*, _qtTree->currentItem() )->asset;
	ASSERT( scene != NULL );

	SceneManager::Singleton()->CloseScene( scene->GetUniqueID() );
}


void Menu_Scene::A_Rename()
{
	QTreeWidgetItem* item = _qtTree->currentItem();
	_qtTree->editItem( item );
}


void Menu_Scene::A_GO_CreateEmpty()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->assetIndex );
	// select new object
	EventManager::Singleton()->EmitEvent( EditorEventType::GAMEOBJECT_SELECT, newObject );
}


void Menu_Scene::A_GO_Create3D_Pivot()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->assetIndex );
	newObject->AddComponent<Transform>();
	newObject->SetName( MENU_GO_3D_PIVOT );

	// select new object
	EventManager::Singleton()->EmitEvent( EditorEventType::GAMEOBJECT_SELECT, newObject );
}


void Menu_Scene::A_GO_Create3D_Camera()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->assetIndex );
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 10) );
	newObject->SetName( MENU_GO_3D_CAMERA );

	Camera* camera = newObject->AddComponent<Camera>();
	camera->SetFovYAngle( 60 );
	camera->SetNearClipping( 0.1f );
	camera->SetFarClipping( 200.0f );

	// select new object
	EventManager::Singleton()->EmitEvent( EditorEventType::GAMEOBJECT_SELECT, newObject );
}


void Menu_Scene::A_GO_Create3D_Light()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->assetIndex );
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_LIGHT );

	newObject->AddComponent<Light>();
}


void Menu_Scene::A_GO_Create3D_Mesh()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->assetIndex );
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_MESH );

	MeshRendering* meshRender = newObject->AddComponent<MeshRendering>();

	// select new object
	EventManager::Singleton()->EmitEvent( EditorEventType::GAMEOBJECT_SELECT, newObject );
}


void Menu_Scene::A_GO_Create3D_Morph()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->assetIndex );
	newObject->AddComponent<Transform>()->SetPositionWorld( Vector3(0, 0, 0) );
	newObject->SetName( MENU_GO_3D_MORPH );

	MorphRendering* morphRender = newObject->AddComponent<MorphRendering>();

	// select new object
	EventManager::Singleton()->EmitEvent( EditorEventType::GAMEOBJECT_SELECT, newObject );
}


void Menu_Scene::A_GO_Particles_Emitter()
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->currentItem() );
	ASSERT( treeItem->asset != NULL );

	GameObject* newObject = SceneManager::Singleton()->NewGameObject( treeItem->assetIndex );
	newObject->SetName( MENU_GO_PS_EMITTER );

	Transform* tran = newObject->AddComponent<Transform>();
	tran->RotateByWorld( Vector3( 90, 0, 0 ) );

	newObject->AddComponent<ParticleEmitter>();

	// select new object
	EventManager::Singleton()->EmitEvent( EditorEventType::GAMEOBJECT_SELECT, newObject );
}
