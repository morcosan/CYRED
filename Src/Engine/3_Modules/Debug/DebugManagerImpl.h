// Copyright (c) 2015 Morco (www.morco.ro)
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
			void Initialize	( Console* console )	override;
			void Finalize		()						override;
				  
			void SetEnabled	( Bool value )					override;
			void Log			( const Char* value )			override;
			void Log			( Int value )					override;
			void Log			( Float value )					override;
			void Log			( Double value )				override;
			void Log			( const Vector2& value )		override;
			void Log			( const Vector3& value )		override;
			void Log			( const Vector4& value )		override;
			void Log			( const Quaternion& value )		override;


		protected:
			Console*	_console;
			Bool		_enabled;
		};
	}
}