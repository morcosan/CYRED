// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_SceneDB.h"

#include "../../../Render/Assets/Material.h"
#include "../../../Render/Assets/Shader.h"
#include "../../../Render/Assets/Texture.h"
#include "../../../Scene/Fragments/SceneDB.h"
#include "../../../Scene/Fragments/Scene.h"
#include "../../../Asset/AssetManager.h"
#include "../../../File/FileManager.h"
#include "../../../../2_BuildingBlocks/String/FiniteString.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_SceneDB::ToJson( void* object )
{
	SceneDB* sceneDB = CAST_S( SceneDB*, object );

	rapidjson::Value json;
	json.SetObject();


	// load scene data as json
	FiniteString filePath( "%s%s%s", sceneDB->scene->GetDirPath(),
									 sceneDB->scene->GetName(),
									 FileManager::FILE_FORMAT_SCENE );
	rapidjson::Document sceneJson( &_al );
	sceneJson.Parse<0>( FileManager::Singleton()->ReadFile( filePath.GetChar() ) );

	// serialize fulls scene
	json.AddMember( rapidjson::StringRef( SCENE ), 
					sceneJson, 
					_al );

	// add assets from scene
	json.AddMember( rapidjson::StringRef( ASSETS ), 
					"", 
					_al );

	return json;
}


void JsonSerializer_SceneDB::FromJson( rapidjson::Value& json, OUT void* object, 
									   DeserFlag flag )
{
	/*if ( json.HasMember( ASSETS ) )
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
	}*/
}
