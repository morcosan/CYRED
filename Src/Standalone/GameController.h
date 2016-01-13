// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"



namespace CYRED
{
	//! used to create a scene from code 
	class DLL GameController
	{
	public:
		GameController() {}
		virtual ~GameController() {}

		virtual void Start() = 0;
		virtual void Update() = 0;
	};
}