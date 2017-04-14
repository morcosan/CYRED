// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "EditorSettings.h"


using namespace CYRED;


const Char* const EditorSettings::EDITOR_NAME				= "CYRED-SceneEditor";
	
const Char* const EditorSettings::FILE_PATH_CONFIG			= "Editor/editor.config";
	
const Char* const EditorSettings::DIR_PATH_SKINS			= "Editor/Skins/";
const Char* const EditorSettings::DIR_PATH_ICONS_ASSETS		= "Editor/Icons/Assets/";
const Char* const EditorSettings::DIR_PATH_ICONS_LAYOUT		= "Editor/Icons/Layout/";

const Char* const EditorSettings::DIR_DEFALUT_ASSETS		= "Editor Assets";
const Char* const EditorSettings::DIR_PATH_DEFALUT_ASSETS	= "DefaultAssets/";

const Char* const EditorSettings::MSG_EDITOR_STARTED		= "Editor started.";
	
const Char* const EditorSettings::SKIN_DEFAULT_BLACK		= "Default-Black";
const Char* const EditorSettings::SKIN_DEFAULT_WHITE		= "Default-White";
	
const Char* const EditorSettings::FILE_FORMAT_SKINS			= ".css";


UInt	EditorSettings::width	= 800;
UInt	EditorSettings::height	= 600;
Int		EditorSettings::posX	= 0;
Int		EditorSettings::posY	= 0;
UInt	EditorSettings::fps		= 60;

String EditorSettings::projectName;
String EditorSettings::projectPath;
