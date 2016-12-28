// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "TimeManager.h"


namespace CYRED
{
	namespace NotAPI
	{
		class TimeManagerImpl : public TimeManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( TimeManagerImpl )


		public:
			void Initialize		( UInt desiredFPS )	override;
			void Finalize		()					override;
			void Update			()					override;
			void RenderUpdate	( Double realTime )	override;

			Float	GetDeltaTime		()					override;
			Float	GetRenderDeltaTime	()					override;
			void	SetDesiredFPS		( UInt desiredFPS )	override;
			UInt	GetDesiredFPS		()					override;
			Double	GetGameTime			()					override;


		protected:
			UInt	_desiredFPS;
			Float	_deltaTime;
			Float	_renderDeltaTime;
			Double	_gameTime;
			Double	_lastRenderTime;
		};
	}
}