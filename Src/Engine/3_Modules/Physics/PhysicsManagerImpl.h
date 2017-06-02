// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "PhysicsManager.h"

class btDynamicsWorld;


namespace CYRED
{
	namespace NotAPI
	{
		class PhysicsManagerImpl : public PhysicsManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( PhysicsManagerImpl )


		public:
			void Initialize	()	override;
			void Finalize	()	override;


		private:
			btDynamicsWorld*	_btWorld;
		};
	}
}