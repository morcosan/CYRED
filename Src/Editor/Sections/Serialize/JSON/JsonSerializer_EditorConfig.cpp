// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "JsonSerializer_EditorConfig.h"
#include "../../Settings/EditorSettings.h"

#include "rapidjson\Include\stringbuffer.h"
#include "rapidjson\Include\prettywriter.h"


using namespace CYRED;


rapidjson::Value JsonSerializer_EditorConfig::ToJson( const void* object )
{
	ASSERT( object == NULL );

	rapidjson::Value json;
	json.SetObject();

	json.AddMember( rapidjson::StringRef( FULLSCREEN ), EditorSettings::fullscreen, _al );
	json.AddMember( rapidjson::StringRef( WIDTH ),		EditorSettings::width,		_al );
	json.AddMember( rapidjson::StringRef( HEIGHT ),		EditorSettings::height,		_al );
	json.AddMember( rapidjson::StringRef( POS_X ),		EditorSettings::posX,		_al );
	json.AddMember( rapidjson::StringRef( POS_Y ),		EditorSettings::posY,		_al );
	json.AddMember( rapidjson::StringRef( FPS ),		EditorSettings::fps,		_al );
	json.AddMember( rapidjson::StringRef( PROJECT_NAME ),		
					rapidjson::StringRef( EditorSettings::projectName.GetChar() ),
					_al );
	json.AddMember( rapidjson::StringRef( PROJECT_PATH ),		
					rapidjson::StringRef( EditorSettings::projectPath.GetChar() ),
					_al );
	{
		rapidjson::Value arrayNode;
		arrayNode.SetArray();

		for ( int i = 0; i < EditorSettings::panels.Size(); i++ ) {
			rapidjson::Value objectNode;
			objectNode.SetObject();

			// add panel type
			cchar* panelType = "";
			switch ( EditorSettings::panels[i].type ) {
				case PanelType::ASSETS:				panelType = P_ASSETS;				break;
				case PanelType::ATTRIBUTES:			panelType = P_ATTRIBUTES;			break;
				case PanelType::CONSOLE:			panelType = P_CONSOLE;				break;
				case PanelType::GAME_VIEWPORT:		panelType = P_GAME_VIEWPORT;		break;
				case PanelType::ISOLATE_HIERARCHY:	panelType = P_ISOLATE_HIERARCHY;	break;
				case PanelType::ISOLATE_VIEWPORT:	panelType = P_ISOLATE_VIEWPORT;		break;
				case PanelType::PREFAB_HIERARCHY:	panelType = P_PREFAB_HIERARCHY;		break;
				case PanelType::PREFAB_VIEWPORT:	panelType = P_PREFAB_VIEWPORT;		break;
				case PanelType::SCENE_HIERARCHY:	panelType = P_SCENE_HIERARCHY;		break;
				case PanelType::SCENE_VIEWPORT:		panelType = P_SCENE_VIEWPORT;		break;
			}
			objectNode.AddMember( rapidjson::StringRef( PANELS_TYPE ), 
								  rapidjson::StringRef( panelType ), 
								  _al );

			// add panel size
			if ( EditorSettings::panels[i].width > 0 ) {
				objectNode.AddMember( rapidjson::StringRef( PANELS_WIDTH ),	
									  EditorSettings::panels[i].width,		
									  _al );
			}
			if ( EditorSettings::panels[i].height > 0 ) {
				objectNode.AddMember( rapidjson::StringRef( PANELS_HEIGHT ),	
									  EditorSettings::panels[i].height,		
									  _al );
			}

			// add split from
			if ( i > 0 ) {
				cchar* splitFrom = "";
				switch ( EditorSettings::panels[i].splitFrom ) {
					case PanelType::ASSETS:				splitFrom = P_ASSETS;				break;
					case PanelType::ATTRIBUTES:			splitFrom = P_ATTRIBUTES;			break;
					case PanelType::CONSOLE:			splitFrom = P_CONSOLE;				break;
					case PanelType::GAME_VIEWPORT:		splitFrom = P_GAME_VIEWPORT;		break;
					case PanelType::ISOLATE_HIERARCHY:	splitFrom = P_ISOLATE_HIERARCHY;	break;
					case PanelType::ISOLATE_VIEWPORT:	splitFrom = P_ISOLATE_VIEWPORT;		break;
					case PanelType::PREFAB_HIERARCHY:	splitFrom = P_PREFAB_HIERARCHY;		break;
					case PanelType::PREFAB_VIEWPORT:	splitFrom = P_PREFAB_VIEWPORT;		break;
					case PanelType::SCENE_HIERARCHY:	splitFrom = P_SCENE_HIERARCHY;		break;
					case PanelType::SCENE_VIEWPORT:		splitFrom = P_SCENE_VIEWPORT;		break;
				}
				objectNode.AddMember( rapidjson::StringRef( PANELS_SPLIT_FROM ), 
									  rapidjson::StringRef( splitFrom ), 
									  _al );

				cchar* splitType = "";
				switch ( EditorSettings::panels[i].splitType ) {
					case PanelSplitType::HORIZONTAL:	splitType = SPLIT_HORIZONTAL;	break;
					case PanelSplitType::VERTICAL:		splitType = SPLIT_VERTICAL;		break;
					case PanelSplitType::NEW_TAB:		splitType = SPLIT_NEW_TAB;		break;
				}
				objectNode.AddMember( rapidjson::StringRef( PANELS_SPLIT_TYPE ), 
									  rapidjson::StringRef( splitType ), 
									  _al );
			}

			// add viewport index
			if ( EditorSettings::panels[i].viewportIndex > -1 ) {
				objectNode.AddMember( rapidjson::StringRef( PANELS_VP_INDEX ), 
									  EditorSettings::panels[i].viewportIndex, 
									  _al );
			}

			// add to array
			arrayNode.PushBack( objectNode, _al );
		}

		// add to json
		json.AddMember( rapidjson::StringRef( PANELS ), arrayNode,	_al );
	}

	return json;
}


