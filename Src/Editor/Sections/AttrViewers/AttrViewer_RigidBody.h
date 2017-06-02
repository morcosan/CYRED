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
		cchar* TITLE				= "Rigid Body";

		cchar* ATTR_SHAPE_TYPE		= "Shape Type";
		cchar* ATTR_SHAPE_SIZE		= "Shape Size";
		cchar* ATTR_SHAPE_RADIUS	= "Shape Radius";
		cchar* ATTR_MASS			= "Mass";


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