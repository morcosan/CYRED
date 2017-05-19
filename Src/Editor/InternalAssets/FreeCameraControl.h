// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Script.h"
#include "CyredBuildingBlocks.h"


namespace CYRED
{
	class FreeCameraControl : public Component
	{
	public:
		FreeCameraControl( GameObject* gameObject = NULL );
		virtual ~FreeCameraControl() {}


	protected:
		void _OnEnable	()					override {}
		void _OnStart	( bool isRuntime )	override;
		void _OnUpdate	( bool isRuntime )	override;


	public:
		void Clone( Component* clone ) const override {}


	public:
		DataMap<int, bool>	myWindows;
		float				rotateSpeed;
		float				zoomSpeed;
		float				panSpeed;


	protected:
		Transform*	_transform;
	};
}