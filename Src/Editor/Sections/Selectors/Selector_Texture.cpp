// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Selector_Texture.h"
#include "../SelectorPopup.h"

#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"


using namespace CYRED;


const char* const Selector_Texture::TYPE		= "Texture";
const char* const Selector_Texture::OPTION_NULL	= "NULL";


void Selector_Texture::DisplayOptions( void* selected )
{
	_popup->ClearList();
	_popup->AddToList( OPTION_NULL, NULL );

	_popup->SetSelected( 0 );

	for ( int i = 0; i < AssetManager::Singleton()->GetTextureCount(); ++i )
	{
		Texture* texture = AssetManager::Singleton()->GetTextureAt( i );

		_popup->AddToList( texture->GetName(), texture );

		if ( selected == texture )
		{
			_popup->SetSelected( i + 1 );	// 0 reserved for NULL
		}
	}
}
