// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Scripter.h"

#include "../Assets/Script.h"
#include "../../../3_Modules/Event/EventManager.h"
#include "../../../3_Modules/Asset/AssetManager.h"


using namespace CYRED;


Scripter::Scripter( GameObject* gameObject )
	: Component( gameObject )
{
	_componentType = ComponentType::SCRIPTER;

	// register event
	EventManager::Singleton()->RegisterListener( EventType::CHANGE_ASSET, this );
}


Scripter::~Scripter()
{
	// unregister
	EventManager::Singleton()->UnregisterListener( EventType::CHANGE_ASSET, this );
}


void Scripter::OnEvent( EventType eType, void* eData )
{
	// check if any script asset was reloaded
	if ( eType == EventType::CHANGE_ASSET ) {
		// find script asset
		for ( int i = 0; i < _scriptsAsset.Size(); i++ ) {
			if ( eData == _scriptsAsset[i] ) {
				// reload this script
				_scripts[i]->LoadFullFile();
				_scripts[i]->LoadLuaFiles( FALSE );
				break;
			}
		}
	}
}


void Scripter::Clone( Component* clone ) const
{
}


Script* Scripter::GetScript( int index ) const
{
	ASSERT( index < _scripts.Size() );
	return _scripts[index];
}


int Scripter::GetScriptsCount() const
{
	return _scripts.Size();
}


void Scripter::SetScript( int index, cchar* scriptUID )
{
	// fill array
	while ( index >= _scripts.Size() ) {
		_scripts.Add( NULL );
		_scriptsAsset.Add( NULL );
	}

	// set script
	Asset* asset = AssetManager::Singleton()->GetScript( scriptUID );
	if ( asset != NULL ) {
		_scripts[index] = CAST_S( Script*, asset->Clone() );
		// load script data
		_scripts[index]->LoadFullFile();
		_scripts[index]->LoadLuaFiles();
		// store asset
		_scriptsAsset[index] = asset;
	}

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_SCRIPTER, this );
	}
}


void Scripter::ClearScripts()
{
	// empty list
	_scripts.Clear();
	_scriptsAsset.Clear();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_SCRIPTER, this );
	}
}


void Scripter::_OnStart( bool isRuntime )
{
	for ( int i = 0; i < _scripts.Size(); i++ ) {
		if ( _scripts[i] != NULL ) {
			// call script function
			if ( isRuntime || _scripts[i]->RunsInEditor() ) {
				_scripts[i]->CallFunction( "OnStart", _gameObject );
				_scripts[i]->SetFirstUpdate( FALSE );
			}
		}
	}
}


void Scripter::_OnUpdate( bool isRuntime )
{
	for ( int i = 0; i < _scripts.Size(); i++ ) {
		if ( _scripts[i] != NULL ) {
			// call script function
			if ( isRuntime || _scripts[i]->RunsInEditor() ) {
				// check if first update used
				if ( _scripts[i]->IsFirstUpdate() ) {
					_scripts[i]->CallFunction( "OnStart", _gameObject );
					_scripts[i]->SetFirstUpdate( FALSE );
				}
				else {
					_scripts[i]->CallFunction( "OnUpdate", _gameObject );
				}
			}
		}
	}
}
