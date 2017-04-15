// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Prefab.h"
#include "CyredModule_Asset.h"


using namespace CYRED;



void AttrViewer_Prefab::_OnInitialize()
{
	_CreateAttrString	( ATTR_NAME,	ATTR_NAME,	AttrFlag::EDIT_FINISH,	CallbackGroup::GROUP_1 );

	_AddToPanel( TITLE );
}


void AttrViewer_Prefab::_OnChangeTarget( void* target )
{
	_target = CAST_S( Prefab*, target );
}


void AttrViewer_Prefab::_OnUpdateGUI()
{
	GameObject* gameObject = _target->GetGameObject();

	_WriteAttrString( ATTR_NAME, _target->GetName() );
}


void AttrViewer_Prefab::_OnUpdateTarget()
{
	++_ignoreUpdateGUI;
	_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );
}

