// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"


namespace CYRED
{
	class Console;
	struct Vector2;
	struct Vector3;
	struct Vector4;
	class Quaternion;
	class String;
}


namespace CYRED
{
	ABSTRACT class DLL DebugManager
	{
		DECLARE_LOCAL_SINGLETON( DebugManager )


	public:
		virtual void Initialize	( Console* console )	PURE_VIRTUAL;
		virtual void Finalize		()						PURE_VIRTUAL;

		virtual void SetEnabled	( Bool value )					PURE_VIRTUAL;
		virtual void Log			( const Char* value )			PURE_VIRTUAL;
		virtual void Log			( Int value )					PURE_VIRTUAL;
		virtual void Log			( Float value )					PURE_VIRTUAL;
		virtual void Log			( Double value )				PURE_VIRTUAL;
		virtual void Log			( const Vector2& value )		PURE_VIRTUAL;
		virtual void Log			( const Vector3& value )		PURE_VIRTUAL;
		virtual void Log			( const Vector4& value )		PURE_VIRTUAL;
		virtual void Log			( const Quaternion& value )		PURE_VIRTUAL;
	};
}