// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../../1_Required/Required.h"
#include "../Serializer.h"

#include "rapidjson\Include\document.h"


namespace CYRED
{
	class DLL JsonSerializer : public Serializer
	{
	public:
		JsonSerializer() {}
		virtual ~JsonSerializer() {};


	public:
		String		Serialize	( void* object )			override;
		void		Deserialize	( const Char* data, OUT void* object,
								  DeserFlag flag )			override;

		String		SerializeVec2	( Vector2 value )		override;
		String		SerializeVec3	( Vector3 value )		override;
		String		SerializeVec4	( Vector4 value )		override;

		Vector2		DeserializeVec2	( const Char* data )	override;
		Vector3		DeserializeVec3	( const Char* data )	override;
		Vector4		DeserializeVec4	( const Char* data )	override;


	public:
		virtual rapidjson::Value	ToJson	( void* object );
		virtual void				FromJson( rapidjson::Value& json, OUT void* object,
											  DeserFlag flag ) {}


	protected:
		static rapidjson::Document::AllocatorType _al;


	protected:
		rapidjson::Value	_ToJsonVec2		( Vector2 value );
		rapidjson::Value	_ToJsonVec3		( Vector3 value );
		rapidjson::Value	_ToJsonVec4		( Vector4 value );
		
		Vector2				_FromJsonVec2	( rapidjson::Value& json );
		Vector3				_FromJsonVec3	( rapidjson::Value& json );
		Vector4				_FromJsonVec4	( rapidjson::Value& json );
	};
}