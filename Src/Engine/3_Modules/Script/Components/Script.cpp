// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Script.h"

using namespace CYRED;
using namespace COMP;


Script::Script( GameObject * gameObject )
	: Component( gameObject )
	, _runInEditor( FALSE )
	, _needStart( TRUE )
{
}


void Script::Start()
{
	_needStart = FALSE;
	_OnStart();
}


void Script::Update()
{
	_OnUpdate();
}


Bool Script::DoesRunInEditor()
{
	return _runInEditor;
}


void Script::SetRunInEditor( Bool value )
{
	_runInEditor = value;
}


Bool Script::DoesNeedStart()
{
	return _needStart;
}
