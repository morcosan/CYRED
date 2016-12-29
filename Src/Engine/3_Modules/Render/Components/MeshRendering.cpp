// Copyright (c) 2015 Morco (www.morco.ro)
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

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_MESH_RENDERING, this );
	}
}


void MeshRendering::SetMesh( Mesh* value )
{
	_mesh = value;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_MESH_RENDERING, this );
	}
}
