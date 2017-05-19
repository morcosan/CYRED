// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	class AttrViewer_CyredProj : public AttrViewer
	{
	public:
		const char* TITLE				= "Project Settings";

		const char* ATTR_APP_CONFIG		= "AppConfig";
		const char* ATTR_APP_NAME		= "AppName";
		const char* ATTR_FULLSCREEN		= "Fullscreen";
		const char* ATTR_WIDTH			= "Width";
		const char* ATTR_HEIGHT			= "Height";
		const char* ATTR_POS_X			= "Pos X";
		const char* ATTR_POS_Y			= "Pos Y";
		const char* ATTR_FPS			= "FPS";
		const char* ATTR_START_SCENE	= "Start Scene";
		const char* ATTR_BUILD_WINDOWS	= "Build Windows";
		const char* ATTR_BUILD_ANDROID	= "Build Android";


	public:
		AttrViewer_CyredProj() {}
		virtual ~AttrViewer_CyredProj() {}


	protected:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _OnUpdateGUI		()					override;
		void _OnUpdateTarget	()					override;


	protected:
		void _WriteToFile();
	};
}