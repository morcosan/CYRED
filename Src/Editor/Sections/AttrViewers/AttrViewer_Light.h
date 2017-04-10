// Copyright (c) 2015 Morco (www.morco.ro)
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
		const Char* TITLE				= "Light";

		const Char* ATTR_LIGHT_TYPE		= "Light Type";
		const Char* ATTR_INTENSITY		= "Intensity";
		const Char* ATTR_COLOR			= "Color";
		const Char* ATTR_RANGE			= "Range";
		const Char* ATTR_SPOT_ANGLE		= "Spot Angle";


	public:
		AttrViewer_Light();
		virtual ~AttrViewer_Light() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		Light* _target;

		DataArray<const Char*>	_lightTypes;

		Int			_GetIndexForType	( LightType type );
		LightType	_GetTypeForIndex	( Int index );

		void		_ChangeVisibility	();
	};
}