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

			void RegisterListener	( EventType eType, IEventListener* listener )	override;
			void UnregisterListener	( EventType eType, IEventListener* listener )	override;

			void EmitEvent			( EventType eType, void* eData )				override;


		protected:
			DataArray< DataArray<IEventListener*> > _listeners;

			//! eType is required UInt for the ALL case
			void _AddListener		( UInt eType, IEventListener* listener );
			void _RemoveListener	( UInt eType, IEventListener* listener );
		};
	}
}