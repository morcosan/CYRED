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
		class MorphRendering;
	}
}


namespace CYRED
{
	class AttrViewer_MorphRendering : public AttrViewer
	{
	public:
		const char* TITLE					= "MorphRendering";

		const char* ATTR_MORPH				= "Morph";
		const char* ATTR_MATERIAL			= "Material";
		const char* ATTR_DUR_STATE			= "Duration State";
		const char* ATTR_DUR_STATE_CHANGE	= "Duration Change";
		const char* ATTR_IS_PLAYING			= "Is Playing";


	public:
		AttrViewer_MorphRendering() {}
		virtual ~AttrViewer_MorphRendering() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	private:
		MorphRendering*	_target;
	};
}