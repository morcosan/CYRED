// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"
#include "CyredModule_Render.h"


namespace CYRED
{
	class AttrViewer_Camera : public AttrViewer
	{
	public:
		cchar* TITLE				= "Camera";

		cchar* ATTR_TYPE			= "Camera Type";
		cchar* ATTR_FOVY_ANGLE		= "FovY Angle";
		cchar* ATTR_NEAR_CLIPPING	= "Near Clipping";
		cchar* ATTR_FAR_CLIPPING	= "Far Clipping";
		cchar* ATTR_ORTH_HEIGHT	= "Ortho Height";


	public:
		AttrViewer_Camera();
		virtual ~AttrViewer_Camera() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		Camera*		_target;

		DataArray<cchar*>	_cameraTypes;

		int			_GetIndexForType	( CameraType type );
		CameraType	_GetTypeForIndex	( int index );

		void		_ChangeVisibility	();
	};
}