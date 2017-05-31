// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Morph.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"


using namespace CYRED;



void AttrViewer_Morph::_OnInitialize()
{
	_CreateAttrString( ATTR_NAME, ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	_CreateAttrList( ATTR_FILE_PATHS, ATTR_FILE_PATHS, AttrType::STRING );
	
	_AddToPanel( TITLE );
}


void AttrViewer_Morph::_OnChangeTarget( void* target )
{
	_target = CAST_S( Morph*, target );
}


void AttrViewer_Morph::_UpdateGUI()
{
	_WriteAttrString( ATTR_NAME, _target->GetName() );

	_WriteAttrListSize( ATTR_FILE_PATHS, _target->GetTotalStates() );

	DataUnion attrValue;
	for ( int i = 0; i < _target->GetTotalStates(); ++i )
	{
		_WriteAttrListIndex( ATTR_FILE_PATHS, i, attrValue.SetString( _target->GetFilePath(i) ) );
	}
}


void AttrViewer_Morph::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );


	_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );

	int numSteps = _ReadAttrListSize( ATTR_FILE_PATHS );
	if ( numSteps > MORPH_LIMIT )
	{
		numSteps = MORPH_LIMIT;
		_WriteAttrListSize( ATTR_FILE_PATHS, MORPH_LIMIT );
	}

	_target->SetTotalStates( numSteps );

	for ( int i = 0; i < numSteps; ++i )
	{
		_target->SetFilePath( i, _ReadAttrListIndex( ATTR_FILE_PATHS, i ).GetString() );
	}


	_target->SetEmitEvents( TRUE );

	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, _target );
}



