// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AppSettings.h"


using namespace CYRED;


const Char* const AppSettings::FILE_PATH_CONFIG	= "game.config";

UInt	AppSettings::width		= 800;
UInt	AppSettings::height		= 600;
Int		AppSettings::posX		= 10;
Int		AppSettings::posY		= 30;
UInt	AppSettings::fps		= 60;
Bool	AppSettings::fullscreen = FALSE;

String	AppSettings::appName	= "CYRED_Standalone";

DataArray<AppSettings::SceneData> AppSettings::scenesData;