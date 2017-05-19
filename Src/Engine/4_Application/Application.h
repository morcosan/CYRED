// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../1_Required/Required.h"


namespace CYRED
{
	ABSTRACT class DLL Application
	{
		DECLARE_REMOTE_SINGLETON( Application )


	public:
		virtual void Exit() PURE_VIRTUAL;
	};
}