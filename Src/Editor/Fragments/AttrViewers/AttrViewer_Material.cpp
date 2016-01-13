// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Material.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"
#include "../Selectors/Selector_Shader.h"


using namespace CYRED;



void AttrViewer_Material::OnSelect_Target( void* target )
{
	_target = CAST_S( Material*, target );
	
	_CreateAttrString	( ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );
	_CreateAttrSelector	( ATTR_SHADER, Selector_Shader::TYPE );
	_CreateAttrBool		( ATTR_WIREFRAME );
	_CreateAttrFloat	( ATTR_LINE_WIDTH );
	
	DataArray<const Char*> cullFaceTypes;
	cullFaceTypes.Add( "CULL_BACK" );
	cullFaceTypes.Add( "CULL_FRONT" );
	cullFaceTypes.Add( "CULL_NONE" );
	_CreateAttrDropdown	( ATTR_CULL_FACE, cullFaceTypes );

	_CreateAttrList		( ATTR_PROPERTIES, AttrType::BOOL );
	
	_AddToPanel( TITLE );
}


void AttrViewer_Material::_OnUpdate_GUI()
{
	DataUnion attr;

	_WriteAttribute( ATTR_NAME,			attr.SetString( _target->GetName() ) );

	Shader* shader = _target->GetShader();
	const Char* shaderName = (shader == NULL) ? Selector_Shader::OPTION_NULL : shader->GetName();
	_WriteAttribute( ATTR_SHADER,		attr.SetReference( shader ), shaderName );

	_WriteAttribute( ATTR_WIREFRAME,	attr.SetBool( _target->IsWireframe() ) );
	_WriteAttribute( ATTR_LINE_WIDTH,	attr.SetFloat( _target->GetLineWidth() ) );

	Int cullFaceIndex = 0;
	switch ( _target->GetFaceCulling() )
	{
		case FaceCulling::CULL_BACK:
			cullFaceIndex = 0;
			break;

		case FaceCulling::CULL_FRONT:
			cullFaceIndex = 1;
			break;

		case FaceCulling::CULL_NONE:
			cullFaceIndex = 2;
			break;
	}
	_WriteAttribute( ATTR_CULL_FACE,	attr.SetInt( cullFaceIndex ) );
}


void AttrViewer_Material::_OnUpdate_Target()
{
	_target->SetEmitEvents( FALSE );

	_target->SetName( _ReadAttribute( ATTR_NAME ).GetString() );

	_target->SetShader( CAST_S( Shader*, _ReadAttribute( ATTR_SHADER ).GetReference() ) );
	_target->SetWireframe( _ReadAttribute( ATTR_WIREFRAME ).GetBool() );
	_target->SetLineWidth( _ReadAttribute( ATTR_LINE_WIDTH ).GetFloat() );

	FaceCulling	faceCulling;
	Int cullFaceIndex		= _ReadAttribute( ATTR_CULL_FACE ).GetInt();
	switch ( cullFaceIndex )
	{
		case 0:
			faceCulling = FaceCulling::CULL_BACK;
			break;

		case 1:
			faceCulling = FaceCulling::CULL_FRONT;
			break;

		case 2:
			faceCulling = FaceCulling::CULL_NONE;
			break;
	}
	_target->SetFaceCulling( faceCulling );

	_target->SetEmitEvents( TRUE );

	EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, _target );
}

