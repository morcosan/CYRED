// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "MenuBar.h"
#include "CyredModule_Scene.h"
#include "../EditorApp.h"

#include "QtWidgets/qactiongroup.h"


using namespace CYRED;



void MenuBar::Initialize()
{
	this->setNativeMenuBar( true );

	_AddMenu_Scene();


	// TODO
	QMenu* menu = this->addMenu( "MAGIC" );
	QObject::connect( menu->addAction( "MAGIC" ), &QAction::triggered, this, &MenuBar::A_Magic );

}


void MenuBar::AddSkins( const DataArray<const Char*>& skinsName, UInt defaultIndex )
{
	QMenu* menu = this->addMenu( SKINS_MENU );

	QActionGroup* actionGroup = Memory::Alloc<QActionGroup>( this );

	// connect the whole group
	QObject::connect( actionGroup, &QActionGroup::triggered, this, &MenuBar::A_Skins );

	for ( UInt i = 0; i < skinsName.Size(); ++i )
	{
		QAction* action = menu->addAction( skinsName[i] );
		action->setCheckable( TRUE );

		// add also to action group
		actionGroup->addAction( action );

		if ( i == defaultIndex )
		{
			action->setChecked( TRUE );
		}
	}
}


void MenuBar::OnEvent( EventType eType, EventName eName, void* eSource )
{
}


void MenuBar::_AddMenu_Scene()
{
	QMenu* menu = this->addMenu( SCENE_MENU );

	_menuActions.Set( MenuAction::SCENE_OPEN_NEW,	menu->addAction( SCENE_OPEN_NEW ) );
	_menuActions.Set( MenuAction::SCENE_LOAD_NEW,	menu->addAction( SCENE_LOAD_NEW ) );
	_menuActions.Set( MenuAction::SCENE_SAVE_ALL,	menu->addAction( SCENE_SAVE_ALL ) );
	_menuActions.Set( MenuAction::SCENE_CLOSE_ALL,	menu->addAction( SCENE_CLOSE_ALL ) );

	QObject::connect( _menuActions.Get( MenuAction::SCENE_OPEN_NEW ),		
					  &QAction::triggered, this, &MenuBar::A_Scene_OpenNew );
	QObject::connect( _menuActions.Get( MenuAction::SCENE_LOAD_NEW ),		
					  &QAction::triggered, this, &MenuBar::A_Scene_LoadNew );
	QObject::connect( _menuActions.Get( MenuAction::SCENE_SAVE_ALL ),	
					  &QAction::triggered, this, &MenuBar::A_Scene_SaveAll );
	QObject::connect( _menuActions.Get( MenuAction::SCENE_CLOSE_ALL ),	
					  &QAction::triggered, this, &MenuBar::A_Scene_CloseAll );
}


void MenuBar::A_Scene_OpenNew()
{
	SceneManager::Singleton()->OpenNewScene();
	EditorApp::Singleton()->ShowStatus( STATUS_OPEN_NEW_SCENE );
}


void MenuBar::A_Scene_LoadNew()
{
	SceneManager::Singleton()->LoadNewScene();
	EditorApp::Singleton()->ShowStatus( STATUS_LOAD_NEW_SCENE );
}


void MenuBar::A_Scene_SaveAll()
{
	SceneManager::Singleton()->SaveAllScenes();
}


void MenuBar::A_Scene_CloseAll()
{
	SceneManager::Singleton()->CloseAllScenes();
	EditorApp::Singleton()->ShowStatus( STATUS_CLOSE_ALL_SCENES );
}


void MenuBar::A_Skins( QAction* action )
{
	EditorApp::Singleton()->ApplySkin( action->text().toUtf8().constData() );
}


void CYRED::MenuBar::A_Magic()
{
	//Char filePath[MAX_SIZE_CUSTOM_STRING];
	//CUSTOM_STRING( filePath, "%s%s", FileManager::DIR_ASSETS, "magic.scene" );

	//Scene* scene = SceneManager::Singleton()->OpenNewScene();
	//scene->SetFilePath( filePath );
	//scene->SetName( "magic" );

	//EditorApp::Singleton()->ShowStatus( STATUS_OPEN_NEW_SCENE );

	//GameObject* newObject = SceneManager::Singleton()->NewGameObject( _selectedSceneIndex );
	//newObject->SetName( "magic" );

	//COMP::Transform* tran = newObject->AddComponent<COMP::Transform>();
	//tran->RotateByWorld( Vector3( 90, 0, 0 ) );
	///*COMP::MeshRendering* meshR = newObject->AddComponent<COMP::MeshRendering>();
	//meshR->SetMaterial( AssetManager::Singleton()->GetMaterial( "Standard" ) );
	//meshR->SetMesh( AssetManager::Singleton()->GetMesh( "Cube" ) );*/


	//COMP::ParticleEmitter* emitter = newObject->AddComponent<COMP::ParticleEmitter>();
	//emitter->SetMaterial( AssetManager::Singleton()->GetMaterial( "Particles" ) );
	//emitter->BindToGPU();

	//A_Scene_SaveAll();


	//SceneManager::Singleton()->OpenScene( "magic" );
}
