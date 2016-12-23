// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Scripter.h"
#include "../Assets/Script.h"


using namespace CYRED;
using namespace COMP;


Scripter::Scripter( GameObject * gameObject )
	: Component( gameObject )
	, _needStart( TRUE )
	, _script( NULL )
{
}


void Scripter::OnStart()
{
	_needStart = FALSE;

	// call custom function
	_OnStart();

	// call script function
	if ( _script != NULL ) {
		_script->CallFunction( "OnStart" );
	}
}


void Scripter::OnUpdate()
{
	// call custom function
	_OnUpdate();

	// call script function
	if ( _script != NULL ) {
		_script->CallFunction( "OnUpdate" );
	}
}


Bool Scripter::NeedStart() const
{
	return _needStart;
}


Script* Scripter::GetScript() const
{
	return _script;
}


void Scripter::SetScript( Script* script )
{
	_script = script;
}
