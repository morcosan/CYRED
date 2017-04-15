// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "AttrViewer_Scripter.h"
#include "CyredModule_Script.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Event.h"
#include "../Selectors/Selector_Script.h"
#include "../Selectors/Selector_Prefab.h"


using namespace CYRED;


void AttrViewer_Scripter::_OnInitialize()
{
	// add attributes
	_CreateInnerAttribute( InnerAttrType::ENABLED );

	// deferred for individual target
	
	_AddToPanel( TITLE );
}


void AttrViewer_Scripter::_OnChangeTarget( void* target )
{
	_target = CAST_S( Scripter*, target );
}


void AttrViewer_Scripter::_OnUpdateGUI()
{
	// reset viewer
	_ResetViewer();

	_CreateAttrListSelector( ATTR_SCRIPTS, ATTR_SCRIPTS, Selector_Script::TYPE );

	// add group for each script
	for ( UInt i = 0; i < _target->GetScriptsCount(); i++ ) {
		Script* script = _target->GetScript( i );

		// add group
		if ( script != NULL ) {
			_OpenGroup( script->GetName() );
			{
				Iterator<String, DataUnion> iter = script->GetVarsListIterator();
				while ( iter.HasNext() ) {
					// add attribute name
					DataUnion::ValueType varType = iter.GetValue().GetValueType();
					FiniteString attrName( "%s%s", script->GetUniqueID(), iter.GetKey().GetChar() );

					switch ( varType ) {
						case DataUnion::BOOL:
							_CreateAttrBool( attrName.GetChar(), iter.GetKey().GetChar(), AttrFlag::NONE, CallbackGroup::GROUP_2 );
							_WriteAttrBool( attrName.GetChar(), iter.GetValue().GetBool() );
							break;

						case DataUnion::INT:
							_CreateAttrInt( attrName.GetChar(), iter.GetKey().GetChar(), AttrFlag::NONE, CallbackGroup::GROUP_2 );
							_WriteAttrInt( attrName.GetChar(), iter.GetValue().GetInt() );
							break;

						case DataUnion::FLOAT:
							_CreateAttrFloat( attrName.GetChar(), iter.GetKey().GetChar(), AttrFlag::NONE, CallbackGroup::GROUP_2 );
							_WriteAttrFloat( attrName.GetChar(), iter.GetValue().GetFloat() );
							break;

						case DataUnion::STRING:
							_CreateAttrString( attrName.GetChar(), iter.GetKey().GetChar(), AttrFlag::NONE, CallbackGroup::GROUP_2 );
							_WriteAttrString( attrName.GetChar(), iter.GetValue().GetString() );
							break;

						case DataUnion::VECTOR2:
							_CreateAttrVector2( attrName.GetChar(), iter.GetKey().GetChar(), AttrFlag::NONE, CallbackGroup::GROUP_2 );
							_WriteAttrVector2( attrName.GetChar(), iter.GetValue().GetVector2() );
							break;

						case DataUnion::VECTOR3:
							_CreateAttrVector3( attrName.GetChar(), iter.GetKey().GetChar(), AttrFlag::NONE, CallbackGroup::GROUP_2 );
							_WriteAttrVector3( attrName.GetChar(), iter.GetValue().GetVector3() );
							break;

						case DataUnion::VECTOR4:
							_CreateAttrVector4( attrName.GetChar(), iter.GetKey().GetChar(), AttrFlag::NONE, CallbackGroup::GROUP_2 );
							_WriteAttrVector4( attrName.GetChar(), iter.GetValue().GetVector4() );
							break;

						case DataUnion::REFERENCE:
							_CreateAttrSelector( attrName.GetChar(), iter.GetKey().GetChar(), Selector_Prefab::TYPE, 
												 AttrFlag::NONE, CallbackGroup::GROUP_2 );
							if ( iter.GetValue().GetReference() != NULL ) {
								Prefab* prefab = CAST_S( Prefab*, iter.GetValue().GetReference() );
								_WriteAttrSelector( attrName.GetChar(), prefab, prefab->GetName() );
							}
							else {
								_WriteAttrSelector( attrName.GetChar(), NULL, Selector_Prefab::OPTION_NULL );
							}
							break;
					}

					iter.Next();
				}
			}
			_CloseGroup();
		}
	}

	// update panel
	_UpdatePanel();

	// display GUI
	// update scripts list
	_WriteAttrListSize( ATTR_SCRIPTS, _target->GetScriptsCount() );
	DataUnion attrValue;
	for ( UInt i = 0; i < _target->GetScriptsCount(); ++i ) {
		Script* script = _target->GetScript( i );
		const Char* scriptName = (script == NULL) ? Selector_Script::OPTION_NULL : script->GetName();
		_WriteAttrListIndex( ATTR_SCRIPTS, i, attrValue.SetReference( script ), scriptName );
	}

	if ( _target->IsEnabled() != _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() )
	{
		DataUnion attr;
		_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );

		_Colorize( _target->IsEnabled() );
	}
}


