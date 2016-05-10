// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"
#include "CyredModule_Render.h"


namespace CYRED
{
	namespace COMP
	{
		class MorphRendering;
	}
}


namespace CYRED
{
	class AttrViewer_MorphRendering : public AttrViewer
	{
	public:
		const Char* TITLE					= "MorphRendering";

		const Char* ATTR_MORPH				= "Morph";
		const Char* ATTR_MATERIAL			= "Material";
		const Char* ATTR_DUR_STATE			= "Duration State";
		const Char* ATTR_DUR_STATE_CHANGE	= "Duration Change";
		const Char* ATTR_IS_PLAYING			= "Is Playing";


	public:
		AttrViewer_MorphRendering() {}
		virtual ~AttrViewer_MorphRendering() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		COMP::MorphRendering*	_target;
	};
}