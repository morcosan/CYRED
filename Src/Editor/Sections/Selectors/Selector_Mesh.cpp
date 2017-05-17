// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Selector_Mesh.h"
#include "../SelectorPopup.h"

#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"


using namespace CYRED;


const char* const Selector_Mesh::TYPE			= "Mesh";
const char* const Selector_Mesh::OPTION_NULL	= "NULL";


void Selector_Mesh::DisplayOptions( void* selected )
{
	_popup->ClearList();
	_popup->AddToList( OPTION_NULL, NULL );

	_popup->SetSelected( 0 );

	for ( int i = 0; i < AssetManager::Singleton()->GetMeshCount(); ++i )
	{
		Mesh* mesh = AssetManager::Singleton()->GetMeshAt( i );

		_popup->AddToList( mesh->GetName(), mesh );

		if ( selected == mesh )
		{
			_popup->SetSelected( i + 1 );	// 0 reserved for NULL
		}
	}
}
