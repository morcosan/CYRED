// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"


struct lua_State;


namespace CYRED
{
	class Script;
}


namespace CYRED
{
	ABSTRACT class DLL ScriptManager
	{
		DECLARE_LOCAL_SINGLETON( ScriptManager )


	public:
		virtual void Initialize	()	PURE_VIRTUAL;
		virtual void Finalize	()	PURE_VIRTUAL;

		virtual lua_State* GetLuaState() const PURE_VIRTUAL;
	};
}