// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "Selector.h"


namespace CYRED
{
	class Selector_Mesh : public Selector
	{
	public:
		static const char* const OPTION_NULL;
		static const char* const TYPE;


	public:
		Selector_Mesh( SelectorPopup* popup ) : Selector( popup ) {}
		virtual ~Selector_Mesh() {}


	public:
		void DisplayOptions( void* selected ) override;
	};
}