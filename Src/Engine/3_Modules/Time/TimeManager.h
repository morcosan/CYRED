// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"



namespace CYRED
{
	ABSTRACT class DLL TimeManager
	{
		DECLARE_LOCAL_SINGLETON( TimeManager )


	public:
		virtual void Initialize	( UInt desiredFPS )	PURE_VIRTUAL;
		virtual void Finalize	()					PURE_VIRTUAL;

		//! will be called by application with each update in loop
		virtual void Update			()					PURE_VIRTUAL;
		virtual void RenderUpdate	( Double realTime )	PURE_VIRTUAL;

		virtual Float	GetDeltaTime		()					PURE_VIRTUAL; //! in sec
		virtual Float	GetRenderDeltaTime	()					PURE_VIRTUAL; //! in sec
		virtual void	SetDesiredFPS		( UInt desiredFPS )	PURE_VIRTUAL;
		virtual UInt	GetDesiredFPS		()					PURE_VIRTUAL;
		virtual Double	GetGameTime			()					PURE_VIRTUAL; //! in sec
	};
}