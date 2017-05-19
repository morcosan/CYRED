// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredApplication.h"
#include "CyredModule_Script.h"


namespace CYRED
{
	class GameInitScript : public Scripter
	{
	public:
		GameInitScript( AppConfig* appConfig );
		virtual ~GameInitScript() {}


	private:
		void _OnStart( bool isRuntime ) override;


	private:
		AppConfig* _appConfig;

		void _LoadAllAssets();
	};
}