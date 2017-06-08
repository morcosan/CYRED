// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_MeshRendering.h"
#include "../../../Render/Components/MeshRendering.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"
#include "../../../Render/Assets/Material.h"
#include "../../../Render/Assets/Mesh.h"
#include "../../../Asset/AssetManager.h"
#include "../../../../2_BuildingBlocks/Random/Random.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_MeshRendering::ToJson( const void* object )
{
	const MeshRendering* meshRender = CAST_S( const MeshRendering*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ATTR_ENABLED ),	
					meshRender->IsEnabled(),	
					_al );
	
	{
		Mesh* mesh = meshRender->GetMesh();
		if ( mesh == NULL ) {
			rapidjson::Value nullNode;
			nullNode.SetNull();
			json.AddMember( rapidjson::StringRef( ATTR_MESH ), nullNode, _al );
		}
		else {
			json.AddMember( rapidjson::StringRef( ATTR_MESH ),
							rapidjson::StringRef( mesh->GetUniqueID() ),
							_al );
		}
	}
	{
		Material* material = meshRender->GetMaterial();
		if ( material == NULL )	{
			rapidjson::Value nullNode;
			nullNode.SetNull();
			json.AddMember( rapidjson::StringRef( ATTR_MATERIAL ), nullNode, _al );
		}
		else {
			json.AddMember( rapidjson::StringRef( ATTR_MATERIAL ),
							rapidjson::StringRef( material->GetUniqueID() ),
							_al );
		}
	}

	return json;
}


void JsonSerializer_MeshRendering::FromJson( rapidjson::Value& json, OUT void* object, DeserFlag flag )
{
	MeshRendering* meshRender = CAST_S( MeshRendering*, object );

	bool emitEvents = meshRender->DoesEmitEvents();
	meshRender->SetEmitEvents( FALSE );


	if ( json.HasMember( ATTR_MESH ) ) {
		if ( json[ATTR_MESH].IsNull() )	{
			meshRender->SetMaterial( NULL );
		}
		else {
			cchar* uniqueID = json[ATTR_MESH].GetString();
			Mesh* mesh = AssetManager::Singleton()->GetMesh( uniqueID );
			if ( mesh == NULL ) {
				bool isOk = Random::ValidateUniqueID( uniqueID );
				if ( isOk )	{
					mesh = new Mesh();
					mesh->SetUniqueID( uniqueID );
					AssetManager::Singleton()->AddMesh( mesh );
				}
			}
			meshRender->SetMesh( mesh );
		}
	}

	if ( json.HasMember( ATTR_MATERIAL ) ) {
		if ( json[ATTR_MATERIAL].IsNull() )	{
			meshRender->SetMaterial( NULL );
		}
		else {
			cchar* uniqueID = json[ATTR_MATERIAL].GetString();
			Material* material = AssetManager::Singleton()->GetMaterial( uniqueID );
			if ( material == NULL )	{
				bool isOk = Random::ValidateUniqueID( uniqueID );
				if ( isOk )	{
					material = new Material();
					material->SetUniqueID( uniqueID );
					AssetManager::Singleton()->AddMaterial( material );
				}
			}
			meshRender->SetMaterial( material );
		}
	}


	meshRender->SetEmitEvents( emitEvents );
}
