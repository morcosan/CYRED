// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "TimeManagerImpl.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition of SceneManager
DEFINE_LOCAL_SINGLETON( TimeManager, TimeManagerImpl )

DEFINE_LOCAL_SINGLETON_IMPL( TimeManagerImpl )



void TimeManagerImpl::Initialize( UInt desiredFPS )
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	_gameTime = 0;
	_lastRenderTime = 0;
	_desiredFPS = desiredFPS;
}


void TimeManagerImpl::Finalize()
{
	if ( !_isInitialized )
	{
		return;
	}
}


void TimeManagerImpl::Update()
{
	ASSERT( _isInitialized );

	_deltaTime = GetDesiredDelta();
	_gameTime += _deltaTime;
}


void TimeManagerImpl::RenderUpdate( Double realTime )
{
	ASSERT( _isInitialized );

	_renderDeltaTime = CAST_S( Float, realTime - _lastRenderTime );
	_lastRenderTime = realTime;
}


Float TimeManagerImpl::GetDeltaTime()
{
	ASSERT( _isInitialized );

	return _deltaTime;
}


Float TimeManagerImpl::GetRenderDeltaTime()
{
	ASSERT( _isInitialized );

	return _renderDeltaTime;
}


void TimeManagerImpl::SetDesiredFPS( UInt desiredFPS )
{
	ASSERT( _isInitialized );

	_desiredFPS = desiredFPS;
}


UInt TimeManagerImpl::GetDesiredFPS()
{
	ASSERT( _isInitialized );

	return _desiredFPS;
}


Float TimeManagerImpl::GetDesiredDelta()
{
	ASSERT( _isInitialized );

	return (1.0f / _desiredFPS);
}


Double TimeManagerImpl::GetGameTime()
{
	ASSERT( _isInitialized );

	return _gameTime;
}
