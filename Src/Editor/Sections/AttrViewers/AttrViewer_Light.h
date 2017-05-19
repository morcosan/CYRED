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
		const char* TITLE				= "Light";

		const char* ATTR_LIGHT_TYPE		= "Light Type";
		const char* ATTR_INTENSITY		= "Intensity";
		const char* ATTR_COLOR			= "Color";
		const char* ATTR_RANGE			= "Range";
		const char* ATTR_SPOT_ANGLE		= "Spot Angle";


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

		DataArray<const char*>	_lightTypes;

		int			_GetIndexForType	( LightType type );
		LightType	_GetTypeForIndex	( int index );

		void		_ChangeVisibility	();
	};
}