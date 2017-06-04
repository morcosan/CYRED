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
	, _isEnabled ( TRUE )
	, _emitEvents( TRUE )
{
}


GameObject::GameObject( int uid )
	: _uid( uid )
	, _isEnabled ( TRUE )
	, _emitEvents( TRUE )
{
}


GameObject::GameObject( cchar* name, int uid )
	: Node( name )
	, _uid( uid )
	, _isEnabled ( TRUE )
	, _emitEvents( TRUE )
{
}


GameObject::~GameObject()
{
	for ( int i = 0; i < _components.Size(); ++i ) {
		PTR_FREE( _components[i] );
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
	return _isEnabled;
}


int GameObject::GetUniqueID() const
{
	return _uid;
}


bool GameObject::DoesEmitEvents() const
{
	return _emitEvents;
}


cchar* GameObject::GetTag() const
{
	return _tag.GetChar();
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
	_isEnabled = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::GAMEOBJECT_UPDATE, this );
	}
}


void GameObject::SetEmitEvents( bool value )
{
	_emitEvents = value;
}


void GameObject::SetTag( cchar* value )
{
	_tag = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::GAMEOBJECT_UPDATE, this );
	}
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


void GameObject::_EmitAddEvent( Component* component )
{
	component->OnAdded();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_ADD, component );
	}
}


void GameObject::_EmitRemoveEvent( Component* component )
{
	component->OnRemoved();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_REMOVE, component );
	}
}
