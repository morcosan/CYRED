// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Scripter.h"

#include "../Assets/Script.h"
#include "../../../3_Modules/Event/EventManager.h"
#include "../../../3_Modules/Asset/AssetManager.h"


using namespace CYRED;
using namespace COMP;


Scripter::Scripter( GameObject* gameObject )
	: Component( gameObject )
{
	_componentType = ComponentType::SCRIPTER;
}


Script* Scripter::GetScript( UInt index ) const
{
	ASSERT( index < _scripts.Size() );
	return _scripts[index];
}


UInt Scripter::GetScriptsCount() const
{
	return _scripts.Size();
}


void Scripter::SetScript( UInt index, const Char* scriptUID )
{
	// fill array
	while ( index >= _scripts.Size() ) {
		_scripts.Add( NULL );
	}

	// set script
	Asset* asset = AssetManager::Singleton()->GetScript( scriptUID );
	if ( asset != NULL ) {
		_scripts[index] = CAST_S( Script*, asset->Clone() );
		// load script data
		_scripts[index]->LoadFullFile();
	}

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT, EventName::SCRIPTER_CHANGED, this );
	}
}


void Scripter::ClearScripts()
{
	// empty list
	_scripts.Clear();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT, EventName::SCRIPTER_CHANGED, this );
	}
}


void Scripter::_OnStart( Bool isRuntime )
{
	for ( UInt i = 0; i < _scripts.Size(); i++ ) {
		if ( _scripts[i] != NULL ) {
			// call script function
			if ( isRuntime || _scripts[i]->RunsInEditor() ) {
				_scripts[i]->CallFunction( "OnStart" );
				_scripts[i]->SetFirstUpdate( FALSE );
			}
		}
	}
}


void Scripter::_OnUpdate( Bool isRuntime )
{
	for ( UInt i = 0; i < _scripts.Size(); i++ ) {
		if ( _scripts[i] != NULL ) {
			// call script function
			if ( isRuntime || _scripts[i]->RunsInEditor() ) {
				// check if first update used
				if ( _scripts[i]->IsFirstUpdate() ) {
					_scripts[i]->CallFunction( "OnStart" );
					_scripts[i]->SetFirstUpdate( FALSE );
				}
				else {
					_scripts[i]->CallFunction( "OnUpdate" );
				}
			}
		}
	}
}
