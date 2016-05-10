// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "GameObject.h"
#include "Component.h"
#include "../3_Modules/Event/EventManager.h"


using namespace CYRED;



GameObject::GameObject()
	: _uid( EMPTY_OBJECT_UID )
	, _enabled ( TRUE )
	, _emitEvents( TRUE )
{
}


GameObject::GameObject( UInt uid )
	: _uid( uid )
	, _enabled ( TRUE )
	, _emitEvents( TRUE )
{
}

GameObject::GameObject( const Char* name, UInt uid )
	: _name( name )
	, _uid( uid )
	, _enabled ( TRUE )
	, _emitEvents( TRUE )
{
}


GameObject::~GameObject()
{
	for ( UInt i = 0; i < _components.Size(); ++i )
	{
		Memory::Free( _components[i] );
	}

	_components.Clear();
}


void GameObject::OnHierarchyChange()
{
	for ( UInt i = 0; i < _components.Size(); ++i )
	{
		_components[i]->OnHierarchyChange();
	}
}


void GameObject::OnUpdate()
{
	for ( UInt i = 0; i < _components.Size(); ++i )
	{
		_components[i]->OnUpdate();
	}

	for ( UInt i = 0; i < _childNodes.Size(); ++i )
	{
		_childNodes[i]->OnUpdate();
	}
}


Bool GameObject::IsEnabled() const
{
	return _enabled;
}


UInt GameObject::GetUID() const
{
	return _uid;
}


Bool GameObject::DoesEmitEvents() const
{
	return _emitEvents;
}


UInt GameObject::GetComponentCount() const
{
	return _components.Size();
}


COMP::Component* GameObject::GetComponentAt( UInt index ) const
{
	ASSERT( index < _components.Size() );
	return _components[index];
}


void GameObject::SetEnabled( Bool value )
{
	_enabled = value;
}


const Char* GameObject::GetName() const
{
	return _name.GetChar();
}


void GameObject::SetName( const Char* name )
{
	_name = name;

	//! announce the whole application
	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::SCENE, 
											  EventName::GAMEOBJECT_RENAMED, 
											  this );
	}
}


void GameObject::SetEmitEvents( Bool value )
{
	_emitEvents = value;
}

