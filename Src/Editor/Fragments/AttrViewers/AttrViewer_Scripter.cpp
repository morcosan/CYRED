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
	_CreateAttrSelector( ATTR_SCRIPT, Selector_Script::TYPE );

	_CreateInnerAttribute( InnerAttrType::ENABLED );
	
	_AddToPanel( TITLE );
}


void AttrViewer_Scripter::_OnChangeTarget( void* target )
{
	_target = CAST_S( COMP::Scripter*, target );
}


void AttrViewer_Scripter::_OnUpdateGUI()
{
	Script* script = _target->GetScript();
	const Char* scriptName = (script == NULL) ? Selector_Script::OPTION_NULL : script->GetName();
	_WriteAttrSelector( ATTR_SCRIPT, script, scriptName );

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
		_target->SetScript( CAST_S( Script*, _ReadAttrSelector( ATTR_SCRIPT ) ) );

		_target->SetEnabled( _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() );
	}

	_target->SetEmitEvents( TRUE );

	// emit event manually
	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::COMPONENT, 
										  EventName::SCRIPTER_CHANGED, 
										  _target );

	_Colorize( _target->IsEnabled() );
}
