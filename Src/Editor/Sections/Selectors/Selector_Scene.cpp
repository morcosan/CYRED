// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Selector_Scene.h"
#include "../SelectorPopup.h"

#include "CyredModule_Asset.h"
#include "CyredModule_Scene.h"


using namespace CYRED;


cchar* const Selector_Scene::TYPE			= "Scene";
cchar* const Selector_Scene::OPTION_NULL	= "NULL";


void Selector_Scene::DisplayOptions( void* selected )
{
	_popup->ClearList();
	_popup->AddToList( OPTION_NULL, NULL );

	_popup->SetSelected( 0 );

	for ( int i = 0; i < AssetManager::Singleton()->GetSceneCount(); ++i )
	{
		Scene* scene = AssetManager::Singleton()->GetSceneAt( i );

		_popup->AddToList( scene->GetName(), scene );

		if ( selected == scene )
		{
			_popup->SetSelected( i + 1 );	// 0 reserved for NULL
		}
	}
}
