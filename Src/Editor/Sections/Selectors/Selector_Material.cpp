// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Selector_Material.h"
#include "../SelectorPopup.h"

#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"


using namespace CYRED;


const char* const Selector_Material::TYPE			= "Material";
const char* const Selector_Material::OPTION_NULL	= "NULL";


void Selector_Material::DisplayOptions( void* selected )
{
	_popup->ClearList();
	_popup->AddToList( OPTION_NULL, NULL );

	_popup->SetSelected( 0 );

	for ( int i = 0; i < AssetManager::Singleton()->GetMaterialCount(); ++i )
	{
		Material* material = AssetManager::Singleton()->GetMaterialAt( i );

		_popup->AddToList( material->GetName(), material );

		if ( selected == material )
		{
			_popup->SetSelected( i + 1 );	// 0 reserved for NULL
		}
	}
}
