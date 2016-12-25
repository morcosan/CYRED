// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Script.h"
#include "CyredModule_Script.h"
#include "CyredModule_Event.h"


using namespace CYRED;



void AttrViewer_Script::_OnInitialize()
{
	// deferred for individual target
	
	_AddToPanel( TITLE );
}


void AttrViewer_Script::_OnChangeTarget( void* target )
{
	_target = CAST_S( Script*, target );

	// reset viewer
	_ResetViewer();

	// add attributes
	_CreateAttrString( ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	_CreateAttrBool( ATTR_RUN_EDITOR );
	_CreateAttrList( ATTR_FILE_PATHS, AttrType::STRING );

	// add variables
	_OpenGroup( GROUP_VARIABLES );
	Iterator<String, Script::LuaVar> iter = _target->GetVarsListIterator();
	while ( iter.HasNext() ) {
		// add attribute name
		DataUnion::ValueType varType = iter.GetValue().type;

		switch ( varType ) {
			case DataUnion::ValueType::BOOL:
				_CreateAttrBool( iter.GetKey().GetChar() );
				break;

			case DataUnion::ValueType::INT:
				_CreateAttrInt( iter.GetKey().GetChar() );
				break;

			case DataUnion::ValueType::FLOAT:
				_CreateAttrFloat( iter.GetKey().GetChar() );
				break;

			case DataUnion::ValueType::STRING:
				_CreateAttrString( iter.GetKey().GetChar() );
				break;

			case DataUnion::ValueType::VECTOR2:
				_CreateAttrVector2( iter.GetKey().GetChar() );
				break;

			case DataUnion::ValueType::VECTOR3:
				_CreateAttrVector3( iter.GetKey().GetChar() );
				break;

			case DataUnion::ValueType::VECTOR4:
				_CreateAttrVector4( iter.GetKey().GetChar() );
				break;
		}

		iter.Next();
	}
	_CloseGroup();

	// add functions
	_OpenGroup( GROUP_FUNCTIONS );
	_CloseGroup();

	// update panel
	_UpdatePanel();
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



