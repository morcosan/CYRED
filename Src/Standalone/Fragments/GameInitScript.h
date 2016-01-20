// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Script.h"
#include "AppSettings.h"


namespace CYRED
{
	class GameInitScript : public COMP::Script
	{
	public:
		GameInitScript() {}
		virtual ~GameInitScript() {}


	protected:
		void _OnStart	() override;


	protected:
		void _LoadScene			( AppSettings::SceneData& sceneData );
		void _LoadSceneAssets	( AppSettings::SceneData& sceneData );
	};
}