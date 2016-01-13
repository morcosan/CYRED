// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_GameObject.h"


using namespace CYRED;



AttrViewer_GameObject::AttrViewer_GameObject()
	: _ignoreCounter( 0 )
{
}


void AttrViewer_GameObject::OnSelect_Target( void* target )
{
	_target = CAST_S( GameObject*, target );
	
	_CreateAttrString	( ATTR_NAME,	AttrFlag::EDIT_FINISH,	CallbackGroup::GROUP_1 );
	_CreateAttrInt		( ATTR_UID,		AttrFlag::READONLY,		CallbackGroup::GROUP_1 );
	
	_CreateInnerAttribute( InnerAttrType::ENABLED );
	
	_AddToPanel( TITLE );
}


void AttrViewer_GameObject::_OnUpdate_GUI()
{
	if ( _ignoreCounter > 0 )
	{
		--_ignoreCounter;
		return;
	}

	DataUnion attr;
	
	_WriteAttribute( ATTR_NAME, attr.SetString( _target->GetName() ) );
	_WriteAttribute( ATTR_UID,	attr.SetInt( _target->GetUID() ) );
	
	_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );

	_Colorize( _target->IsEnabled(), TRUE );
}


void AttrViewer_GameObject::_OnUpdate_Target()
{
	_ignoreCounter = 1;
	_target->SetName( _ReadAttribute( ATTR_NAME ).GetString() );

	_target->SetEnabled( _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() );

	_Colorize( _target->IsEnabled(), TRUE );
}

