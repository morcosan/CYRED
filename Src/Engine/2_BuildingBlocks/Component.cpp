// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "Component.h"


using namespace CYRED;
using namespace COMP;


Component::Component( GameObject* gameObject )
	: _gameObject( gameObject )
	, _enabled( TRUE )
	, _emitEvents( TRUE )
	, _componentType( ComponentType::COMPONENT )
	, _componentSubtype( NULL )
{
}


ComponentType Component::GetComponentType() const
{
	return _componentType;
}


const Char* Component::GetComponentSubtype() const
{
	return _componentSubtype;
}


GameObject* Component::GetGameObject() const
{
	return _gameObject;
}


void Component::SetEnabled( Bool value )
{
	_enabled = value;
	_OnEnable();
}


void Component::SetEmitEvents( Bool value )
{
	_emitEvents = value;
}


Bool Component::IsEnabled() const
{
	return _enabled;
}


Bool Component::DoesEmitEvents() const
{
	return _emitEvents;
}

