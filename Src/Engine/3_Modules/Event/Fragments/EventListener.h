// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "Events.h"


namespace CYRED
{
	ABSTRACT class DLL EventListener
	{
	public:
		EventListener() {}
		virtual ~EventListener() {}

		virtual void OnEvent( EventType eType, EventName eName, void* eSource ) PURE_VIRTUAL;
	};
}