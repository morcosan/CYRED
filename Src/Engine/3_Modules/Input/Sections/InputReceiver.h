// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"
#include "InputEvent.h"


namespace CYRED
{
	ABSTRACT class DLL InputReceiver
	{
	public:
		InputReceiver() {}
		virtual ~InputReceiver() PURE_VIRTUAL {}


	public:
		void		ClearQueue		();

		UInt		GetEventsCount	();
		InputEvent&	GetEventAt		( UInt index );


	protected:
		DataArray<InputEvent>	_eventQueue;
	};
}