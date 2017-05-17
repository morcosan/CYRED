// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "EditorSettings.h"


using namespace CYRED;


const char* const EditorSettings::EDITOR_NAME				= "CYRED-SceneEditor";
	
const char* const EditorSettings::FILE_PATH_CONFIG			= "Editor/editor.config";
	
const char* const EditorSettings::DIR_PATH_SKINS			= "Editor/Skins/";
const char* const EditorSettings::DIR_PATH_ICONS_ASSETS		= "Editor/Icons/Assets/";
const char* const EditorSettings::DIR_PATH_ICONS_LAYOUT		= "Editor/Icons/Layout/";

const char* const EditorSettings::DIR_DEFALUT_ASSETS		= "Editor Assets";
const char* const EditorSettings::DIR_PATH_DEFALUT_ASSETS	= "DefaultAssets/";

const char* const EditorSettings::MSG_EDITOR_STARTED		= "Editor started.";
	
const char* const EditorSettings::SKIN_DEFAULT_BLACK		= "Default-Black";
const char* const EditorSettings::SKIN_DEFAULT_WHITE		= "Default-White";
	
const char* const EditorSettings::FILE_FORMAT_SKINS			= ".css";


bool	EditorSettings::fullscreen	= false;
int	EditorSettings::width		= 800;
int	EditorSettings::height		= 600;
int		EditorSettings::posX		= 0;
int		EditorSettings::posY		= 0;
int	EditorSettings::fps			= 60;

String EditorSettings::projectName;
String EditorSettings::projectPath;

DataArray<EditorSettings::PanelData> EditorSettings::panels;