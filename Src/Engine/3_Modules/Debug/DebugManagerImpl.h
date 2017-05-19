// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "DebugManager.h"


namespace CYRED
{
	namespace NotAPI
	{
		class DebugManagerImpl : public DebugManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( DebugManagerImpl )


		public:
			void Initialize	()								override;
			void Finalize	()								override;
				  
			void SetEnabled	( bool value )					override;

			void Error		( const char* value )			override;

			void Log		( const char* value )			override;
			void LogInt		( int value )					override;
			void LogFloat	( float value )					override;
			void LogVec2	( const Vector2& value )		override;
			void LogVec3	( const Vector3& value )		override;
			void LogVec4	( const Vector4& value )		override;


		private:
			bool		_enabled;
		};
	}
}