// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "Required.h"


namespace CYRED
{
	//! class for memory management
	//! all allocations and deletions must be done via this
	class DLL Memory
	{
	private:
		Memory() {}

	public:
		template <typename T, typename... Args>
		static T* Alloc( Args... args )
		{
			// TODO
			return new T( args... );
		}

		template <typename T>
		static void Free( T* ptr )
		{
			// TODO
			if ( ptr != NULL )
			{
				delete( ptr );
			}
		}

		template <typename T>
		static T* AllocArray( int size )
		{
			// TODO
			return new T[size];
		}

		template <typename T>
		static void FreeArray( T* ptr )
		{
			// TODO
			if ( ptr != NULL )
			{
				delete[]( ptr );
			}
		}
	};
}

