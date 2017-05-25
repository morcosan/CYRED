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
		cchar* TITLE				= "Project Settings";

		cchar* ATTR_APP_CONFIG		= "AppConfig";
		cchar* ATTR_APP_NAME		= "AppName";
		cchar* ATTR_FULLSCREEN		= "Fullscreen";
		cchar* ATTR_WIDTH			= "Width";
		cchar* ATTR_HEIGHT			= "Height";
		cchar* ATTR_POS_X			= "Pos X";
		cchar* ATTR_POS_Y			= "Pos Y";
		cchar* ATTR_FPS			= "FPS";
		cchar* ATTR_START_SCENE	= "Start Scene";
		cchar* ATTR_BUILD_WINDOWS	= "Build Windows";
		cchar* ATTR_BUILD_ANDROID	= "Build Android";


	public:
		AttrViewer_CyredProj() {}
		virtual ~AttrViewer_CyredProj() {}


	private:
		void _OnInitialize		()					override;
		void _OnChangeTarget	( void* target )	override;
		void _UpdateGUI		()					override;
		void _UpdateTarget	()					override;


	private:
		void _WriteToFile();
	};
}