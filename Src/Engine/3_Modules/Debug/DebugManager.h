// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"


namespace CYRED
{
	struct Vector2;
	struct Vector3;
	struct Vector4;
	class Quaternion;
	class String;

	struct DebugInfo
	{
		const Char* message;
	};
}


namespace CYRED
{
	ABSTRACT class DLL DebugManager
	{
		DECLARE_LOCAL_SINGLETON( DebugManager )


	public:
		virtual void Initialize		()								PURE_VIRTUAL;
		virtual void Finalize		()								PURE_VIRTUAL;

		virtual void SetEnabled		( Bool value )					PURE_VIRTUAL;

		virtual void Error			( const Char* value )			PURE_VIRTUAL;

		virtual void Log			( const Char* value )			PURE_VIRTUAL;
		virtual void LogInt			( Int value )					PURE_VIRTUAL;
		virtual void LogFloat		( Float value )					PURE_VIRTUAL;
		virtual void LogVec2		( const Vector2& value )		PURE_VIRTUAL;
		virtual void LogVec3		( const Vector3& value )		PURE_VIRTUAL;
		virtual void LogVec4		( const Vector4& value )		PURE_VIRTUAL;
	};
}