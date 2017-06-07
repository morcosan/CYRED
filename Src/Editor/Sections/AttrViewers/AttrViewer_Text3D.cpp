// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Text3D.h"

#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Event.h"

#include "../Selectors/Selector_Font.h"
#include "../Selectors/Selector_Shader.h"


using namespace CYRED;


void AttrViewer_Text3D::_OnInitialize()
{
	_CreateAttrString	( ATTR_TEXT,		ATTR_TEXT,		AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );
	_CreateAttrVector4	( ATTR_TEXT_COLOR,	ATTR_TEXT_COLOR );

	DataArray<cchar*> hAlign;
	hAlign.Add( H_ALIN_LEFT );
	hAlign.Add( H_ALIN_MIDDLE );
	hAlign.Add( H_ALIN_RIGHT );
	_CreateAttrDropdown	( ATTR_H_ALIGN, ATTR_H_ALIGN, hAlign );

	DataArray<cchar*> vAlign;
	vAlign.Add( V_ALIN_TOP );
	vAlign.Add( V_ALIN_MIDDLE );
	vAlign.Add( V_ALIN_BOTTOM );
	_CreateAttrDropdown	( ATTR_V_ALIGN, ATTR_V_ALIGN, vAlign );

	_CreateAttrSelector	( ATTR_FONT,		ATTR_FONT,		Selector_Font::TYPE );
	_CreateAttrInt		( ATTR_FONT_SIZE,	ATTR_FONT_SIZE );
	_CreateAttrSelector	( ATTR_SHADER,		ATTR_SHADER,	Selector_Shader::TYPE );

	_CreateInnerAttribute( InnerAttrType::ENABLED );
	_CreateInnerAttribute( InnerAttrType::SETTINGS );

	_AddToPanel( TITLE );
}


void AttrViewer_Text3D::_OnChangeTarget( void* target )
{
	_target = CAST_S( Text3D*, target );

	// prepare settings
	DataUnion attr;
	_WriteInnerAttribute( InnerAttrType::SETTINGS, attr.SetReference( _target ) );
}


void AttrViewer_Text3D::_UpdateGUI()
{
	Mesh* mesh = _target->GetMesh();
	cchar* meshName = (mesh == NULL) ? Selector_Mesh::OPTION_NULL : mesh->GetName();
	_WriteAttrSelector( ATTR_MESH, mesh, meshName );

	Material* material = _target->GetMaterial();
	cchar* matName = (material == NULL) ? Selector_Material::OPTION_NULL : material->GetName();
	_WriteAttrSelector( ATTR_MATERIAL, material, matName );

	if ( _target->IsEnabled() != _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() ) {
		DataUnion attr;
		_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );

		_Colorize( _target->IsEnabled() );
	}
}


void AttrViewer_Text3D::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );
	{
		_target->SetMesh	( CAST_S( Mesh*,	 _ReadAttrSelector( ATTR_MESH ) ) );
		_target->SetMaterial( CAST_S( Material*, _ReadAttrSelector( ATTR_MATERIAL ) ) );
		_target->SetEnabled	( _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() );
	}
	_target->SetEmitEvents( TRUE );

	// emit event manually
	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, _target );

	// update ui
	_Colorize( _target->IsEnabled() );
}
