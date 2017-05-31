// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Scene.h"
#include "CyredModule_Event.h"
#include "CyredModule_Scene.h"


using namespace CYRED;



void AttrViewer_Scene::_OnInitialize()
{
	_CreateAttrString( ATTR_NAME, ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	_AddToPanel( TITLE );
}


void AttrViewer_Scene::_OnChangeTarget( void* target )
{
	_target = CAST_S( Scene*, target );

	// reset colorize
	_Colorize( TRUE, TRUE );
}


void AttrViewer_Scene::_UpdateGUI()
{
	_WriteAttrString( ATTR_NAME, _target->GetName() );
}


void AttrViewer_Scene::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );

	_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );

	_target->SetEmitEvents( TRUE );

	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, _target );
}

