// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "DebugManagerImpl.h"
#include "../../2_BuildingBlocks/Math/Math.h"
#include "../../2_BuildingBlocks/String/String.h"
#include "../../2_BuildingBlocks/String/FiniteString.h"
#include "../Event/EventManager.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition of SceneManager
DEFINE_LOCAL_SINGLETON( DebugManager, DebugManagerImpl )
DEFINE_LOCAL_SINGLETON_IMPL( DebugManagerImpl )


void DebugManagerImpl::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	_enabled = TRUE;
}


void DebugManagerImpl::Finalize()
{
	if ( !_isInitialized )
	{
		return;
	}
}


void DebugManagerImpl::SetEnabled( bool value )
{
	ASSERT( _isInitialized );

	_enabled = value;
}


void DebugManagerImpl::Error( const char* value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) 
	{
		return;
	}

	DebugInfo debugInfo { value };
	EventManager::Singleton()->EmitEvent( EventType::CONSOLE_ERROR, &debugInfo );
}


void DebugManagerImpl::Log( const char* value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) {
		return;
	}

	DebugInfo debugInfo { value };
	EventManager::Singleton()->EmitEvent( EventType::CONSOLE_LOG, &debugInfo );
}


void DebugManagerImpl::LogInt( int value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) {
		return;
	}

	FiniteString text( "%d", value );
	DebugInfo debugInfo { text.GetChar() };
	EventManager::Singleton()->EmitEvent( EventType::CONSOLE_LOG, &debugInfo );
}


void DebugManagerImpl::LogFloat( float value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) 
	{
		return;
	}

	FiniteString text( "%f", value );
	DebugInfo debugInfo { text.GetChar() };
	EventManager::Singleton()->EmitEvent( EventType::CONSOLE_LOG, &debugInfo );
}


void DebugManagerImpl::LogVec2( const Vector2& value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) 
	{
		return;
	}

	FiniteString text( "(%f, %f)", value.x, value.y );
	DebugInfo debugInfo { text.GetChar() };
	EventManager::Singleton()->EmitEvent( EventType::CONSOLE_LOG, &debugInfo );
}


void DebugManagerImpl::LogVec3( const Vector3& value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) 
	{
		return;
	}

	FiniteString text( "(%f, %f, %f)", value.x, value.y, value.z );
	DebugInfo debugInfo { text.GetChar() };
	EventManager::Singleton()->EmitEvent( EventType::CONSOLE_LOG, &debugInfo );
}


void DebugManagerImpl::LogVec4( const Vector4& value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) 
	{
		return;
	}

	FiniteString text( "(%f, %f, %f, %f)", value.x, value.y, value.z, value.w );
	DebugInfo debugInfo { text.GetChar() };
	EventManager::Singleton()->EmitEvent( EventType::CONSOLE_LOG, &debugInfo );
}

