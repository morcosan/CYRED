// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Scripter.h"

#include "../Assets/Script.h"
#include "../../../3_Modules/Event/EventManager.h"


using namespace CYRED;
using namespace COMP;


Scripter::Scripter( GameObject* gameObject )
	: Component( gameObject )
	, _script( NULL )
{
	_componentType = ComponentType::SCRIPTER;
}


Script* Scripter::GetScript() const
{
	return _script;
}


void Scripter::SetScript( Script* script )
{
	_script = script;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT, EventName::SCRIPTER_CHANGED, this );
	}
}


void Scripter::_OnStart( Bool isRuntime )
{
	if ( _script != NULL ) {
		// call script function
		if ( isRuntime || _script->RunsInEditor() ) {
			_script->CallFunction( "OnStart" );
			_script->SetFirstUpdate( FALSE );
		}
	}
}


void Scripter::_OnUpdate( Bool isRuntime )
{
	if ( _script != NULL ) {
		// call script function
		if ( isRuntime || _script->RunsInEditor() ) {
			// check if first update used
			if ( _script->IsFirstUpdate() ) {
				_script->CallFunction( "OnStart" );
				_script->SetFirstUpdate( FALSE );
			}
			else {
				_script->CallFunction( "OnUpdate" );
			}
		}
	}
}
