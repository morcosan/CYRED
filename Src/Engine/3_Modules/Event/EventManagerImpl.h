// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "EventManager.h"
#include "../../2_BuildingBlocks/Data/DataArray.h"
#include "../../2_BuildingBlocks/Data/DataMap.h"


namespace CYRED
{
	namespace NonAPI
	{
		class EventManagerImpl : public EventManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( EventManagerImpl )


		public:
			void Initialize	()											override;
			void Update		()											override;
			void Finalize	()											override;

			void Register	( IEventListener* listener, int eventType,
							  EventListenMode mode )					override;
			void Unregister	( IEventListener* listener, int eventType,
							  EventListenMode mode )					override;

			void EmitEvent	( int eventType, void* eventData )			override;
			void PushEvent	( int eventType, void* eventData )			override;


		private:
			struct _PushedEvent
			{
				int		eventType;
				void*	eventData;
			};

			DataMap<int, DataArray<IEventListener*>>	_listenersSync;
			DataMap<int, DataArray<IEventListener*>>	_listenersAsyncAll;
			DataMap<int, DataArray<IEventListener*>>	_listenersAsyncLast;

			DataArray<_PushedEvent>						_pushedEvents;


		private:
			void _SendEvent			( DataMap<int, DataArray<IEventListener*>>* listeners,
									  int eventType, void* eventData );
		};
	}
}