void AttrViewer_Scripter::_OnUpdateTarget()
{
	_target->SetEmitEvents( FALSE );

	// update
	{
		if ( _activatedGroup == CallbackGroup::GROUP_1 ) {
			UInt scriptsCount = _ReadAttrListSize( ATTR_SCRIPTS );
			// clear list
			_target->ClearScripts();
			// add all again
			for ( UInt i = 0; i < scriptsCount; ++i ) {
				Script* scriptAsset = CAST_S( Script*, _ReadAttrListIndex( ATTR_SCRIPTS, i ).GetReference() );
				_target->SetScript( i, (scriptAsset == NULL) ? "" : scriptAsset->GetUniqueID() );
			}

			// refresh selection

		}
		else if ( _activatedGroup == CallbackGroup::GROUP_2 ) {
			for ( UInt i = 0; i < _target->GetScriptsCount(); i++ ) {
				// get script
				Script* script = _target->GetScript( i );

				if ( script != NULL ) {
					// set variables
					Iterator<String, DataUnion> iter = script->GetVarsListIterator();
					while ( iter.HasNext() ) {
						DataUnion::ValueType varType = iter.GetValue().GetValueType();
						DataUnion varValue;
						FiniteString attrName( "%s%s", script->GetUniqueID(), iter.GetKey().GetChar() );

						// set variable value
						switch ( varType ) {
							case DataUnion::BOOL:
								varValue.SetBool( _ReadAttrBool( attrName.GetChar() ) );
								script->SetVariable( iter.GetKey().GetChar(), varValue );
								break;

							case DataUnion::INT:
								varValue.SetInt( _ReadAttrInt( attrName.GetChar() ) );
								script->SetVariable( iter.GetKey().GetChar(), varValue );
								break;

							case DataUnion::FLOAT:
								varValue.SetFloat( _ReadAttrFloat( attrName.GetChar() ) );
								script->SetVariable( iter.GetKey().GetChar(), varValue );
								break;

							case DataUnion::STRING:
								varValue.SetString( _ReadAttrString( attrName.GetChar() ).GetChar() );
								script->SetVariable( iter.GetKey().GetChar(), varValue );
								break;

							case DataUnion::VECTOR2:
								varValue.SetVector2( _ReadAttrVector2( attrName.GetChar() ) );
								script->SetVariable( iter.GetKey().GetChar(), varValue );
								break;

							case DataUnion::VECTOR3:
								varValue.SetVector3( _ReadAttrVector3( attrName.GetChar() ) );
								script->SetVariable( iter.GetKey().GetChar(), varValue );
								break;

							case DataUnion::VECTOR4:
								varValue.SetVector4( _ReadAttrVector4( attrName.GetChar() ) );
								script->SetVariable( iter.GetKey().GetChar(), varValue );
								break;

							case DataUnion::REFERENCE:
								varValue.SetReference( _ReadAttrSelector( attrName.GetChar() ) );
								script->SetVariable( iter.GetKey().GetChar(), varValue );
								break;
						}

						iter.Next();
					}
				}
			}
		}

		_target->SetEnabled( _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() );
	}

	_target->SetEmitEvents( TRUE );

	// emit event manually
	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_SCRIPTER, _target );

	_Colorize( _target->IsEnabled() );
}
