// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../DLL_Include/CyredRequired.h"


namespace CYRED
{
	ABSTRACT class DLL Application
	{
		DECLARE_REMOTE_SINGLETON( Application )


	public:
		virtual void Exit() PURE_VIRTUAL;
	};
}