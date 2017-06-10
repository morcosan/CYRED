// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"
#include "Sections\Events.h"

namespace CYRED
{
	class IEventListener;

	namespace Enum_EventListenMode
	{
		enum Enum
		{
			SYNC
			, ASYNC_ALL
			, ASYNC_LAST
		};
	}
	typedef Enum_EventListenMode::Enum	EventListenMode;
}


namespace CYRED
{
	ABSTRACT class DLL EventManager
	{
		DECLARE_LOCAL_SINGLETON( EventManager )


	public:
		virtual void Initialize	()											PURE_VIRTUAL;
		virtual void Update		()											PURE_VIRTUAL;
		virtual void Finalize	()											PURE_VIRTUAL;

		virtual void Register	( IEventListener* listener, int eventType,
								  EventListenMode mode )					PURE_VIRTUAL;
		virtual void Unregister	( IEventListener* listener, int eventType,
								  EventListenMode mode )					PURE_VIRTUAL;

		virtual void EmitEvent	( int eventType, void* eventData )			PURE_VIRTUAL;
		virtual void PushEvent	( int eventType, void* eventData )			PURE_VIRTUAL;
	};
}