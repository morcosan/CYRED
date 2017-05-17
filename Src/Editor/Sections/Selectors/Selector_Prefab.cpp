// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Selector_Prefab.h"
#include "../SelectorPopup.h"

#include "CyredModule_Asset.h"


using namespace CYRED;


const char* const Selector_Prefab::TYPE			= "Prefab";
const char* const Selector_Prefab::OPTION_NULL	= "NULL";


void Selector_Prefab::DisplayOptions( void* selected )
{
	_popup->ClearList();
	_popup->AddToList( OPTION_NULL, NULL );

	_popup->SetSelected( 0 );

	for ( int i = 0; i < AssetManager::Singleton()->GetPrefabCount(); ++i )
	{
		Prefab* prefab = AssetManager::Singleton()->GetPrefabAt( i );

		_popup->AddToList( prefab->GetName(), prefab );

		if ( selected == prefab ) {
			_popup->SetSelected( i + 1 );	// 0 reserved for NULL
		}
	}
}
