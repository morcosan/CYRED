// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Data/DataMap.h"
#include "Serializer.h"



namespace CYRED
{
	class DLL SerializeSystem
	{
	public:
		SerializeSystem() {}
		virtual ~SerializeSystem() {};


	public:
		template <class T> String	Serialize		( const T* object );	
		template <class T> void		Deserialize		( const char* data, OUT T* object, 
													  DeserFlag flag = DeserFlag::FULL );
		template <class T> void		AddSerializer	( Serializer* serializer );


	public:
		virtual String		SerializeVec2	( Vector2 value )		PURE_VIRTUAL;
		virtual String		SerializeVec3	( Vector3 value )		PURE_VIRTUAL;
		virtual String		SerializeVec4	( Vector4 value )		PURE_VIRTUAL;
		virtual Vector2		DeserializeVec2	( const char* data )	PURE_VIRTUAL;
		virtual Vector3		DeserializeVec3	( const char* data )	PURE_VIRTUAL;
		virtual Vector4		DeserializeVec4	( const char* data )	PURE_VIRTUAL;


	protected:
		DataMap<TYPE_INDEX, Serializer*>	_serializers;
	};



	template<class T>
	String SerializeSystem::Serialize( const T* object )
	{
		ASSERT( _serializers.Has( GET_TYPE_INDEX( T ) ) );
		
		return _serializers.Get( GET_TYPE_INDEX( T ) )->Serialize( object );
	}


	template<class T>
	void SerializeSystem::Deserialize( const char* data, OUT T* object,
									   DeserFlag flag )
	{
		ASSERT( _serializers.Has( GET_TYPE_INDEX( T ) ) );

		_serializers.Get( GET_TYPE_INDEX( T ) )->Deserialize( data, object, flag );
	}


	template<class T>
	void SerializeSystem::AddSerializer( Serializer* serializer )
	{
		_serializers.Set( GET_TYPE_INDEX( T ), serializer );
	}
}