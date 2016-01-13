// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredStandalone.h"
#include "CyredEngine.h"


using namespace CYRED;

class SampleController : public GameController
{
	virtual void Start() 
	{
		SceneManager::Singleton()->OpenNewScene();

		GameObject* go = SceneManager::Singleton()->NewGameObject( "first object" );

	}

	virtual void Update()
	{

	}
};