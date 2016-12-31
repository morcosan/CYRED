// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "Selector.h"


namespace CYRED
{
	class Selector_Material : public Selector
	{
	public:
		static const Char* const OPTION_NULL;
		static const Char* const TYPE;


	public:
		Selector_Material( SelectorPopup* popup ) : Selector( popup ) {}
		virtual ~Selector_Material() {}


	public:
		void DisplayOptions( void* selected ) override;
	};
}