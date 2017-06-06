// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "TimeManager.h"


namespace CYRED
{
	namespace NonAPI
	{
		class TimeManagerImpl : public TimeManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( TimeManagerImpl )


		public:
			void	Initialize			( int desiredFPS )	override;
			void	Finalize			()					override;
			void	Update				()					override;
			void	RenderUpdate		( double realTime )	override;

			float	GetDeltaTime		()					override;
			float	GetRenderDeltaTime	()					override;
			void	SetDesiredFPS		( int desiredFPS )	override;
			int		GetDesiredFPS		()					override;
			double	GetGameTime			()					override;


		private:
			int		_desiredFPS;
			float	_deltaTime;
			float	_renderDeltaTime;
			double	_gameTime;
			double	_lastRenderTime;
		};
	}
}
