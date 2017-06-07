// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"

#include "../../../2_BuildingBlocks/Component.h"


namespace CYRED
{
	class DLL Text3D : public Component
	{
	public:
		Text3D( GameObject* gameObject = NULL );
		virtual ~Text3D() {}


	public:
		void OnAdded	()							override {}
		void OnRemoved	()							override {}
		void Clone		( Component* clone ) const	override;


	private:
		void _OnEnable() override {}
	};
}