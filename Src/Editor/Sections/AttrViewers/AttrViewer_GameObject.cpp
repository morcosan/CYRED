// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_GameObject.h"


using namespace CYRED;



void AttrViewer_GameObject::_OnInitialize()
{
	_CreateAttrString	( ATTR_NAME,	ATTR_NAME,	AttrFlag::EDIT_FINISH,	CallbackGroup::GROUP_1 );
	_CreateAttrInt		( ATTR_UID,		ATTR_UID,	AttrFlag::READONLY,		CallbackGroup::GROUP_1 );
	
	_CreateInnerAttribute( InnerAttrType::ENABLED );
	
	_AddToPanel( TITLE );
}


void AttrViewer_GameObject::_OnChangeTarget( void* target )
{
	_target = CAST_S( GameObject*, target );
}


void AttrViewer_GameObject::_OnUpdateGUI()
{
	_WriteAttrString( ATTR_NAME, _target->GetName() );
	_WriteAttrInt( ATTR_UID, _target->GetUniqueID() );
	
	if ( _target->IsEnabled() != _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() ) {
		DataUnion attr;
		_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );

		_Colorize( _target->IsEnabled(), TRUE );
	}
}


void AttrViewer_GameObject::_OnUpdateTarget()
{
	++_ignoreUpdateGUI;
	_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );

	_target->SetEnabled( _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() );

	_Colorize( _target->IsEnabled(), TRUE );
}

