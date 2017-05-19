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
		virtual void Initialize	()	PURE_VIRTUAL;
		virtual void Finalize	()	PURE_VIRTUAL;

		virtual void RegisterListener	( EventType eType, IEventListener* listener )	PURE_VIRTUAL;
		virtual void UnregisterListener	( EventType eType, IEventListener* listener )	PURE_VIRTUAL;

		virtual void EmitEvent			( EventType eType, void* eData )				PURE_VIRTUAL;
	};
}