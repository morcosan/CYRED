// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_GameObject.h"


using namespace CYRED;



void AttrViewer_GameObject::_OnInitialize()
{
	_CreateAttrString	( ATTR_NAME,	ATTR_NAME,	AttrFlag::EDIT_FINISH,	CallbackGroup::GROUP_1 );

	_CreateAttrString	( ATTR_TAG,		ATTR_TAG );

	_CreateInnerAttribute( InnerAttrType::ENABLED );
	
	_AddToPanel( TITLE );
}


void AttrViewer_GameObject::_OnChangeTarget( void* target )
{
	_target = CAST_S( GameObject*, target );
}


void AttrViewer_GameObject::_UpdateGUI()
{
	_WriteAttrString( ATTR_NAME, _target->GetName() );
	_WriteAttrString( ATTR_TAG, _target->GetTag() );
	
	if ( _target->IsEnabled() != _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() ) {
		DataUnion attr;
		_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );
	}

	_Colorize( _target->IsEnabled(), TRUE );
}


void AttrViewer_GameObject::_UpdateTarget()
{
	++_ignoreUpdateGUI;
	_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );
	_target->SetTag( _ReadAttrString( ATTR_TAG ).GetChar() );

	bool prevEnabled = _target->IsEnabled();
	bool newEnabled = _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool();
	if ( prevEnabled != newEnabled ) {
		_target->SetEnabled( newEnabled );
	}

	_Colorize( _target->IsEnabled(), TRUE );
}

