// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"

#include "../../../2_BuildingBlocks/Component.h"


namespace CYRED
{
	class Mesh;
	class Material;
}


namespace CYRED
{
	class DLL MeshRendering : public Component
	{
	public:
		MeshRendering( GameObject* gameObject = NULL );
		virtual ~MeshRendering() {}


	public:
		void Clone( Component* clone ) const override;


	public:
		Material*	GetMaterial	() const;
		Mesh*		GetMesh		() const;

		void		SetMaterial	( Material* value );
		void		SetMesh		( Mesh* value );


	protected:
		void _OnEnable() override {}


	protected:
		Material*	_material;
		Mesh*		_mesh;
	};
}