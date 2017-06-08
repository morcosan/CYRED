// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"

namespace CYRED
{
	class RigidBody;
}


namespace CYRED
{
	class AttrViewer_RigidBody : public AttrViewer
	{
	public:
		cchar* const TITLE				= "Rigid Body";

		cchar* const ATTR_IS_TRIGGER	= "Is Trigger";
		cchar* const ATTR_SHAPE_TYPE	= "Shape Type";
		cchar* const SHAPE_TYPE_BOX		= "Box";
		cchar* const SHAPE_TYPE_SPHERE	= "Sphere";
		cchar* const ATTR_SHAPE_SIZE	= "Shape Size";
		cchar* const ATTR_SHAPE_RADIUS	= "Shape Radius";
		cchar* const ATTR_MASS			= "Mass";


	public:
		AttrViewer_RigidBody() {}
		virtual ~AttrViewer_RigidBody() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI			()					override;
		void _UpdateTarget		()					override;


	private:
		RigidBody*	_target;


	private:
		void _ChangeVisibility	();
	};
}