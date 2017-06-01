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
