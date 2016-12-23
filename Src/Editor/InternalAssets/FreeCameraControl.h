// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Script.h"
#include "CyredBuildingBlocks.h"


namespace CYRED
{
	class FreeCameraControl : public COMP::Scripter
	{
	public:
		FreeCameraControl( GameObject* gameObject = NULL );
		virtual ~FreeCameraControl() {}


	protected:
		void _OnStart	() override;
		void _OnUpdate	() override;


	public:
		DataMap<Int, Bool>	myWindows;
		Float				rotateSpeed;
		Float				zoomSpeed;
		Float				panSpeed;


	protected:
		COMP::Transform*	_transform;
	};
}