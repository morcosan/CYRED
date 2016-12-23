// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"
#include "Fragments\Events.h"


namespace CYRED
{
	class EventListener;
}


namespace CYRED
{
	ABSTRACT class DLL EventManager
	{
		DECLARE_LOCAL_SINGLETON( EventManager )


	public:
		virtual void Initialize	()	PURE_VIRTUAL;
		virtual void Finalize	()	PURE_VIRTUAL;

		virtual void RegisterListener	( EventType eType, EventListener* listener )	PURE_VIRTUAL;
		virtual void UnregisterListener	( EventType eType, EventListener* listener )	PURE_VIRTUAL;

		virtual void EmitEvent			( EventType eType, EventName eName, 
										  void* eSource )								PURE_VIRTUAL;
	};
}