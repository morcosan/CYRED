// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"
#include "CyredModule_Render.h"


namespace CYRED
{
	class AttrViewer_Light : public AttrViewer
	{
	public:
		cchar* TITLE				= "Light";

		cchar* ATTR_LIGHT_TYPE		= "Light Type";
		cchar* ATTR_INTENSITY		= "Intensity";
		cchar* ATTR_COLOR			= "Color";
		cchar* ATTR_RANGE			= "Range";
		cchar* ATTR_SPOT_ANGLE		= "Spot Angle";


	public:
		AttrViewer_Light();
		virtual ~AttrViewer_Light() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	private:
		Light* _target;

		DataArray<cchar*>	_lightTypes;

		int			_GetIndexForType	( LightType type );
		LightType	_GetTypeForIndex	( int index );

		void		_ChangeVisibility	();
	};
}