// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "EventManager.h"
#include "../../2_BuildingBlocks/Data/DataArray.h"


namespace CYRED
{

	namespace NotAPI
	{
		class EventManagerImpl : public EventManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( EventManagerImpl )


		public:
			void Initialize	()	override;
			void Finalize	()	override;

			void RegisterListener	( EventType eType, EventListener* listener )	override;
			void UnregisterListener	( EventType eType, EventListener* listener )	override;

			void EmitEvent			( EventType eType, EventName eName, 
									  void* eSource )								override;


		protected:
			DataArray< DataArray<EventListener*> > _listeners;

			//! eType is required UInt for the ALL case
			void _AddListener		( UInt eType, EventListener* listener );
			void _RemoveListener	( UInt eType, EventListener* listener );
		};
	}
}