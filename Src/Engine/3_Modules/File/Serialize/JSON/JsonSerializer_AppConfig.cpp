// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_AppConfig.h"
#include "../../../../4_Application/AppConfig.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;



rapidjson::Value JsonSerializer_AppConfig::ToJson( void* object )
{
	AppConfig* appConfig = CAST_S( AppConfig*, object );

	rapidjson::Value json;
	json.SetObject();

	// add app name
	json.AddMember( rapidjson::StringRef( APP_NAME ),	
					rapidjson::StringRef( appConfig->appName.GetChar() ),
					_al );
	// add fullscreen flag
	json.AddMember( rapidjson::StringRef( FULLSCREEN ),	
					appConfig->fullscreen,	
					_al );
	// add width
	json.AddMember( rapidjson::StringRef( WIDTH ),	
					appConfig->width,	
					_al );
	// add height
	json.AddMember( rapidjson::StringRef( HEIGHT ),	
					appConfig->height,	
					_al );
	// add window pos x
	json.AddMember( rapidjson::StringRef( POS_X ),	
					appConfig->posX,	
					_al );
	// add window pos y
	json.AddMember( rapidjson::StringRef( POS_Y ),	
					appConfig->posY,	
					_al );
	// add fps limit
	json.AddMember( rapidjson::StringRef( FPS ),	
					appConfig->fps,	
					_al );
	// add start scene
	{
		// create object
		rapidjson::Value objectNode;
		objectNode.SetObject();
		// add to object
		objectNode.AddMember( rapidjson::StringRef( ASSET_NAME ),	
							  rapidjson::StringRef( appConfig->startScene.name.GetChar() ),	
							  _al );
		objectNode.AddMember( rapidjson::StringRef( ASSET_PATH ),	
							  rapidjson::StringRef( appConfig->startScene.path.GetChar() ),	
							  _al );
		// add to json
		json.AddMember( rapidjson::StringRef( START_SCENE ),	
						objectNode,	
						_al );
	}
	
	// add all assets
	{
		// create object
		rapidjson::Value assetNode;
		assetNode.SetObject();

		// add textures
		{
			// create list
			rapidjson::Value arrayNode;
			arrayNode.SetArray();
			// add all to list
			for ( UInt i = 0; i < appConfig->assetTextures.Size(); i++ ) {
				// create object
				rapidjson::Value objectNode;
				objectNode.SetObject();
				// add to object
				objectNode.AddMember( rapidjson::StringRef( ASSET_NAME ),	
									  rapidjson::StringRef( appConfig->assetTextures[i].name.GetChar() ),	
									  _al );
				objectNode.AddMember( rapidjson::StringRef( ASSET_PATH ),	
									  rapidjson::StringRef( appConfig->assetTextures[i].path.GetChar() ),	
									  _al );
				// add to list
				arrayNode.PushBack( objectNode, _al );
			}
			// add to assets
			assetNode.AddMember( rapidjson::StringRef( ASSETS_TEXTURES ),	
								 arrayNode,	
								 _al );
		}

		// add scenes
		{
			// create list
			rapidjson::Value arrayNode;
			arrayNode.SetArray();
			// add all to list
			for ( UInt i = 0; i < appConfig->assetScenes.Size(); i++ ) {
				// create object
				rapidjson::Value objectNode;
				objectNode.SetObject();
				// add to object
				objectNode.AddMember( rapidjson::StringRef( ASSET_NAME ),	
									  rapidjson::StringRef( appConfig->assetScenes[i].name.GetChar() ),	
									  _al );
				objectNode.AddMember( rapidjson::StringRef( ASSET_PATH ),	
									  rapidjson::StringRef( appConfig->assetScenes[i].path.GetChar() ),	
									  _al );
				// add to list
				arrayNode.PushBack( objectNode, _al );
			}
			// add to assets
			assetNode.AddMember( rapidjson::StringRef( ASSETS_SCENES ),	
								 arrayNode,	
								 _al );
		}

		// add shaders
		{
			// create list
			rapidjson::Value arrayNode;
			arrayNode.SetArray();
			// add all to list
			for ( UInt i = 0; i < appConfig->assetShaders.Size(); i++ ) {
				// create object
				rapidjson::Value objectNode;
				objectNode.SetObject();
				// add to object
				objectNode.AddMember( rapidjson::StringRef( ASSET_NAME ),	
									  rapidjson::StringRef( appConfig->assetShaders[i].name.GetChar() ),	
									  _al );
				objectNode.AddMember( rapidjson::StringRef( ASSET_PATH ),	
									  rapidjson::StringRef( appConfig->assetShaders[i].path.GetChar() ),	
									  _al );
				// add to list
				arrayNode.PushBack( objectNode, _al );
			}
			// add to assets
			assetNode.AddMember( rapidjson::StringRef( ASSETS_SHADERS ),	
								 arrayNode,	
								 _al );
		}

		// add materials
		{
			// create list
			rapidjson::Value arrayNode;
			arrayNode.SetArray();
			// add all to list
			for ( UInt i = 0; i < appConfig->assetMaterials.Size(); i++ ) {
				// create object
				rapidjson::Value objectNode;
				objectNode.SetObject();
				// add to object
				objectNode.AddMember( rapidjson::StringRef( ASSET_NAME ),	
									  rapidjson::StringRef( appConfig->assetMaterials[i].name.GetChar() ),	
									  _al );
				objectNode.AddMember( rapidjson::StringRef( ASSET_PATH ),	
									  rapidjson::StringRef( appConfig->assetMaterials[i].path.GetChar() ),	
									  _al );
				// add to list
				arrayNode.PushBack( objectNode, _al );
			}
			// add to assets
			assetNode.AddMember( rapidjson::StringRef( ASSETS_MATERIALS ),	
								 arrayNode,	
								 _al );
		}

		// add meshes
		{
			// create list
			rapidjson::Value arrayNode;
			arrayNode.SetArray();
			// add all to list
			for ( UInt i = 0; i < appConfig->assetMeshes.Size(); i++ ) {
				// create object
				rapidjson::Value objectNode;
				objectNode.SetObject();
				// add to object
				objectNode.AddMember( rapidjson::StringRef( ASSET_NAME ),	
									  rapidjson::StringRef( appConfig->assetMeshes[i].name.GetChar() ),	
									  _al );
				objectNode.AddMember( rapidjson::StringRef( ASSET_PATH ),	
									  rapidjson::StringRef( appConfig->assetMeshes[i].path.GetChar() ),	
									  _al );
				// add to list
				arrayNode.PushBack( objectNode, _al );
			}
			// add to assets
			assetNode.AddMember( rapidjson::StringRef( ASSETS_MESHES ),	
								 arrayNode,	
								 _al );
		}

		// add morphs
		{
			// create list
			rapidjson::Value arrayNode;
			arrayNode.SetArray();
			// add all to list
			for ( UInt i = 0; i < appConfig->assetMorphs.Size(); i++ ) {
				// create object
				rapidjson::Value objectNode;
				objectNode.SetObject();
				// add to object
				objectNode.AddMember( rapidjson::StringRef( ASSET_NAME ),	
									  rapidjson::StringRef( appConfig->assetMorphs[i].name.GetChar() ),	
									  _al );
				objectNode.AddMember( rapidjson::StringRef( ASSET_PATH ),	
									  rapidjson::StringRef( appConfig->assetMorphs[i].path.GetChar() ),	
									  _al );
				// add to list
				arrayNode.PushBack( objectNode, _al );
			}
			// add to assets
			assetNode.AddMember( rapidjson::StringRef( ASSETS_MORPHS ),	
								 arrayNode,	
								 _al );
		}	

		// add to json
		json.AddMember( rapidjson::StringRef( ASSETS ),	
						assetNode,	
						_al );
	}
	
	return json;
}


