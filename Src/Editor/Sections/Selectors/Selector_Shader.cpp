// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Selector_Shader.h"
#include "../SelectorPopup.h"

#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"


using namespace CYRED;


cchar* const Selector_Shader::TYPE			= "Shader";
cchar* const Selector_Shader::OPTION_NULL	= "NULL";


void Selector_Shader::DisplayOptions( void* selected )
{
	_popup->ClearList();
	_popup->AddToList( OPTION_NULL, NULL );

	_popup->SetSelected( 0 );

	for ( int i = 0; i < AssetManager::Singleton()->GetShaderCount(); ++i )
	{
		Shader* shader = AssetManager::Singleton()->GetShaderAt( i );

		_popup->AddToList( shader->GetName(), shader );

		if ( selected == shader )
		{
			_popup->SetSelected( i + 1 );	// 0 reserved for NULL
		}
	}
}
