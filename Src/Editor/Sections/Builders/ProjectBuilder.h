// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"


namespace CYRED
{
	ABSTRACT class ProjectBuilder
	{
		DECLARE_LOCAL_SINGLETON( ProjectBuilder )


	public:
		virtual void Initialize	()	PURE_VIRTUAL;
		virtual void Finalize	()	PURE_VIRTUAL;

		virtual void BuildWindows( cchar* buildPath )	PURE_VIRTUAL;
		virtual void BuildAndroid( cchar* buildPath )	PURE_VIRTUAL;
	};
}