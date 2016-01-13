// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Scene.h"
#include "CyredModule_Event.h"
#include "CyredModule_Scene.h"


using namespace CYRED;



void AttrViewer_Scene::OnSelect_Target( void* target )
{
	_target = CAST_S( Scene*, target );
	
	_CreateAttrString( ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	_AddToPanel( TITLE );
}


void AttrViewer_Scene::_OnUpdate_GUI()
{
	DataUnion attr;

	_WriteAttribute( ATTR_NAME,			attr.SetString( _target->GetName() ) );
}


void AttrViewer_Scene::_OnUpdate_Target()
{
	_target->SetEmitEvents( FALSE );

	_target->SetName( _ReadAttribute( ATTR_NAME ).GetString() );

	_target->SetEmitEvents( TRUE );

	EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, _target );
}

