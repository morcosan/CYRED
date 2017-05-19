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
		const char* TITLE			= "MeshRendering";

		const char* ATTR_MESH		= "Mesh";
		const char* ATTR_MATERIAL	= "Material";


	public:
		AttrViewer_MeshRendering() {}
		virtual ~AttrViewer_MeshRendering() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		MeshRendering*	_target;
	};
}