// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Scripter.h"

#include "../Assets/Script.h"
#include "../../../3_Modules/Event/EventManager.h"
#include "../../../3_Modules/Asset/AssetManager.h"
#include "../../../3_Modules/Physics/PhysicsManagerImpl.h"
#include "../../../3_Modules/File/FileManager.h"


using namespace CYRED;


Scripter::Scripter( GameObject* gameObject )
	: Component( gameObject )
{
	_componentType = ComponentType::SCRIPTER;

	// register event
	EventManager::Singleton()->Register( this, EventType::ASSET_UPDATE,		 EventListenMode::ASYNC_ALL );
	EventManager::Singleton()->Register( this, EventType::GAMEOBJECT_DELETE, EventListenMode::ASYNC_ALL );
}


Scripter::~Scripter()
{
	// unregister
	EventManager::Singleton()->Unregister( this, EventType::ASSET_UPDATE,		EventListenMode::ASYNC_ALL);
	EventManager::Singleton()->Unregister( this, EventType::GAMEOBJECT_DELETE,	EventListenMode::ASYNC_ALL );
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
				Script* asset = CAST_S( Script*, eventData );
				FileManager::Singleton()->Deserialize<Script>( 
					FileManager::Singleton()->Serialize<Script>( asset ).GetChar(), 
					_scripts[i]
				);
				_scripts[i]->LoadLuaFiles( FALSE );
				break;
			}
		}
	}
	// check if collision object was destroyed
	else if ( eventType == EventType::GAMEOBJECT_DELETE ) {
		// search inside collisions
		Iterator<GameObject*, _Collision>& iter = _collisions.GetIterator();
		while ( iter.HasNext() ) {
			if ( eventData == iter.GetKey() ) {
				// remove from list
				_collisions.Erase( iter.GetKey() );
				// found, stop
				break;
			}

			// next
			iter.Next();
		}
	}
}


void Scripter::Clone( Component* clone ) const
{
}


void Scripter::OnCollision( GameObject* other, bool isTrigger )
{
	if ( _isEnabled ) {
		// add to collisions
		if ( _collisions.Has( other ) ) {
			_Collision& coll = _collisions.Get( other );
			coll.type = _OnCollisionType::ACTIVE;
			coll.isTrigger = isTrigger;
		}
		else {
			_collisions.Set( other, _Collision {
				_OnCollisionType::ENTER,
				isTrigger
			} );
		}
	}
}


void Scripter::OnMouseDown( int button )
{
	// call script API
	if ( _isEnabled ) {
		for ( int i = 0; i < _scripts.Size(); i++ ) {
			if ( _scripts[i] != NULL ) {
				_scripts[i]->CallFunction( Script::FUNC_ON_MOUSE_DOWN, _gameObject, button );
			}
		}
	}
}


void Scripter::OnMouseUp( int button )
{
	// call script API
	if ( _isEnabled ) {
		for ( int i = 0; i < _scripts.Size(); i++ ) {
			if ( _scripts[i] != NULL ) {
				_scripts[i]->CallFunction( Script::FUNC_ON_MOUSE_UP, _gameObject, button );
			}
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
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


void Scripter::ClearScripts()
{
	// empty list
	_scripts.Clear();
	_scriptsAsset.Clear();

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


void Scripter::_OnStart( bool isRuntime )
{
	if ( _isEnabled ) {
		for ( int i = 0; i < _scripts.Size(); i++ ) {
			if ( _scripts[i] != NULL && (isRuntime || _scripts[i]->RunsInEditor()) ) {
				// call script function
				_scripts[i]->CallFunction( Script::FUNC_ON_START, _gameObject );
				_scripts[i]->SetFirstUpdate( FALSE );
			}
		}
	}
}


void Scripter::_OnUpdate( bool isRuntime )
{
	// used to remove items from map
	DataArray<GameObject*> toRemove;

	// update collisions
	Iterator<GameObject*, _Collision>& iter = _collisions.GetIterator();
	while ( iter.HasNext() ) {
		_Collision& coll = iter.GetValue();

		switch ( coll.type ) {
			case _OnCollisionType::ENTER:
				// call on enter
				for ( int i = 0; i < _scripts.Size(); i++ ) {
					if ( _scripts[i] != NULL && isRuntime ) {
						_scripts[i]->CallFunction( 
							Script::FUNC_ON_COLLISION_ENTER, 
							_gameObject, 
							iter.GetKey(),
							coll.isTrigger
						);
					}
				}
				break;

			case _OnCollisionType::ACTIVE:
				// it is expected to exit
				// it will be overriden by OnCollision()
				coll.type = _OnCollisionType::EXIT;
				break;

			case _OnCollisionType::EXIT:
				// call on exit
				for ( int i = 0; i < _scripts.Size(); i++ ) {
					if ( _scripts[i] != NULL && isRuntime ) {
						_scripts[i]->CallFunction( 
							Script::FUNC_ON_COLLISION_EXIT, 
							_gameObject, 
							iter.GetKey(),
							coll.isTrigger
						);
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
	if ( _isEnabled ) {
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
}
