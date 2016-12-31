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

		// add scripts
		{
			// create list
			rapidjson::Value arrayNode;
			arrayNode.SetArray();
			// add all to list
			for ( UInt i = 0; i < appConfig->assetScripts.Size(); i++ ) {
				// create object
				rapidjson::Value objectNode;
				objectNode.SetObject();
				// add to object
				objectNode.AddMember( rapidjson::StringRef( ASSET_NAME ),	
									  rapidjson::StringRef( appConfig->assetScripts[i].name.GetChar() ),	
									  _al );
				objectNode.AddMember( rapidjson::StringRef( ASSET_PATH ),	
									  rapidjson::StringRef( appConfig->assetScripts[i].path.GetChar() ),	
									  _al );
				// add to list
				arrayNode.PushBack( objectNode, _al );
			}
			// add to assets
			assetNode.AddMember( rapidjson::StringRef( ASSETS_SCRIPTS ),	
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
	AppConfig* appConfig = CAST_S( AppConfig*, object );

	// load settings
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

	// load start scene
	if ( json.HasMember( START_SCENE ) ) {
		appConfig->startScene = AppConfig::AssetConfig {
			json[START_SCENE][ASSET_NAME].GetString(),
			json[START_SCENE][ASSET_PATH].GetString()
		};
	}

	// load assets
	if ( json.HasMember( ASSETS ) ) {
		// load materials
		if ( json[ASSETS].HasMember( ASSETS_MATERIALS ) ) {
			rapidjson::Value& assets = json[ASSETS][ASSETS_MATERIALS];
			for ( UInt i = 0; i < assets.Size(); i++ ) {
				appConfig->assetMaterials.Add( AppConfig::AssetConfig {
					assets[i][ASSET_NAME].GetString(),
					assets[i][ASSET_PATH].GetString()
				});
			}
		}
		// load textures
		if ( json[ASSETS].HasMember( ASSETS_TEXTURES ) ) {
			rapidjson::Value& assets = json[ASSETS][ASSETS_TEXTURES];
			for ( UInt i = 0; i < assets.Size(); i++ ) {
				appConfig->assetTextures.Add( AppConfig::AssetConfig {
					assets[i][ASSET_NAME].GetString(),
					assets[i][ASSET_PATH].GetString()
				});
			}
		}
		// load meshes
		if ( json[ASSETS].HasMember( ASSETS_MESHES ) ) {
			rapidjson::Value& assets = json[ASSETS][ASSETS_MESHES];
			for ( UInt i = 0; i < assets.Size(); i++ ) {
				appConfig->assetMeshes.Add( AppConfig::AssetConfig {
					assets[i][ASSET_NAME].GetString(),
					assets[i][ASSET_PATH].GetString()
				});
			}
		}
		// load morphs
		if ( json[ASSETS].HasMember( ASSETS_MORPHS ) ) {
			rapidjson::Value& assets = json[ASSETS][ASSETS_MORPHS];
			for ( UInt i = 0; i < assets.Size(); i++ ) {
				appConfig->assetMorphs.Add( AppConfig::AssetConfig {
					assets[i][ASSET_NAME].GetString(),
					assets[i][ASSET_PATH].GetString()
				});
			}
		}
		// load scenes
		if ( json[ASSETS].HasMember( ASSETS_SCENES ) ) {
			rapidjson::Value& assets = json[ASSETS][ASSETS_SCENES];
			for ( UInt i = 0; i < assets.Size(); i++ ) {
				appConfig->assetScenes.Add( AppConfig::AssetConfig {
					assets[i][ASSET_NAME].GetString(),
					assets[i][ASSET_PATH].GetString()
				});
			}
		}
		// load shaders
		if ( json[ASSETS].HasMember( ASSETS_SHADERS ) ) {
			rapidjson::Value& assets = json[ASSETS][ASSETS_SHADERS];
			for ( UInt i = 0; i < assets.Size(); i++ ) {
				appConfig->assetShaders.Add( AppConfig::AssetConfig {
					assets[i][ASSET_NAME].GetString(),
					assets[i][ASSET_PATH].GetString()
				});
			}
		}
		// load scripts
		if ( json[ASSETS].HasMember( ASSETS_SCRIPTS ) ) {
			rapidjson::Value& assets = json[ASSETS][ASSETS_SCRIPTS];
			for ( UInt i = 0; i < assets.Size(); i++ ) {
				appConfig->assetScripts.Add( AppConfig::AssetConfig {
					assets[i][ASSET_NAME].GetString(),
					assets[i][ASSET_PATH].GetString()
				});
			}
		}
	}
}
