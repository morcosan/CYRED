// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Selector_Script.h"
#include "../SelectorPopup.h"

#include "CyredModule_Asset.h"
#include "CyredModule_Script.h"


using namespace CYRED;


const Char* const Selector_Script::TYPE			= "Morph";
const Char* const Selector_Script::OPTION_NULL	= "NULL";


void Selector_Script::DisplayOptions( void* selected )
{
	_popup->ClearList();
	_popup->AddToList( OPTION_NULL, NULL );

	_popup->SetSelected( 0 );

	for ( UInt i = 0; i < AssetManager::Singleton()->GetScriptCount(); ++i ) {
		Script* script = AssetManager::Singleton()->GetScriptAt( i );

		_popup->AddToList( script->GetName(), script );

		if ( selected == script ) {
			_popup->SetSelected( i + 1 );	// 0 reserved for NULL
		}
	}
}
