// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Selector_Morph.h"
#include "../SelectorPopup.h"

#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"


using namespace CYRED;


const Char* const Selector_Morph::TYPE			= "Morph";
const Char* const Selector_Morph::OPTION_NULL	= "NULL";


void Selector_Morph::DisplayOptions( void* selected )
{
	_popup->ClearList();
	_popup->AddToList( OPTION_NULL, NULL );

	_popup->SetSelected( 0 );

	for ( UInt i = 0; i < AssetManager::Singleton()->GetMorphCount(); ++i )
	{
		Morph* morph = AssetManager::Singleton()->GetMorphAt( i );

		_popup->AddToList( morph->GetName(), morph );

		if ( selected == morph )
		{
			_popup->SetSelected( i + 1 );	// 0 reserved for NULL
		}
	}
}
