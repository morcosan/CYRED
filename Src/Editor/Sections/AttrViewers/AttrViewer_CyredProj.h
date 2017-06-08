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
		cchar* const TITLE				= "Project Settings";

		cchar* const ATTR_APP_CONFIG	= "AppConfig";
		cchar* const ATTR_APP_NAME		= "AppName";
		cchar* const ATTR_FULLSCREEN	= "Fullscreen";
		cchar* const ATTR_WIDTH			= "Width";
		cchar* const ATTR_HEIGHT		= "Height";
		cchar* const ATTR_POS_X			= "Pos X";
		cchar* const ATTR_POS_Y			= "Pos Y";
		cchar* const ATTR_FPS			= "FPS";
		cchar* const ATTR_START_SCENE	= "Start Scene";
		cchar* const ATTR_BUILD_WINDOWS	= "Build Windows";
		cchar* const ATTR_BUILD_ANDROID	= "Build Android";


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