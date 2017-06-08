// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Mesh.h"
#include "../../../Render/Assets/Mesh.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Mesh::ToJson( const void* object )
{
	const Mesh* mesh = CAST_S( const Mesh*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ATTR_UNIQUE_ID ),
					rapidjson::StringRef( mesh->GetUniqueID() ),
					_al );
	
	switch ( mesh->GetMeshType() )
	{
		case MeshType::LINE:
			json.AddMember( rapidjson::StringRef( ATTR_MESH_TYPE ),
							rapidjson::StringRef( MESH_TYPE_LINE ),
							_al );
			break;

		case MeshType::POLYGON:
			json.AddMember( rapidjson::StringRef( ATTR_MESH_TYPE ),
							rapidjson::StringRef( MESH_TYPE_POLY ),
							_al );
			break;
	}

	switch ( mesh->GetLoadType() )
	{
		case MeshLoadType::EXTERNAL:
			json.AddMember( rapidjson::StringRef( ATTR_LOAD_TYPE ),
							rapidjson::StringRef( LOAD_TYPE_EXTERNAL ),
							_al );
			break;

		case MeshLoadType::SCRIPTED:
			json.AddMember( rapidjson::StringRef( ATTR_LOAD_TYPE ),
							rapidjson::StringRef( LOAD_TYPE_SCRIPTED ),
							_al );
			break;
	}

	switch ( mesh->GetLoadType() )
	{
		case MeshLoadType::EXTERNAL:
		case MeshLoadType::SCRIPTED:
			json.AddMember( rapidjson::StringRef( ATTR_FILE_PATH ),
							rapidjson::StringRef( mesh->GetExternalPath() ),
							_al );
			break;
	}

	json.AddMember( rapidjson::StringRef( ATTR_CLEAR_BUFFER ),
					mesh->DoesClearBuffersOnBind(),
					_al );

	return json;
}


void JsonSerializer_Mesh::FromJson( rapidjson::Value& json, OUT void* object,
									DeserFlag flag )
{
	Mesh* mesh = CAST_S( Mesh*, object );

	bool emitEvents = mesh->DoesEmitEvents();
	mesh->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_UNIQUE_ID ) ) {
		mesh->SetUniqueID( json[ATTR_UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY ) {
		return;
	}

	if ( json.HasMember( ATTR_MESH_TYPE ) ) {
		FiniteString type( json[ATTR_MESH_TYPE].GetString() );

		if ( type == MESH_TYPE_LINE ) {
			mesh->SetMeshType( MeshType::LINE );
		}
		if ( type == MESH_TYPE_POLY ) {
			mesh->SetMeshType( MeshType::POLYGON );
		}
	}

	if ( json.HasMember( ATTR_LOAD_TYPE ) ) {
		FiniteString type( json[ATTR_LOAD_TYPE].GetString() );

		if ( type == LOAD_TYPE_EXTERNAL ) {
			mesh->SetLoadType( MeshLoadType::EXTERNAL );
		}
		else if ( type == LOAD_TYPE_SCRIPTED ) {
			mesh->SetLoadType( MeshLoadType::SCRIPTED );
		}
	}

	if ( json.HasMember( ATTR_FILE_PATH ) ) {
		mesh->SetExternalPath( json[ATTR_FILE_PATH].GetString() );
	}

	if ( json.HasMember( ATTR_CLEAR_BUFFER ) ) {
		mesh->SetClearBuffersOnBind( json[ATTR_CLEAR_BUFFER].GetBool() );
	}


	mesh->SetEmitEvents( emitEvents );
}
