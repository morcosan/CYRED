// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "Selector.h"


namespace CYRED
{
	class Selector_Scene : public Selector
	{
	public:
		static const char* const OPTION_NULL;
		static const char* const TYPE;


	public:
		Selector_Scene( SelectorPopup* popup ) : Selector( popup ) {}
		virtual ~Selector_Scene() {}


	public:
		void DisplayOptions( void* selected ) override;
	};
}