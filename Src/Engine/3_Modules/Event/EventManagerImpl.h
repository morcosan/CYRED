// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "EventManager.h"
#include "../../2_BuildingBlocks/Data/DataArray.h"
#include "../../2_BuildingBlocks/Data/DataMap.h"


namespace CYRED
{
	namespace NotAPI
	{
		class EventManagerImpl : public EventManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( EventManagerImpl )


		public:
			void Initialize			()											override;
			void Finalize			()											override;

			void RegisterListener	( IEventListener* listener, int eventType )	override;
			void UnregisterListener	( IEventListener* listener, int eventType )	override;

			void EmitEvent			( int eventType, void* eventData )			override;


		private:
			DataMap<int, DataArray<IEventListener*>> _listeners;
		};
	}
}