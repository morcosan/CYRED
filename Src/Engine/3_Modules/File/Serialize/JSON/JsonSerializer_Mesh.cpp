// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_Mesh.h"
#include "../../../Render/Assets/Mesh.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_Mesh::ToJson( void* object )
{
	Mesh* mesh = CAST_S( Mesh*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( UNIQUE_ID ),
					rapidjson::StringRef( mesh->GetUniqueID() ),
					_al );
	
	switch ( mesh->GetMeshType() )
	{
		case MeshType::LINE:
			json.AddMember( rapidjson::StringRef( MESH_TYPE ),
							rapidjson::StringRef( MESH_TYPE_LINE ),
							_al );
			break;

		case MeshType::POLYGON:
			json.AddMember( rapidjson::StringRef( MESH_TYPE ),
							rapidjson::StringRef( MESH_TYPE_POLY ),
							_al );
			break;
	}

	switch ( mesh->GetLoadType() )
	{
		case MeshLoadType::GEN_CUBE:
			json.AddMember( rapidjson::StringRef( LOAD_TYPE ),
							rapidjson::StringRef( LOAD_TYPE_CUBE ),
							_al );
			break;

		case MeshLoadType::GEN_QUAD:
			json.AddMember( rapidjson::StringRef( LOAD_TYPE ),
							rapidjson::StringRef( LOAD_TYPE_QUAD ),
							_al );
			break;

		case MeshLoadType::EXTERNAL:
			json.AddMember( rapidjson::StringRef( LOAD_TYPE ),
							rapidjson::StringRef( LOAD_TYPE_EXT ),
							_al );
			break;
	}

	switch ( mesh->GetLoadType() )
	{
		case MeshLoadType::EXTERNAL:
			json.AddMember( rapidjson::StringRef( EXT_FILE_PATH ),
							rapidjson::StringRef( mesh->GetExternalPath() ),
							_al );
			break;
	}

	json.AddMember( rapidjson::StringRef( CLEAR_BUFFER ),
					mesh->DoesClearBuffersOnBind(),
					_al );

	return json;
}


void JsonSerializer_Mesh::FromJson( rapidjson::Value& json, OUT void* object,
									DeserFlag flag )
{
	Mesh* mesh = CAST_S( Mesh*, object );

	Bool emitEvents = mesh->DoesEmitEvents();
	mesh->SetEmitEvents( FALSE );

	if ( json.HasMember( UNIQUE_ID ) )
	{
		mesh->SetUniqueID( json[UNIQUE_ID].GetString() );
	}

	if ( flag == DeserFlag::UID_ONLY )
	{
		return;
	}

	if ( json.HasMember( MESH_TYPE ) )
	{
		FiniteString type( json[MESH_TYPE].GetString() );

		if ( type == MESH_TYPE_LINE )
		{
			mesh->SetMeshType( MeshType::LINE );
		}
		if ( type == MESH_TYPE_POLY )
		{
			mesh->SetMeshType( MeshType::POLYGON );
		}
	}

	if ( json.HasMember( LOAD_TYPE ) ) {
		FiniteString type( json[LOAD_TYPE].GetString() );

		if ( type == LOAD_TYPE_EXT ) {
			mesh->SetLoadType( MeshLoadType::EXTERNAL );
		}
		else if ( type == LOAD_TYPE_CUBE ) {
			mesh->SetLoadType( MeshLoadType::GEN_CUBE );
		}
		else if ( type == LOAD_TYPE_QUAD ) {
			mesh->SetLoadType( MeshLoadType::GEN_QUAD );
		}
	}

	if ( json.HasMember( EXT_FILE_PATH ) )
	{
		mesh->SetExternalPath( json[EXT_FILE_PATH].GetString() );
	}

	if ( json.HasMember( CLEAR_BUFFER ) )
	{
		mesh->SetClearBuffersOnBind( json[CLEAR_BUFFER].GetBool() );
	}

	mesh->SetEmitEvents( emitEvents );
}
