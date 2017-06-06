// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"


namespace CYRED
{
	ABSTRACT class DLL TimeManager
	{
		DECLARE_LOCAL_SINGLETON( TimeManager )


	public:
		virtual void	Initialize			( int desiredFPS )	PURE_VIRTUAL;
		virtual void	Finalize			()					PURE_VIRTUAL;

		virtual void	Update				()					PURE_VIRTUAL;
		virtual void	RenderUpdate		( double realTime )	PURE_VIRTUAL;

		virtual float	GetDeltaTime		()					PURE_VIRTUAL; //! in sec
		virtual float	GetRenderDeltaTime	()					PURE_VIRTUAL; //! in sec
		virtual void	SetDesiredFPS		( int desiredFPS )	PURE_VIRTUAL;
		virtual int		GetDesiredFPS		()					PURE_VIRTUAL;
		virtual double	GetGameTime			()					PURE_VIRTUAL; //! in sec
	};
}
