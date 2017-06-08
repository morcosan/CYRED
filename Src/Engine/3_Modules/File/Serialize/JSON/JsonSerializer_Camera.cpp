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

	json.AddMember( rapidjson::StringRef( ATTR_ENABLED ),	
					camera->IsEnabled(),	
					_al );
	
	switch ( camera->GetCameraType() ) {
		case CameraType::PERSPECTIVE:
			json.AddMember( rapidjson::StringRef( ATTR_CAMERA_TYPE ),
							rapidjson::StringRef( Camera::TYPE_PERSPECTIVE ),
							_al );
			json.AddMember( rapidjson::StringRef( ATTR_FOVY_ANGLE ), 
							camera->GetFovYAngle(), 
							_al );
			break;

		case CameraType::ORTHOGRAPHIC:
			json.AddMember( rapidjson::StringRef( ATTR_CAMERA_TYPE ),
							rapidjson::StringRef( Camera::TYPE_ORTHOGRAPHIC ),
							_al );
			json.AddMember( rapidjson::StringRef( ATTR_ORTHO_HEIGHT ), 
							camera->GetOrthoSize().y, 
							_al );
			break;
	}
	json.AddMember( rapidjson::StringRef( ATTR_NEAR_CLIP ), 
					camera->GetNearClipping(), 
					_al );
	json.AddMember( rapidjson::StringRef( ATTR_FAR_CLIP ), 
					camera->GetFarClipping(), 
					_al );

	return json;
}


void JsonSerializer_Camera::FromJson( rapidjson::Value& json, OUT void* object, DeserFlag flag )
{
	Camera* camera = CAST_S( Camera*, object );

	bool emitEvents = camera->DoesEmitEvents();
	camera->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_ENABLED ) ) {
		camera->SetEnabled( json[ATTR_ENABLED].GetBool() );
	}

	if ( json.HasMember( ATTR_CAMERA_TYPE ) ) {
		FiniteString type( json[ATTR_CAMERA_TYPE].GetString() );

		if ( type == Camera::TYPE_PERSPECTIVE ) {
			camera->SetCameraType( CameraType::PERSPECTIVE );
		}
		else if ( type == Camera::TYPE_ORTHOGRAPHIC ) {
			camera->SetCameraType( CameraType::ORTHOGRAPHIC );
		}
	}

	switch ( camera->GetCameraType() ) {
		case CameraType::PERSPECTIVE:
			if ( json.HasMember( ATTR_FOVY_ANGLE ) ) {
				camera->SetFovYAngle( CAST_S( float, json[ATTR_FOVY_ANGLE].GetDouble() ) );
			}
			break;

		case CameraType::ORTHOGRAPHIC:
			if ( json.HasMember( ATTR_ORTHO_HEIGHT ) ) {
				camera->SetOrthoHeight( CAST_S( float, json[ATTR_ORTHO_HEIGHT].GetDouble() ) );
			}
			break;
	}

	if ( json.HasMember( ATTR_NEAR_CLIP ) ) {
		camera->SetNearClipping( CAST_S( float, json[ATTR_NEAR_CLIP].GetDouble() ) );
	}

	if ( json.HasMember( ATTR_FAR_CLIP ) ) {
		camera->SetFarClipping( CAST_S( float, json[ATTR_FAR_CLIP].GetDouble() ) );
	}


	camera->SetEmitEvents( emitEvents );
}
