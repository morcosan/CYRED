// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "MeshRendering.h"

#include "../Assets/Material.h"
#include "../Assets/Mesh.h"
#include "../../Event/EventManager.h"


using namespace CYRED;


MeshRendering::MeshRendering( GameObject* gameObject )
	: Component( gameObject )
	, _material( NULL )
	, _mesh( NULL )
{
	_componentType = ComponentType::MESH_RENDERING;
}


void MeshRendering::Clone( Component* clone ) const
{
	// cast to transform
	MeshRendering* cloneComp = CAST_S( MeshRendering*, clone );
	// set attributes
	cloneComp->SetMesh( _mesh );
	cloneComp->SetMaterial( _material );
	cloneComp->SetEnabled( _enabled );
}


Material* MeshRendering::GetMaterial() const
{
	return _material;
}


Mesh* MeshRendering::GetMesh() const
{
	return _mesh;
}


void MeshRendering::SetMaterial( Material* value )
{
	_material = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}


void MeshRendering::SetMesh( Mesh* value )
{
	_mesh = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, this );
	}
}
