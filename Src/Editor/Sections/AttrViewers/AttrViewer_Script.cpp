// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Script.h"
#include "CyredModule_Script.h"
#include "CyredModule_Event.h"
#include "../Selectors/Selector_Prefab.h"


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
	_CreateAttrString( ATTR_NAME, ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	_CreateAttrBool( ATTR_RUN_EDITOR,	ATTR_RUN_EDITOR );
	_CreateAttrList( ATTR_FILE_PATHS,	ATTR_FILE_PATHS, AttrType::STRING );

	// add variables
	_OpenGroup( GROUP_VARIABLES );
	{
		Iterator<String, DataUnion> iter = _target->GetVarsListIterator();
		while ( iter.HasNext() ) {
			// add attribute name
			DataUnion::ValueType varType = iter.GetValue().GetValueType();
			FiniteString attrName( "%s%s", _target->GetUniqueID(), iter.GetKey().GetChar() );

			switch ( varType ) {
				case DataUnion::BOOL:
					_CreateAttrBool( attrName.GetChar(), iter.GetKey().GetChar(), 
									 AttrFlag::READONLY, CallbackGroup::GROUP_1 );
					break;

				case DataUnion::INT:
					_CreateAttrInt( attrName.GetChar(), iter.GetKey().GetChar(), 
									AttrFlag::READONLY, CallbackGroup::GROUP_1 );
					break;

				case DataUnion::FLOAT:
					_CreateAttrFloat( attrName.GetChar(), iter.GetKey().GetChar(), 
									  AttrFlag::READONLY, CallbackGroup::GROUP_1 );
					break;

				case DataUnion::STRING:
					_CreateAttrString( attrName.GetChar(), iter.GetKey().GetChar(), 
									   AttrFlag::READONLY, CallbackGroup::GROUP_1 );
					break;

				case DataUnion::VECTOR2:
					_CreateAttrVector2( attrName.GetChar(), iter.GetKey().GetChar(), 
										AttrFlag::READONLY, CallbackGroup::GROUP_1 );
					break;

				case DataUnion::VECTOR3:
					_CreateAttrVector3( attrName.GetChar(), iter.GetKey().GetChar(), 
										AttrFlag::READONLY, CallbackGroup::GROUP_1 );
					break;

				case DataUnion::VECTOR4:
					_CreateAttrVector4( attrName.GetChar(), iter.GetKey().GetChar(), 
										AttrFlag::READONLY, CallbackGroup::GROUP_1 );
					break;

				case DataUnion::REFERENCE:
					_CreateAttrSelector( attrName.GetChar(), iter.GetKey().GetChar(),
										 Selector_Prefab::TYPE,
										 AttrFlag::READONLY, CallbackGroup::GROUP_1 );
					break;
			}

			iter.Next();
		}
	}
	_CloseGroup();

	// add functions
	_OpenGroup( GROUP_FUNCTIONS );
	{
		Iterator<String, DataArray<Script::LuaFunc>> iter = _target->GetFuncListIterator();
		while ( iter.HasNext() ) {
			FiniteString attrName( "%s%s", _target->GetUniqueID(), iter.GetKey().GetChar() );
			_CreateAttrLabel( attrName.GetChar(), iter.GetKey().GetChar() );

			iter.Next();
		}
	}
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
	for ( int i = 0; i < _target->GetPathsCount(); ++i ) {
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

		int numFilePaths = _ReadAttrListSize( ATTR_FILE_PATHS );
		// clear list
		_target->ClearFilePaths();
		// add all again
		for ( int i = 0; i < numFilePaths; ++i ) {
			_target->SetFilePath( i, _ReadAttrListIndex( ATTR_FILE_PATHS, i ).GetString() );
		}
	}

	_target->SetEmitEvents( TRUE );

	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, _target );
}



