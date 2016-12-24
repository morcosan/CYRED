// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredApplication.h"
#include "CyredModule_Script.h"


namespace CYRED
{
	class GameInitScript : public COMP::Scripter
	{
	public:
		GameInitScript( AppConfig* appConfig );
		virtual ~GameInitScript() {}


	protected:
		void _OnStart( Bool isRuntime ) override;


	protected:
		AppConfig* _appConfig;

		void _LoadAllAssets();
	};
}