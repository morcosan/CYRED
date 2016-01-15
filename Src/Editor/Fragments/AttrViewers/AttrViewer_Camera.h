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
		const Char* TITLE				= "Camera";
		const Char* ATTR_TYPE			= "Type";
		const Char* ATTR_FOVY_ANGLE		= "FovY Angle";
		const Char* ATTR_NEAR_CLIPPING	= "Near Clipping";
		const Char* ATTR_FAR_CLIPPING	= "Far Clipping";
		const Char* ATTR_ASPECT_RATIO	= "Aspect Ratio";
		const Char* ATTR_ORTH_SIZE		= "Ortho Size";


	public:
		AttrViewer_Camera();
		virtual ~AttrViewer_Camera() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		COMP::Camera*		_target;

		DataArray<const Char*>	_cameraTypes;

		Int			_GetIndexForType( CameraType type );
		CameraType	_GetTypeForIndex( Int index );
	};
}