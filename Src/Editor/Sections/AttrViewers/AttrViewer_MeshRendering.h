// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"
#include "CyredModule_Render.h"


namespace CYRED
{
	namespace COMP
	{
		class MeshRendering;
	}
}


namespace CYRED
{
	class AttrViewer_MeshRendering : public AttrViewer
	{
	public:
		cchar* TITLE			= "MeshRendering";

		cchar* ATTR_MESH		= "Mesh";
		cchar* ATTR_MATERIAL	= "Material";


	public:
		AttrViewer_MeshRendering() {}
		virtual ~AttrViewer_MeshRendering() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		MeshRendering*	_target;
	};
}