// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer.h"
#include "../../../Debug/DebugManager.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Document::AllocatorType JsonSerializer::_al;


String JsonSerializer::Serialize( const void* object )
{
	rapidjson::Value json = ToJson( object );

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer( buffer );
	json.Accept( writer );

	return buffer.GetString();
}


void JsonSerializer::Deserialize( cchar* data, OUT void* object, DeserFlag flag )
{
	rapidjson::Document json;
	json.Parse<0>( data );

	if ( json.IsObject() )
	{
		FromJson( json, object, flag );
	}
	else
	{
		DebugManager::Singleton()->Log( MSG_WARNING_LOADING );
	}
}


String JsonSerializer::SerializeVec2( Vector2 value )
{
	rapidjson::Value json = _ToJsonVec2( value );

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer( buffer );
	json.Accept( writer );

	return buffer.GetString();
}


String JsonSerializer::SerializeVec3( Vector3 value )
{
	rapidjson::Value json = _ToJsonVec3( value );

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer( buffer );
	json.Accept( writer );

	return buffer.GetString();
}


String JsonSerializer::SerializeVec4( Vector4 value )
{
	rapidjson::Value json = _ToJsonVec4( value );

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer( buffer );
	json.Accept( writer );

	return buffer.GetString();
}


Vector2 JsonSerializer::DeserializeVec2( cchar * data )
{
	rapidjson::Document json;
	json.Parse<0>( data );

	if ( json.IsObject() )
	{
		return _FromJsonVec2( json );
	}

	return Vector2();
}


Vector3 JsonSerializer::DeserializeVec3( cchar * data )
{
	rapidjson::Document json;
	json.Parse<0>( data );

	if ( json.IsObject() )
	{
		return _FromJsonVec3( json );
	}

	return Vector3();
}


Vector4 JsonSerializer::DeserializeVec4( cchar * data )
{
	rapidjson::Document json;
	json.Parse<0>( data );

	if ( json.IsObject() )
	{
		return _FromJsonVec4( json );
	}

	return Vector4();
}


rapidjson::Value JsonSerializer::ToJson( const void* object )
{
	// bcuz it needs to return something
	// it will be override
	return rapidjson::Value();
}


rapidjson::Value JsonSerializer::_ToJsonVec2( Vector2 value )
{
	rapidjson::Value json;
	json.SetObject();

	json.AddMember( "x", value.x, _al );
	json.AddMember( "y", value.y, _al );

	return json;
}


rapidjson::Value JsonSerializer::_ToJsonVec3( Vector3 value )
{
	rapidjson::Value json;
	json.SetObject();

	json.AddMember( "x", value.x, _al );
	json.AddMember( "y", value.y, _al );
	json.AddMember( "z", value.z, _al );

	return json;
}


rapidjson::Value JsonSerializer::_ToJsonVec4( Vector4 value )
{
	rapidjson::Value json;
	json.SetObject();

	json.AddMember( "x", value.x, _al );
	json.AddMember( "y", value.y, _al );
	json.AddMember( "z", value.z, _al );
	json.AddMember( "w", value.w, _al );

	return json;
}


Vector2 JsonSerializer::_FromJsonVec2( rapidjson::Value& json )
{
	Vector2 value;
	value.x = json.HasMember( "x" ) ? CAST_S( float, json["x"].GetDouble() ) : 0.0f;	
	value.y = json.HasMember( "y" ) ? CAST_S( float, json["y"].GetDouble() ) : 0.0f;	
	return value;
}


Vector3 JsonSerializer::_FromJsonVec3( rapidjson::Value& json )
{
	Vector3 value;
	value.x = json.HasMember( "x" ) ? CAST_S( float, json["x"].GetDouble() ) : 0.0f;	
	value.y = json.HasMember( "y" ) ? CAST_S( float, json["y"].GetDouble() ) : 0.0f;	
	value.z = json.HasMember( "z" ) ? CAST_S( float, json["z"].GetDouble() ) : 0.0f;	
	return value;
}


Vector4 JsonSerializer::_FromJsonVec4( rapidjson::Value& json )
{
	Vector4 value;
	value.x = json.HasMember( "x" ) ? CAST_S( float, json["x"].GetDouble() ) : 0.0f;	
	value.y = json.HasMember( "y" ) ? CAST_S( float, json["y"].GetDouble() ) : 0.0f;	
	value.z = json.HasMember( "z" ) ? CAST_S( float, json["z"].GetDouble() ) : 0.0f;	
	value.w = json.HasMember( "w" ) ? CAST_S( float, json["w"].GetDouble() ) : 0.0f;	
	return value;
}