void JsonSerializer_EditorConfig::FromJson( rapidjson::Value& json, OUT void* object, 
											DeserFlag flag )
{
	ASSERT( object == NULL );
	
	if ( json.HasMember( FULLSCREEN ) ) {
		EditorSettings::fullscreen = json[FULLSCREEN].GetBool();
	}

	if ( json.HasMember( WIDTH ) ) {
		EditorSettings::width = json[WIDTH].GetInt();
	}

	if ( json.HasMember( HEIGHT ) ) {
		EditorSettings::height = json[HEIGHT].GetInt();
	}

	if ( json.HasMember( POS_X ) ) {
		EditorSettings::posX = json[POS_X].GetInt();
	}

	if ( json.HasMember( POS_Y ) ) {
		EditorSettings::posY = json[POS_Y].GetInt();
	}

	if ( json.HasMember( FPS ) ) {
		EditorSettings::fps = json[FPS].GetInt();
	}

	if ( json.HasMember( PROJECT_NAME ) ) {
		EditorSettings::projectName = json[PROJECT_NAME].GetString();
	}

	if ( json.HasMember( PROJECT_PATH ) ) {
		EditorSettings::projectPath = json[PROJECT_PATH].GetString();
	}

	if ( json.HasMember( PANELS ) ) {
		rapidjson::Value& panels = json[PANELS];

		for ( uint i = 0; i < panels.Size(); i++ ) {
			// new panel struct
			EditorSettings::PanelData panel;

			// add type
			String type( panels[i][PANELS_TYPE].GetString() );
			if ( type == P_ASSETS ) {
				panel.type = PanelType::ASSETS;
			}
			else if ( type == P_ATTRIBUTES ) {
				panel.type = PanelType::ATTRIBUTES;
			}
			else if ( type == P_ATTRIBUTES ) {
				panel.type = PanelType::ATTRIBUTES;
			}
			else if ( type == P_CONSOLE ) {
				panel.type = PanelType::CONSOLE;
			}
			else if ( type == P_PREFAB_HIERARCHY ) {
				panel.type = PanelType::PREFAB_HIERARCHY;
			}
			else if ( type == P_PREFAB_VIEWPORT ) {
				panel.type = PanelType::PREFAB_VIEWPORT;
			}
			else if ( type == P_SCENE_HIERARCHY ) {
				panel.type = PanelType::SCENE_HIERARCHY;
			}
			else if ( type == P_SCENE_VIEWPORT ) {
				panel.type = PanelType::SCENE_VIEWPORT;
			}
			else if ( type == P_GAME_VIEWPORT ) {
				panel.type = PanelType::GAME_VIEWPORT;
			}
			else if ( type == P_ISOLATE_HIERARCHY ) {
				panel.type = PanelType::ISOLATE_HIERARCHY;
			}
			else if ( type == P_ISOLATE_VIEWPORT ) {
				panel.type = PanelType::ISOLATE_VIEWPORT;
			}

			// add width and height
			if ( panels[i].HasMember( PANELS_WIDTH ) ) {
				panel.width = panels[i][PANELS_WIDTH].GetInt();
			}
			else {
				panel.width = -1;
			}

			if ( panels[i].HasMember( PANELS_HEIGHT ) ) {
				panel.height = panels[i][PANELS_HEIGHT].GetInt();
			}
			else {
				panel.height = -1;
			}

			// add split from
			if ( panels[i].HasMember( PANELS_SPLIT_FROM ) ) {
				String splitFrom( panels[i][PANELS_SPLIT_FROM].GetString() );
				if ( splitFrom == P_ASSETS ) {
					panel.splitFrom = PanelType::ASSETS;
				}
				else if ( splitFrom == P_ATTRIBUTES ) {
					panel.splitFrom = PanelType::ATTRIBUTES;
				}
				else if ( splitFrom == P_ATTRIBUTES ) {
					panel.splitFrom = PanelType::ATTRIBUTES;
				}
				else if ( splitFrom == P_CONSOLE ) {
					panel.splitFrom = PanelType::CONSOLE;
				}
				else if ( splitFrom == P_PREFAB_HIERARCHY ) {
					panel.splitFrom = PanelType::PREFAB_HIERARCHY;
				}
				else if ( splitFrom == P_PREFAB_VIEWPORT ) {
					panel.splitFrom = PanelType::PREFAB_VIEWPORT;
				}
				else if ( splitFrom == P_SCENE_HIERARCHY ) {
					panel.splitFrom = PanelType::SCENE_HIERARCHY;
				}
				else if ( splitFrom == P_SCENE_VIEWPORT ) {
					panel.splitFrom = PanelType::SCENE_VIEWPORT;
				}
				else if ( splitFrom == P_ISOLATE_HIERARCHY ) {
					panel.splitFrom = PanelType::ISOLATE_HIERARCHY;
				}
				else if ( splitFrom == P_ISOLATE_VIEWPORT ) {
					panel.splitFrom = PanelType::ISOLATE_VIEWPORT;
				}
			}

			// add split type
			if ( panels[i].HasMember( PANELS_SPLIT_TYPE ) ) {
				String splitType( panels[i][PANELS_SPLIT_TYPE].GetString() );
				if ( splitType == SPLIT_HORIZONTAL ) {
					panel.splitType = PanelSplitType::HORIZONTAL;
				}
				else if ( splitType == SPLIT_VERTICAL ) {
					panel.splitType = PanelSplitType::VERTICAL;
				}
				else if ( splitType == SPLIT_NEW_TAB ) {
					panel.splitType = PanelSplitType::NEW_TAB;
				}
			}

			// add viewport index
			if ( panels[i].HasMember( PANELS_VP_INDEX ) ) {
				panel.viewportIndex = panels[i][PANELS_VP_INDEX].GetInt();
			}
			else {
				panel.viewportIndex = -1;
			}

			// add to list
			EditorSettings::panels.Add( panel );
		}
	}
}
