// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "MenuBar.h"
#include "CyredModule_Scene.h"
#include "../EditorApp.h"
#include "Builders\ProjectBuilder.h"
#include "Settings\ProjectSettings.h"

#include "QtWidgets/qactiongroup.h"


#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"


using namespace CYRED;



void MenuBar::Initialize()
{
	this->setNativeMenuBar( true );

	_AddMenu_Scene();
	_AddMenu_Project();

	// TODO
	QMenu* menu = this->addMenu( "MAGIC" );
	QObject::connect( menu->addAction( "MAGIC" ), &QAction::triggered, this, &MenuBar::A_Magic );

}


void MenuBar::AddSkins( const DataArray<cchar*>& skinsName, int defaultIndex )
{
	QMenu* menu = this->addMenu( SKINS_MENU );

	QActionGroup* actionGroup = Memory::Alloc<QActionGroup>( this );

	// connect the whole group
	QObject::connect( actionGroup, &QActionGroup::triggered, this, &MenuBar::A_Skins );

	for ( int i = 0; i < skinsName.Size(); ++i )
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


void MenuBar::_AddMenu_Scene()
{
	QMenu* menu = this->addMenu( SCENE_MENU );

	QAction* actionOpenNew = menu->addAction( SCENE_OPEN_NEW );
	QAction* actionLoadNew = menu->addAction( SCENE_LOAD_NEW );
	QAction* actionSaveAll = menu->addAction( SCENE_SAVE_ALL );
	QAction* actionCloseAll = menu->addAction( SCENE_CLOSE_ALL );

	QObject::connect( actionOpenNew, &QAction::triggered, this, &MenuBar::A_Scene_OpenNew );
	QObject::connect( actionLoadNew, &QAction::triggered, this, &MenuBar::A_Scene_LoadNew );
	QObject::connect( actionSaveAll, &QAction::triggered, this, &MenuBar::A_Scene_SaveAll );
	QObject::connect( actionCloseAll, &QAction::triggered, this, &MenuBar::A_Scene_CloseAll );
}


void MenuBar::_AddMenu_Project()
{
	QMenu* menu = this->addMenu( PROJECT_MENU );

	QAction* actionSettings = menu->addAction( PROJECT_SETTINGS );
	menu->addSeparator();
	QAction* actionBuildWin = menu->addAction( PROJECT_BUILD_WIN );

	QObject::connect( actionSettings, &QAction::triggered, this, &MenuBar::A_Project_Settings );
	QObject::connect( actionBuildWin, &QAction::triggered, this, &MenuBar::A_Project_BuildWin );
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


void MenuBar::A_Project_Settings()
{
	EventManager::Singleton()->EmitEvent( EventType::EDITOR_PROJ_SETTINGS, NULL );
}


void MenuBar::A_Project_BuildWin()
{
	ProjectBuilder::Singleton()->BuildWindows( ProjectSettings::dirPathBuildWindows.GetChar() );
}


void MenuBar::A_Skins( QAction* action )
{
	EditorApp::Singleton()->ApplySkin( action->text().toUtf8().constData() );
}


void CYRED::MenuBar::A_Magic()
{
	////char filePath[MAX_SIZE_CUSTOM_STRING];
	////CUSTOM_STRING( filePath, "%s%s", FileManager::DIR_ASSETS, "magic.scene" );

	//Scene* scene = SceneManager::Singleton()->OpenNewScene();
	////scene->SetFilePath( filePath );
	////scene->SetName( "magic" );

	//GameObject* newObject = SceneManager::Singleton()->NewGameObject();
	//newObject->SetName( "magic" );

	//Transform* tran = newObject->AddComponent<Transform>();
	//MorphRendering* morphR = newObject->AddComponent<MorphRendering>();
	//morphR->SetMaterial( AssetManager::Singleton()->GetMaterial( "ERRZIeoxy0mp9FUDxH0drcMwNhWnrGvE" ) );
	//morphR->SetMorph( AssetManager::Singleton()->GetMorph( "AKMPXdCG15fh3fVsYxOAREnyy6jnciIN" ) );
}
