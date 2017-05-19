// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"


namespace CYRED
{
	class SelectorPopup;
}


namespace CYRED
{
	ABSTRACT class Selector
	{
	public:
		Selector( SelectorPopup* popup ) : _popup( popup ) {}
		virtual ~Selector() {}


	public:
		virtual void DisplayOptions( void* selected ) PURE_VIRTUAL;


	protected:
		SelectorPopup* _popup;
	};
}