// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Selector_Font.h"

#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"

#include "../SelectorPopup.h"


using namespace CYRED;


cchar* const Selector_Font::TYPE		= "Font";
cchar* const Selector_Font::OPTION_NULL	= "NULL";


void Selector_Font::DisplayOptions( void* selected )
{
	_popup->ClearList();
	_popup->AddToList( OPTION_NULL, NULL );

	_popup->SetSelected( 0 );

	for ( int i = 0; i < AssetManager::Singleton()->GetFontCount(); ++i ) {
		Font* font = AssetManager::Singleton()->GetFontAt( i );

		_popup->AddToList( font->GetName(), font );

		if ( selected == font ) {
			_popup->SetSelected( i + 1 );	// 0 reserved for NULL
		}
	}
}
