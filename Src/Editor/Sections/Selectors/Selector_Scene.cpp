// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Selector_Scene.h"
#include "../SelectorPopup.h"

#include "CyredModule_Asset.h"
#include "CyredModule_Scene.h"


using namespace CYRED;


const Char* const Selector_Scene::TYPE			= "Scene";
const Char* const Selector_Scene::OPTION_NULL	= "NULL";


void Selector_Scene::DisplayOptions( void* selected )
{
	_popup->ClearList();
	_popup->AddToList( OPTION_NULL, NULL );

	_popup->SetSelected( 0 );

	for ( UInt i = 0; i < AssetManager::Singleton()->GetSceneCount(); ++i )
	{
		Scene* scene = AssetManager::Singleton()->GetSceneAt( i );

		_popup->AddToList( scene->GetName(), scene );

		if ( selected == scene )
		{
			_popup->SetSelected( i + 1 );	// 0 reserved for NULL
		}
	}
}
