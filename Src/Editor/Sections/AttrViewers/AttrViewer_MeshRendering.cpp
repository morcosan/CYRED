// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_MeshRendering.h"

#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Event.h"

#include "../Selectors/Selector_Material.h"
#include "../Selectors/Selector_Mesh.h"


using namespace CYRED;


void AttrViewer_MeshRendering::_OnInitialize()
{
	_CreateAttrSelector( ATTR_MESH,		ATTR_MESH,		Selector_Mesh::TYPE );
	_CreateAttrSelector( ATTR_MATERIAL, ATTR_MATERIAL,	Selector_Material::TYPE );
	
	_CreateInnerAttribute( InnerAttrType::ENABLED );
	_CreateInnerAttribute( InnerAttrType::SETTINGS );

	_AddToPanel( TITLE );
}


void AttrViewer_MeshRendering::_OnChangeTarget( void* target )
{
	_target = CAST_S( MeshRendering*, target );

	// prepare settings
	DataUnion attr;
	_WriteInnerAttribute( InnerAttrType::SETTINGS, attr.SetReference( _target ) );

	// change color for enable
	_Colorize( _target->IsEnabled() );
}


void AttrViewer_MeshRendering::_UpdateGUI()
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
		// change color for enable
		_Colorize( _target->IsEnabled() );
	}
}


void AttrViewer_MeshRendering::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );
	{
		_target->SetMesh	( CAST_S( Mesh*,	 _ReadAttrSelector( ATTR_MESH ) ) );
		_target->SetMaterial( CAST_S( Material*, _ReadAttrSelector( ATTR_MATERIAL ) ) );
		
		bool newValue = _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool();
		if ( _target->IsEnabled() != newValue ) {
			_target->SetEnabled( newValue );
			// change color for enable
			_Colorize( _target->IsEnabled() );
		}
	}
	_target->SetEmitEvents( TRUE );

	// emit event manually
	++_ignoreUpdateGUI;
	EventManager::Singleton()->PushEvent( EventType::COMPONENT_UPDATE, _target );

	// update ui
	_Colorize( _target->IsEnabled() );
}
