// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "AttrViewer_Scripter.h"
#include "CyredModule_Script.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Event.h"
#include "../Selectors/Selector_Script.h"


using namespace CYRED;



void AttrViewer_Scripter::_OnInitialize()
{
	// deferred for individual target
	
	_AddToPanel( TITLE );
}


void AttrViewer_Scripter::_OnChangeTarget( void* target )
{
	_target = CAST_S( COMP::Scripter*, target );
}


void AttrViewer_Scripter::_OnUpdateGUI()
{
	// reset viewer
	_ResetViewer();

	// add attributes
	_CreateInnerAttribute( InnerAttrType::ENABLED );

	_CreateAttrListSelector( ATTR_SCRIPTS, Selector_Script::TYPE );

	// add group for each script
	for ( UInt i = 0; i < _target->GetScriptsCount(); i++ ) {
		Script* script = _target->GetScript( i );

		// add group
		if ( script != NULL ) {
			_OpenGroup( script->GetName() );
			{
				Iterator<String, Script::LuaVar> iter = script->GetVarsListIterator();
				while ( iter.HasNext() ) {
					// add attribute name
					DataUnion::ValueType varType = iter.GetValue().type;

					switch ( varType ) {
						case DataUnion::ValueType::BOOL:
							_CreateAttrBool( iter.GetKey().GetChar(), AttrFlag::READONLY, CallbackGroup::GROUP_1 );
							break;

						case DataUnion::ValueType::INT:
							_CreateAttrInt( iter.GetKey().GetChar(), AttrFlag::READONLY, CallbackGroup::GROUP_1 );
							break;

						case DataUnion::ValueType::FLOAT:
							_CreateAttrFloat( iter.GetKey().GetChar(), AttrFlag::READONLY, CallbackGroup::GROUP_1 );
							break;

						case DataUnion::ValueType::STRING:
							_CreateAttrString( iter.GetKey().GetChar(), AttrFlag::READONLY, CallbackGroup::GROUP_1 );
							break;

						case DataUnion::ValueType::VECTOR2:
							_CreateAttrVector2( iter.GetKey().GetChar(), AttrFlag::READONLY, CallbackGroup::GROUP_1 );
							break;

						case DataUnion::ValueType::VECTOR3:
							_CreateAttrVector3( iter.GetKey().GetChar(), AttrFlag::READONLY, CallbackGroup::GROUP_1 );
							break;

						case DataUnion::ValueType::VECTOR4:
							_CreateAttrVector4( iter.GetKey().GetChar(), AttrFlag::READONLY, CallbackGroup::GROUP_1 );
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
		UInt scriptsCount = _ReadAttrListSize( ATTR_SCRIPTS );
		// clear list
		_target->ClearScripts();
		// add all again
		for ( UInt i = 0; i < scriptsCount; ++i ) {
			Script* scriptAsset = CAST_S( Script*, _ReadAttrListIndex( ATTR_SCRIPTS, i ).GetReference() );
			_target->SetScript( i, (scriptAsset == NULL) ? "" : scriptAsset->GetUniqueID() );
		}

		_target->SetEnabled( _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() );
	}

	_target->SetEmitEvents( TRUE );

	// emit event manually
	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::COMPONENT, EventName::SCRIPTER_CHANGED, _target );

	_Colorize( _target->IsEnabled() );
}
