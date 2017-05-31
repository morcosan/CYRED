// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "Events.h"


namespace CYRED
{
	ABSTRACT class DLL IEventListener
	{
	public:
		IEventListener() {}
		virtual ~IEventListener() {}

		virtual void OnEvent( int eventType, void* eventData ) PURE_VIRTUAL;
	};
}