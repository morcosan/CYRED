// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_AssetDB.h"
#include "../../../Render/Assets/Material.h"
#include "../../../Render/Assets/Shader.h"
#include "../../../Render/Assets/Texture.h"
#include "../../../Asset/AssetManager.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_AssetDB::ToJson( void* object )
{
	ASSERT( object == NULL );

	rapidjson::Value json;
	json.SetObject();

	//json.AddMember( rapidjson::StringRef( ENABLED ),	
	//				transform->IsEnabled(),	
	//				_al );
	//
	//json.AddMember( rapidjson::StringRef( POSITION_WORLD ), 
	//				_ToJsonVec3( transform->GetPositionWorld() ), 
	//				_al );
	//json.AddMember( rapidjson::StringRef( ROTATIO_WORLD ), 
	//				_ToJsonVec3( transform->GetEulerRotationWorld() ), 
	//				_al );
	//json.AddMember( rapidjson::StringRef( SCALE_WORLD ), 
	//				_ToJsonVec3( transform->GetScaleWorld() ), 
	//				_al );

	return json;
}


void JsonSerializer_AssetDB::FromJson( rapidjson::Value& json, OUT void* object, 
									   DeserFlag flag )
{
	ASSERT( object == NULL );

	if ( json.HasMember( ASSETS ) )
	{
		rapidjson::Value& assets = json[ASSETS];

		for ( UInt i = 0; i < assets.Size(); ++i )
		{
			if ( assets[i].HasMember( ASSET_NAME ) &&
				 assets[i].HasMember( ASSET_TYPE ) && 
				 assets[i].HasMember( ASSET_DIR ) )
			{
				String temp( assets[i][ASSET_TYPE].GetString() );

				const Char* assetName = assets[i][ASSET_NAME].GetString();
				const Char* assetDir = assets[i][ASSET_DIR].GetString();
				StatusAssetAdd status;
				Asset* asset = NULL;

				if ( temp == TYPE_MATERIAL )
				{
					Material* material = Memory::Alloc<Material>();
					material->SetEmitEvents( FALSE );
					material->SetName( assetName );
					material->SetDirPath( assetDir );
					material->LoadUniqueID();
					material->SetEmitEvents( TRUE );

					status = AssetManager::Singleton()->AddMaterial( material );
					asset = material;
				}
				else if ( temp == TYPE_SHADER )
				{
					Shader* shader = Memory::Alloc<Shader>();
					shader->SetEmitEvents( FALSE );
					shader->SetName( assetName );
					shader->SetDirPath( assetDir );
					shader->LoadUniqueID();
					shader->SetEmitEvents( TRUE );

					status = AssetManager::Singleton()->AddShader( shader );
					asset = shader;
				}
				else if ( temp == TYPE_TEXTURE )
				{
					Texture* texture = Memory::Alloc<Texture>();
					texture->SetEmitEvents( FALSE );
					texture->SetName( assetName );
					texture->SetDirPath( assetDir );
					texture->LoadUniqueID();
					texture->SetEmitEvents( TRUE );

					status = AssetManager::Singleton()->AddTexture( texture );
					asset = texture;
				}

				if ( status != StatusAssetAdd::SUCCESS )
				{
					Memory::Free( asset );
				}
			}
		}
	}
}
