// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "AttrViewer.h"


namespace CYRED
{
	class AttrViewer_CyredProj : public AttrViewer
	{
	public:
		const Char* TITLE				= "Project Settings";

		const Char* ATTR_APP_CONFIG		= "AppConfig";
		const Char* ATTR_APP_NAME		= "AppName";
		const Char* ATTR_FULLSCREEN		= "Fullscreen";
		const Char* ATTR_WIDTH			= "Width";
		const Char* ATTR_HEIGHT			= "Height";
		const Char* ATTR_POS_X			= "Pos X";
		const Char* ATTR_POS_Y			= "Pos Y";
		const Char* ATTR_FPS			= "FPS";
		const Char* ATTR_START_SCENE	= "Start Scene";
		const Char* ATTR_BUILD_WINDOWS	= "Build Windows";
		const Char* ATTR_BUILD_ANDROID	= "Build Android";


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