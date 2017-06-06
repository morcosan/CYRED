// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "TimeManagerImpl.h"


using namespace CYRED;
using namespace NonAPI;


//! deferred definition of SceneManager
DEFINE_LOCAL_SINGLETON( TimeManager, TimeManagerImpl )
DEFINE_LOCAL_SINGLETON_IMPL( TimeManagerImpl )


void TimeManagerImpl::Initialize( int desiredFPS )
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

	_deltaTime = 1.0f / _desiredFPS;
	_gameTime += _deltaTime;
}


void TimeManagerImpl::RenderUpdate( double realTime )
{
	ASSERT( _isInitialized );

	_renderDeltaTime = CAST_S( float, realTime - _lastRenderTime );
	_lastRenderTime = realTime;
}


// in seconds
float TimeManagerImpl::GetDeltaTime()
{
	ASSERT( _isInitialized );

	return _deltaTime;
}


float TimeManagerImpl::GetRenderDeltaTime()
{
	ASSERT( _isInitialized );

	return _renderDeltaTime;
}


void TimeManagerImpl::SetDesiredFPS( int desiredFPS )
{
	ASSERT( _isInitialized );

	_desiredFPS = desiredFPS;
}


int TimeManagerImpl::GetDesiredFPS()
{
	ASSERT( _isInitialized );

	return _desiredFPS;
}


double TimeManagerImpl::GetGameTime()
{
	ASSERT( _isInitialized );

	return _gameTime;
}
