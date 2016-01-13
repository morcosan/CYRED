// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "MeshRendering.h"

#include "../Assets/Material.h"
#include "../Assets/Mesh.h"


using namespace CYRED;
using namespace COMP;



MeshRendering::MeshRendering( GameObject* gameObject )
	: Component( gameObject )
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
}


void MeshRendering::SetMesh( Mesh* value )
{
	_mesh = value;
}
