// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"

namespace CYRED
{
	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct Quaternion;
	class  String;

	struct DebugInfo
	{
		cchar* message;
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

		virtual void SetEnabled		( bool value )					PURE_VIRTUAL;

		virtual void Error			( cchar* value )			PURE_VIRTUAL;

		virtual void Log			( cchar* value )			PURE_VIRTUAL;
		virtual void LogInt			( int value )					PURE_VIRTUAL;
		virtual void LogFloat		( float value )					PURE_VIRTUAL;
		virtual void LogVec2		( const Vector2& value )		PURE_VIRTUAL;
		virtual void LogVec3		( const Vector3& value )		PURE_VIRTUAL;
		virtual void LogVec4		( const Vector4& value )		PURE_VIRTUAL;
	};
}