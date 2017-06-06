// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Scripter.h"

#include "../Assets/Script.h"
#include "../../../3_Modules/Event/EventManager.h"
#include "../../../3_Modules/Asset/AssetManager.h"
#include "../../../3_Modules/Physics/PhysicsManagerImpl.h"


using namespace CYRED;


Scripter::Scripter( GameObject* gameObject )
	: Component( gameObject )
{
	_componentType = ComponentType::SCRIPTER;

	// register event
	EventManager::Singleton()->RegisterListener( this, EventType::ASSET_UPDATE );
}


Scripter::~Scripter()
{
	// unregister
	EventManager::Singleton()->UnregisterListener( this, EventType::ASSET_UPDATE );
}


void Scripter::OnAdded()
{
	// update physics
	NonAPI::PhysicsManagerImpl::Singleton()->RegisterScripter( this );
	// clear collisions
	_collisions.Clear();
}


void Scripter::OnRemoved()
{
	// update physics
	NonAPI::PhysicsManagerImpl::Singleton()->UnregisterScripter( this );
}


void Scripter::OnEvent( int eventType, void* eventData )
{
	// check if any script asset was reloaded
	if ( eventType == EventType::ASSET_UPDATE ) {
		// find script asset
		for ( int i = 0; i < _scriptsAsset.Size(); i++ ) {
			if ( eventData == _scriptsAsset[i] ) {
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


void Scripter::OnCollision( GameObject* other )
{
	if ( _isEnabled ) {
		// add to collisions
		if ( _collisions.Has( other ) ) {
			_collisions.Set( other, OnCollisionType::ACTIVE );
		}
		else {
			_collisions.Set( other, OnCollisionType::ENTER );
		}
	}
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
		EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, this );
	}
}


void Scripter::ClearScripts()
{
	// empty list
	_scripts.Clear();
	_scriptsAsset.Clear();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, this );
	}
}


void Scripter::_OnStart( bool isRuntime )
{
	if ( !_isEnabled ) {
		return;
	}

	for ( int i = 0; i < _scripts.Size(); i++ ) {
		if ( _scripts[i] != NULL && (isRuntime || _scripts[i]->RunsInEditor()) ) {
			// call script function
			_scripts[i]->CallFunction( Script::FUNC_ON_START, _gameObject );
			_scripts[i]->SetFirstUpdate( FALSE );
		}
	}
}


void Scripter::_OnUpdate( bool isRuntime )
{
	// used to remove items from map
	DataArray<GameObject*> toRemove;

	// update collisions
	Iterator<GameObject*, OnCollisionType>& iter = _collisions.GetIterator();
	while ( iter.HasNext() ) {
		OnCollisionType collType = iter.GetValue();

		switch ( collType ) {
			case OnCollisionType::ENTER:
				// call on enter
				for ( int i = 0; i < _scripts.Size(); i++ ) {
					if ( _scripts[i] != NULL && isRuntime ) {
						_scripts[i]->CallFunction( Script::FUNC_ON_COLLISION_ENTER, _gameObject, iter.GetKey() );
					}
				}
				break;

			case OnCollisionType::ACTIVE:
				// it is expected to exit
				// it will be overriden by OnCollision()
				_collisions.Set( iter.GetKey(), OnCollisionType::EXIT );
				break;

			case OnCollisionType::EXIT:
				// call on exit
				for ( int i = 0; i < _scripts.Size(); i++ ) {
					if ( _scripts[i] != NULL && isRuntime ) {
						_scripts[i]->CallFunction( Script::FUNC_ON_COLLISION_EXIT, _gameObject, iter.GetKey() );
					}
				}

				// remove from list
				toRemove.Add( iter.GetKey() );
				break;
		}

		// next
		iter.Next();
	}

	// remove marked items from collisions
	for ( int i = 0; i < toRemove.Size(); i++ ) {
		_collisions.Erase( toRemove[i] );
	}


	// update scripts
	for ( int i = 0; i < _scripts.Size(); i++ ) {
		if ( _scripts[i] != NULL && (isRuntime || _scripts[i]->RunsInEditor()) ) {
			// check if first update used
			if ( _scripts[i]->IsFirstUpdate() ) {
				_scripts[i]->CallFunction( Script::FUNC_ON_START, _gameObject );
				_scripts[i]->SetFirstUpdate( FALSE );
			}
			else {
				_scripts[i]->CallFunction( Script::FUNC_ON_UPDATE, _gameObject );
			}
		}
	}
}