void JsonSerializer_AppConfig::FromJson( rapidjson::Value& json, OUT void* object, 
										 DeserFlag flag )
{
	/*AppConfig* appConfig = CAST_S( AppConfig*, object );

	if ( json.HasMember( APP_NAME ) )
	{
		appConfig->appName = json[APP_NAME].GetString();
	}
	if ( json.HasMember( FULLSCREEN ) )
	{
		appConfig->fullscreen = json[FULLSCREEN].GetBool();
	}
	if ( !appConfig->fullscreen )
	{
		if ( json.HasMember( WIDTH ) )
		{
			appConfig->width = json[WIDTH].GetInt();
		}
		if ( json.HasMember( HEIGHT ) )
		{
			appConfig->height = json[HEIGHT].GetInt();
		}
		if ( json.HasMember( POS_X ) )
		{
			appConfig->posX = json[POS_X].GetInt();
		}
		if ( json.HasMember( POS_Y ) )
		{
			appConfig->posY = json[POS_Y].GetInt();
		}
	}
	if ( json.HasMember( FPS ) )
	{
		appConfig->fps = json[FPS].GetInt();
	}
	if ( json.HasMember( SCENES ) )
	{
		rapidjson::Value& scenes = json[SCENES];

		for ( UInt i = 0; i < scenes.Size(); ++i )
		{
			appConfig->scenes.Add( scenes[i].GetString() );
		}
	}*/
}
