// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "DebugManagerImpl.h"
#include "Fragments\Console.h"
#include "../../2_BuildingBlocks/Math/Math.h"
#include "../../2_BuildingBlocks/String/String.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition of SceneManager
DEFINE_LOCAL_SINGLETON( DebugManager, DebugManagerImpl )

DEFINE_LOCAL_SINGLETON_IMPL( DebugManagerImpl )



void DebugManagerImpl::Initialize( Console* console )
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	_console = console;
	_enabled = TRUE;
}


void DebugManagerImpl::Finalize()
{
	ASSERT( _isInitialized );

	Memory::Free( _console );
	_console = NULL;
}


void DebugManagerImpl::SetEnabled( Bool value )
{
	ASSERT( _isInitialized );

	_enabled = value;
}


void DebugManagerImpl::Log( const Char* value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) 
	{
		return;
	}

	_console->Log( value );
}


void DebugManagerImpl::Log( Int value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) 
	{
		return;
	}

	Char text[MAX_SIZE_CUSTOM_STRING];
	CUSTOM_STRING( text, "%d", value );

	_console->Log( text );
}


void DebugManagerImpl::Log( Float value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) 
	{
		return;
	}

	Char text[MAX_SIZE_CUSTOM_STRING];
	CUSTOM_STRING( text, "%f", value );

	_console->Log( text );
}


void DebugManagerImpl::Log( Double value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) 
	{
		return;
	}

	Char text[MAX_SIZE_CUSTOM_STRING];
	CUSTOM_STRING( text, "%lf", value );

	_console->Log( text );
}


void DebugManagerImpl::Log( const Vector2& value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) 
	{
		return;
	}

	Char text[MAX_SIZE_CUSTOM_STRING];
	CUSTOM_STRING( text, "(%f, %f)", value.x, value.y );

	_console->Log( text );
}


void DebugManagerImpl::Log( const Vector3& value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) 
	{
		return;
	}

	Char text[MAX_SIZE_CUSTOM_STRING];
	CUSTOM_STRING( text, "(%f, %f, %f)", value.x, value.y, value.z );

	_console->Log( text );
}


void DebugManagerImpl::Log( const Vector4& value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) 
	{
		return;
	}

	Char text[MAX_SIZE_CUSTOM_STRING];
	CUSTOM_STRING( text, "(%f, %f, %f, %f)", value.x, value.y, value.z, value.w );

	_console->Log( text );
}


void DebugManagerImpl::Log( const Quaternion& value )
{
	ASSERT( _isInitialized );
	if ( !_enabled ) 
	{
		return;
	}

	Vector3 euler = value.ToEulerAngles();

	Char text[MAX_SIZE_CUSTOM_STRING];
	CUSTOM_STRING( text, "(%f, %f, %f)", euler.x, euler.y, euler.z );

	_console->Log( text );
}

