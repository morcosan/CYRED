// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Component.h"


using namespace CYRED;


Component::Component( GameObject* gameObject )
	: _gameObject( gameObject )
	, _isEnabled( TRUE )
	, _emitEvents( TRUE )
	, _componentType( ComponentType::COMPONENT )
	, _isFirstUpdate( TRUE )
{
}


void Component::OnUpdate( bool isRuntime )
{
	// first update is considered OnStart
	if ( _isFirstUpdate ) {
		// mark flag
		_isFirstUpdate = FALSE;
		
		// call OnStart
		_OnStart( isRuntime );
	}
	else {
		// call OnUpdate
		_OnUpdate( isRuntime );
	}
}


ComponentType Component::GetComponentType() const
{
	return _componentType;
}


GameObject* Component::GetGameObject() const
{
	return _gameObject;
}


void Component::SetEnabled( bool value )
{
	_isEnabled = value;
	_OnEnable();
}


void Component::SetEmitEvents( bool value )
{
	_emitEvents = value;
}


bool Component::IsEnabled() const
{
	return _isEnabled;
}


bool Component::DoesEmitEvents() const
{
	return _emitEvents;
}

