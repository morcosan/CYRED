// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Shader.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"


using namespace CYRED;



void AttrViewer_Shader::OnSelect_Target( void* target )
{
	_target = CAST_S( Shader*, target );
	
	_CreateAttrString	( ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	DataArray<const Char*> renderers;
	renderers.Add( RENDERER_FORWARD );
	_CreateAttrDropdown	( ATTR_RENDERER, renderers );

	_CreateAttrString	( ATTR_VERTEX );
	_CreateAttrString	( ATTR_GEOMETRY );
	_CreateAttrString	( ATTR_FRAGMENT );
	
	_AddToPanel( TITLE );
}


void AttrViewer_Shader::_OnUpdate_GUI()
{
	DataUnion attr;

	_WriteAttribute( ATTR_NAME,			attr.SetString( _target->GetName() ) );

	Int rendererIndex = 0;
	_WriteAttribute( ATTR_RENDERER,		attr.SetInt( rendererIndex ) );

	const Char* vertexPath = NULL;
	const Char* geometryPath = NULL;
	const Char* fragmentPath = NULL;
	_target->GetShaderFiles( RENDERER_FORWARD, &vertexPath, &geometryPath, &fragmentPath );
	_WriteAttribute( ATTR_VERTEX,		attr.SetString( vertexPath ) );
	_WriteAttribute( ATTR_GEOMETRY,		attr.SetString( geometryPath ) );
	_WriteAttribute( ATTR_FRAGMENT,		attr.SetString( fragmentPath ) );
}


void AttrViewer_Shader::_OnUpdate_Target()
{
	_target->SetEmitEvents( FALSE );

	_target->SetName( _ReadAttribute( ATTR_NAME ).GetString() );

	const Char* renderer = NULL;
	Int rendererIndex = _ReadAttribute( ATTR_RENDERER ).GetInt();
	switch ( rendererIndex )
	{
		case 0:
			renderer = RENDERER_FORWARD;
			break;
	}

	const Char*	vertexPath = _ReadAttribute( ATTR_VERTEX ).GetString();
	const Char*	geometryPath = _ReadAttribute( ATTR_GEOMETRY ).GetString();
	const Char*	fragmentPath = _ReadAttribute( ATTR_FRAGMENT ).GetString();

	_target->SetShaderFiles( renderer, vertexPath, geometryPath, fragmentPath );

	_target->SetEmitEvents( TRUE );

	EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, _target );
}

