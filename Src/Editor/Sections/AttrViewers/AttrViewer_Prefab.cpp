// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Prefab.h"
#include "CyredModule_Asset.h"


using namespace CYRED;



void AttrViewer_Prefab::_OnInitialize()
{
	// deferred for individual target

	_AddToPanel( TITLE );
}


void AttrViewer_Prefab::_OnChangeTarget( void* target )
{
	_target = CAST_S( Prefab*, target );

	// reset viewer
	_ResetViewer();

	// add attributes
	_CreateAttrString( ATTR_NAME, ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	_OpenGroup( GROUP_GAMEOBJECTS );
	{
		Node* root = _target->GetRoot();
		for ( UInt i = 0; i < root->GetChildNodeCount(); i++ ) {
			GameObject* gameObject = CAST_S( GameObject*, root->GetChildNodeAt( i ) );
			FiniteString attrName( "%s%s", _target->GetUniqueID(), gameObject->GetName() );
			_CreateAttrLabel( attrName.GetChar(), gameObject->GetName() );
		}
	}
	_CloseGroup();

	// update panel
	_UpdatePanel();
}


void AttrViewer_Prefab::_OnUpdateGUI()
{
	_WriteAttrString( ATTR_NAME, _target->GetName() );
}


void AttrViewer_Prefab::_OnUpdateTarget()
{
	++_ignoreUpdateGUI;
	_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );
}

