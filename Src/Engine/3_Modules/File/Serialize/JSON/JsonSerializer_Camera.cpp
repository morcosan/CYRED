// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Camera.h"
#include "../../../Render/Components/Camera.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Camera::ToJson( const void* object )
{
	const Camera* camera = CAST_S( const Camera*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ENABLED ),	
					camera->IsEnabled(),	
					_al );
	
	switch ( camera->GetCameraType() )
	{
		case CameraType::PERSPECTIVE:
			json.AddMember( rapidjson::StringRef( CAMERA_TYPE ),
							rapidjson::StringRef( Camera::TYPE_PERSPECTIVE ),
							_al );
			json.AddMember( rapidjson::StringRef( FOVY_ANGLE ), 
							camera->GetFovYAngle(), 
							_al );
			break;

		case CameraType::ORTHOGRAPHIC:
			json.AddMember( rapidjson::StringRef( CAMERA_TYPE ),
							rapidjson::StringRef( Camera::TYPE_ORTHOGRAPHIC ),
							_al );
			json.AddMember( rapidjson::StringRef( ORTHO_HEIGHT ), 
							camera->GetOrthoSize().y, 
							_al );
			break;
	}
	json.AddMember( rapidjson::StringRef( NEAR_CLIP ), 
					camera->GetNearClipping(), 
					_al );
	json.AddMember( rapidjson::StringRef( FAR_CLIP ), 
					camera->GetFarClipping(), 
					_al );

	return json;
}


void JsonSerializer_Camera::FromJson( rapidjson::Value& json, OUT void* object, DeserFlag flag )
{
	Camera* camera = CAST_S( Camera*, object );

	bool emitEvents = camera->DoesEmitEvents();
	camera->SetEmitEvents( FALSE );

	if ( json.HasMember( ENABLED ) )
	{
		camera->SetEnabled( json[ENABLED].GetBool() );
	}
	if ( json.HasMember( CAMERA_TYPE ) )
	{
		FiniteString type( json[CAMERA_TYPE].GetString() );

		if ( type == Camera::TYPE_PERSPECTIVE )
		{
			camera->SetCameraType( CameraType::PERSPECTIVE );
		}
		else if ( type == Camera::TYPE_ORTHOGRAPHIC )
		{
			camera->SetCameraType( CameraType::ORTHOGRAPHIC );
		}
	}
	switch ( camera->GetCameraType() )
	{
		case CameraType::PERSPECTIVE:
		{
			if ( json.HasMember( FOVY_ANGLE ) )
			{
				camera->SetFovYAngle( CAST_S( float, json[FOVY_ANGLE].GetDouble() ) );
			}
			break;
		}

		case CameraType::ORTHOGRAPHIC:
		{
			if ( json.HasMember( ORTHO_HEIGHT ) )
			{
				camera->SetOrthoHeight( CAST_S( float, json[ORTHO_HEIGHT].GetDouble() ) );
			}
			break;
		}
	}
	if ( json.HasMember( NEAR_CLIP ) )
	{
		camera->SetNearClipping( CAST_S( float, json[NEAR_CLIP].GetDouble() ) );
	}
	if ( json.HasMember( FAR_CLIP ) )
	{
		camera->SetFarClipping( CAST_S( float, json[FAR_CLIP].GetDouble() ) );
	}

	camera->SetEmitEvents( emitEvents );
}
