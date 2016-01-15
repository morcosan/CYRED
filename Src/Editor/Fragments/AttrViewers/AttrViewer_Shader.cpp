// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Shader.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"


using namespace CYRED;



void AttrViewer_Shader::_OnInitialize()
{
	_CreateAttrString	( ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	DataArray<const Char*> renderers;
	renderers.Add( RENDERER_FORWARD );
	_CreateAttrDropdown	( ATTR_RENDERER, renderers );

	_CreateAttrString	( ATTR_VERTEX );
	_CreateAttrString	( ATTR_GEOMETRY );
	_CreateAttrString	( ATTR_FRAGMENT );
	
	_AddToPanel( TITLE );
}


void AttrViewer_Shader::_OnChangeTarget( void* target )
{
	_target = CAST_S( Shader*, target );
}


void AttrViewer_Shader::_OnUpdateGUI()
{
	_WriteAttrString( ATTR_NAME, _target->GetName() );

	Int rendererIndex = 0;
	_WriteAttrDropdown( ATTR_RENDERER, rendererIndex );

	const Char* vertexPath = NULL;
	const Char* geometryPath = NULL;
	const Char* fragmentPath = NULL;
	_target->GetShaderFiles( RENDERER_FORWARD, &vertexPath, &geometryPath, &fragmentPath );
	_WriteAttrString( ATTR_VERTEX, vertexPath );
	_WriteAttrString( ATTR_GEOMETRY, geometryPath );
	_WriteAttrString( ATTR_FRAGMENT, fragmentPath );
}


void AttrViewer_Shader::_OnUpdateTarget()
{
	_target->SetEmitEvents( FALSE );

	_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );

	const Char* renderer = NULL;
	Int rendererIndex = _ReadAttrDropdown( ATTR_RENDERER );
	switch ( rendererIndex )
	{
		case 0:
			renderer = RENDERER_FORWARD;
			break;
	}

	const Char*	vertexPath = _ReadAttrString( ATTR_VERTEX ).GetChar();
	const Char*	geometryPath = _ReadAttrString( ATTR_GEOMETRY ).GetChar();
	const Char*	fragmentPath = _ReadAttrString( ATTR_FRAGMENT ).GetChar();

	_target->SetShaderFiles( renderer, vertexPath, geometryPath, fragmentPath );

	_target->SetEmitEvents( TRUE );

	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, _target );
}

