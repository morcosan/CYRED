// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "EditorSettings.h"


using namespace CYRED;


cchar* const EditorSettings::EDITOR_NAME				= "CYRED-SceneEditor";
	
cchar* const EditorSettings::FILE_PATH_CONFIG			= "Editor/editor.config";
	
cchar* const EditorSettings::DIR_PATH_SKINS				= "Editor/Skins/";
cchar* const EditorSettings::DIR_PATH_ICONS				= "Editor/Icons/";

cchar* const EditorSettings::DIR_DEFALUT_ASSETS			= "Editor Assets";
cchar* const EditorSettings::DIR_PATH_DEFALUT_ASSETS	= "DefaultAssets/";

cchar* const EditorSettings::MSG_EDITOR_STARTED			= "Editor started.";
	
cchar* const EditorSettings::SKIN_DEFAULT_BLACK			= "Default-Black";
cchar* const EditorSettings::SKIN_DEFAULT_WHITE			= "Default-White";
	
cchar* const EditorSettings::FILE_FORMAT_SKINS			= ".css";


bool	EditorSettings::fullscreen	= false;
int		EditorSettings::width		= 1;
int		EditorSettings::height		= 1;
int		EditorSettings::posX		= 0;
int		EditorSettings::posY		= 0;
int		EditorSettings::fps			= 60;

String EditorSettings::projectName;
String EditorSettings::projectPath;

DataArray<EditorSettings::PanelData> EditorSettings::panels;