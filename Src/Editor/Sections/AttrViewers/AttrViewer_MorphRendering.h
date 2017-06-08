// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"

namespace CYRED
{
	class MorphRendering;
}


namespace CYRED
{
	class AttrViewer_MorphRendering : public AttrViewer
	{
	public:
		cchar* const TITLE					= "MorphRendering";

		cchar* const ATTR_MORPH				= "Morph";
		cchar* const ATTR_MATERIAL			= "Material";
		cchar* const ATTR_DUR_STATE			= "Duration State";
		cchar* const ATTR_DUR_STATE_CHANGE	= "Duration Change";
		cchar* const ATTR_IS_PLAYING		= "Is Playing";


	public:
		AttrViewer_MorphRendering() {}
		virtual ~AttrViewer_MorphRendering() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		MorphRendering*	_target;
	};
}