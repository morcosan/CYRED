// Copyright (c) 2015 Morco (www.morco.ro)
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



rapidjson::Value JsonSerializer_MeshRendering::ToJson( void* object )
{
	MeshRendering* meshRender = CAST_S( MeshRendering*, object );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( ENABLED ),	
					meshRender->IsEnabled(),	
					_al );
	
	{
		Mesh* mesh = meshRender->GetMesh();
		if ( mesh == NULL )
		{
			rapidjson::Value nullNode;
			nullNode.SetNull();
			json.AddMember( rapidjson::StringRef( MESH ), nullNode, _al );
		}
		else
		{
			json.AddMember( rapidjson::StringRef( MESH ),
							rapidjson::StringRef( mesh->GetUniqueID() ),
							_al );
		}
	}
	{
		Material* material = meshRender->GetMaterial();
		if ( material == NULL )
		{
			rapidjson::Value nullNode;
			nullNode.SetNull();
			json.AddMember( rapidjson::StringRef( MATERIAL ), nullNode, _al );
		}
		else
		{
			json.AddMember( rapidjson::StringRef( MATERIAL ),
							rapidjson::StringRef( material->GetUniqueID() ),
							_al );
		}
	}

	return json;
}


void JsonSerializer_MeshRendering::FromJson( rapidjson::Value& json, OUT void* object, 
										 DeserFlag flag )
{
	MeshRendering* meshRender = CAST_S( MeshRendering*, object );

	Bool emitEvents = meshRender->DoesEmitEvents();
	meshRender->SetEmitEvents( FALSE );


	if ( json.HasMember( MESH ) )
	{
		if ( json[MESH].IsNull() )
		{
			meshRender->SetMaterial( NULL );
		}
		else
		{
			const Char* uniqueID = json[MESH].GetString();
			Mesh* mesh = AssetManager::Singleton()->GetMesh( uniqueID );
			if ( mesh == NULL )
			{
				Bool isOk = Random::ValidateUniqueID( uniqueID );
				if ( isOk )
				{
					mesh = Memory::Alloc<Mesh>();
					mesh->SetUniqueID( uniqueID );
					AssetManager::Singleton()->AddMesh( mesh );
				}
			}
			meshRender->SetMesh( mesh );
		}
	}

	if ( json.HasMember( MATERIAL ) )
	{
		if ( json[MATERIAL].IsNull() )
		{
			meshRender->SetMaterial( NULL );
		}
		else
		{
			const Char* uniqueID = json[MATERIAL].GetString();
			Material* material = AssetManager::Singleton()->GetMaterial( uniqueID );
			if ( material == NULL )
			{
				Bool isOk = Random::ValidateUniqueID( uniqueID );
				if ( isOk )
				{
					material = Memory::Alloc<Material>();
					material->SetUniqueID( uniqueID );
					AssetManager::Singleton()->AddMaterial( material );
				}
			}
			meshRender->SetMaterial( material );
		}
	}


	meshRender->SetEmitEvents( emitEvents );
}
