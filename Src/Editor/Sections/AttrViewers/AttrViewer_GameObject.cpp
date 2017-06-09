// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_GameObject.h"

#include "CyredModule_Event.h"


using namespace CYRED;


void AttrViewer_GameObject::_OnInitialize()
{
	_CreateAttrString	( ATTR_NAME,  ATTR_NAME,	AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );
	_CreateAttrString	( ATTR_TAG,	  ATTR_TAG,		AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );
	_CreateAttrInt		( ATTR_LAYER, ATTR_LAYER,	AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	_CreateInnerAttribute( InnerAttrType::ENABLED );
	
	_AddToPanel( TITLE );
}


void AttrViewer_GameObject::_OnChangeTarget( void* target )
{
	_target = CAST_S( GameObject*, target );
}


void AttrViewer_GameObject::_UpdateGUI()
{
	_WriteAttrString( ATTR_NAME,	_target->GetName() );
	_WriteAttrString( ATTR_TAG,		_target->GetTag() );
	_WriteAttrInt	( ATTR_LAYER,	_target->GetLayer() );
	
	if ( _target->IsEnabled() != _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() ) {
		DataUnion attr;
		_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );
	}

	// update ui
	_Colorize( _target->IsEnabled(), TRUE );
}


void AttrViewer_GameObject::_UpdateTarget()
{
	// special event for rename
	++_ignoreUpdateGUI;
	_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );

	_target->SetEmitEvents( FALSE );
	{
		_target->SetTag( _ReadAttrString( ATTR_TAG ).GetChar() );
		_target->SetLayer( _ReadAttrInt( ATTR_LAYER ) );
		_target->SetEnabled( _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() );

		// update layer, for allowed values
		_WriteAttrInt( ATTR_LAYER,	_target->GetLayer() );
	}
	_target->SetEmitEvents( TRUE );

	// emit event manually
	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::GAMEOBJECT_UPDATE, _target );

	// update ui
	_Colorize( _target->IsEnabled(), TRUE );
}

