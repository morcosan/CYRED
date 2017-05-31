// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Shader.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"


using namespace CYRED;



void AttrViewer_Shader::_OnInitialize()
{
	_CreateAttrString	( ATTR_NAME, ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	_CreateAttrString	( ATTR_VERTEX,		ATTR_VERTEX );
	_CreateAttrString	( ATTR_GEOMETRY,	ATTR_GEOMETRY );
	_CreateAttrString	( ATTR_FRAGMENT,	ATTR_FRAGMENT );
	
	_AddToPanel( TITLE );
}


void AttrViewer_Shader::_OnChangeTarget( void* target )
{
	_target = CAST_S( Shader*, target );

	// reset colorize
	_Colorize( TRUE, TRUE );
}


void AttrViewer_Shader::_UpdateGUI()
{
	_WriteAttrString( ATTR_NAME, _target->GetName() );

	cchar* vertexPath = NULL;
	cchar* geometryPath = NULL;
	cchar* fragmentPath = NULL;
	_target->GetShaderFiles( &vertexPath, &geometryPath, &fragmentPath );
	_WriteAttrString( ATTR_VERTEX, vertexPath );
	_WriteAttrString( ATTR_GEOMETRY, geometryPath );
	_WriteAttrString( ATTR_FRAGMENT, fragmentPath );
}


void AttrViewer_Shader::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );

	_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );

	String&	vertexPath = _ReadAttrString( ATTR_VERTEX );
	String&	geometryPath = _ReadAttrString( ATTR_GEOMETRY );
	String&	fragmentPath = _ReadAttrString( ATTR_FRAGMENT );

	_target->SetShaderFiles( vertexPath.GetChar(), geometryPath.GetChar(), fragmentPath.GetChar() );

	_target->SetEmitEvents( TRUE );

	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, _target );
}

