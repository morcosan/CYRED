// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredModule_Event.h"


namespace CYRED
{
	class Console : public IEventListener
	{
	public:
		Console();
		virtual ~Console();


	public:
		virtual void OnEvent( EventType eType, void* eData ) override;

	};
}