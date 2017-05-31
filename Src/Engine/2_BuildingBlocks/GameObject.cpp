// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "GameObject.h"
#include "Component.h"
#include "../3_Modules/Event/EventManager.h"
#include "Components\Transform.h"
#include "../3_Modules/File/FileManager.h"


using namespace CYRED;



GameObject::GameObject()
	: _uid( EMPTY_OBJECT_UID )
	, _enabled ( TRUE )
	, _emitEvents( TRUE )
{
}


GameObject::GameObject( int uid )
	: _uid( uid )
	, _enabled ( TRUE )
	, _emitEvents( TRUE )
{
}

GameObject::GameObject( cchar* name, int uid )
	: Node( name )
	, _uid( uid )
	, _enabled ( TRUE )
	, _emitEvents( TRUE )
{
}


GameObject::~GameObject()
{
	for ( int i = 0; i < _components.Size(); ++i ) {
		Memory::Free( _components[i] );
	}

	_components.Clear();
}


void GameObject::OnHierarchyChange()
{
	// anounce components
	for ( int i = 0; i < _components.Size(); ++i ) {
		_components[i]->OnHierarchyChange();
	}

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::GAMEOBJECT_UPDATE, this );
	}
}


void GameObject::OnUpdate( bool isRuntime )
{
	// update components
	for ( int i = 0; i < _components.Size(); ++i ) {
		_components[i]->OnUpdate( isRuntime );
	}

	// update child gameobjects
	for ( int i = 0; i < _childNodes.Size(); ++i ) {
		_childNodes[i]->OnUpdate( isRuntime );
	}
}


bool GameObject::IsEnabled() const
{
	return _enabled;
}


int GameObject::GetUniqueID() const
{
	return _uid;
}


bool GameObject::DoesEmitEvents() const
{
	return _emitEvents;
}


int GameObject::GetComponentCount() const
{
	return _components.Size();
}


Component* GameObject::GetComponentAt( int index ) const
{
	ASSERT( index < _components.Size() );
	return _components[index];
}


void GameObject::SetEnabled( bool value )
{
	_enabled = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::GAMEOBJECT_UPDATE, this );
	}
}


void GameObject::SetEmitEvents( bool value )
{
	_emitEvents = value;
}


void GameObject::Clone( GameObject* clone ) const
{
	// serialize this
	String& fileData = FileManager::Singleton()->Serialize<GameObject>( this );
	// deserialize into clone
	FileManager::Singleton()->Deserialize<GameObject>( fileData.GetChar(), clone );
}


void GameObject::_OnRename()
{
	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::GAMEOBJECT_RENAME, this );
	}
}


void GameObject::_EmitAddEvent()
{
	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_ADD, this );
	}
}


void GameObject::_EmitRemoveEvent()
{
	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_REMOVE, this );
	}
}
