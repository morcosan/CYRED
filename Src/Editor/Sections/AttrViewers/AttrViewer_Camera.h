// Copyright (c) 2015 Morco (www.morco.ro)
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
		const char* TITLE				= "Camera";

		const char* ATTR_TYPE			= "Camera Type";
		const char* ATTR_FOVY_ANGLE		= "FovY Angle";
		const char* ATTR_NEAR_CLIPPING	= "Near Clipping";
		const char* ATTR_FAR_CLIPPING	= "Far Clipping";
		const char* ATTR_ORTH_HEIGHT	= "Ortho Height";


	public:
		AttrViewer_Camera();
		virtual ~AttrViewer_Camera() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		Camera*		_target;

		DataArray<const char*>	_cameraTypes;

		int			_GetIndexForType	( CameraType type );
		CameraType	_GetTypeForIndex	( int index );

		void		_ChangeVisibility	();
	};
}