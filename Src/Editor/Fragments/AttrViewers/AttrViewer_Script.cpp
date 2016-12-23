// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Script.h"
#include "CyredModule_Script.h"
#include "CyredModule_Event.h"


using namespace CYRED;



void AttrViewer_Script::_OnInitialize()
{
	_CreateAttrString( ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	_CreateAttrBool( ATTR_RUN_EDITOR );
	_CreateAttrList( ATTR_FILE_PATHS, AttrType::STRING );
	
	_AddToPanel( TITLE );
}


void AttrViewer_Script::_OnChangeTarget( void* target )
{
	_target = CAST_S( Script*, target );
}


void AttrViewer_Script::_OnUpdateGUI()
{
	_WriteAttrString( ATTR_NAME, _target->GetName() );

	_WriteAttrBool( ATTR_RUN_EDITOR, _target->RunsInEditor() );

	_WriteAttrListSize( ATTR_FILE_PATHS, _target->GetPathsCount() );

	DataUnion attrValue;
	for ( UInt i = 0; i < _target->GetPathsCount(); ++i ) {
		_WriteAttrListIndex( ATTR_FILE_PATHS, i, attrValue.SetString( _target->GetFilePath(i) ) );
	}
}


void AttrViewer_Script::_OnUpdateTarget()
{
	_target->SetEmitEvents( FALSE );

	// update
	{
		_target->SetRunInEditor( _ReadAttrBool( ATTR_RUN_EDITOR ) );

		_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );

		UInt numFilePaths = _ReadAttrListSize( ATTR_FILE_PATHS );
		// clear list
		_target->ClearFilePaths();
		// add all again
		for ( UInt i = 0; i < numFilePaths; ++i ) {
			_target->SetFilePath( i, _ReadAttrListIndex( ATTR_FILE_PATHS, i ).GetString() );
		}
	}

	_target->SetEmitEvents( TRUE );

	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, _target );
}



