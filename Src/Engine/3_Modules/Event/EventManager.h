// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"
#include "Sections\Events.h"

namespace CYRED
{
	class IEventListener;
}


namespace CYRED
{
	ABSTRACT class DLL EventManager
	{
		DECLARE_LOCAL_SINGLETON( EventManager )


	public:
		virtual void Initialize			()											PURE_VIRTUAL;
		virtual void Finalize			()											PURE_VIRTUAL;

		virtual void RegisterListener	( IEventListener* listener, int eventType )	PURE_VIRTUAL;
		virtual void UnregisterListener	( IEventListener* listener, int eventType )	PURE_VIRTUAL;

		virtual void EmitEvent			( int eventType, void* eventData )			PURE_VIRTUAL;
	};
}