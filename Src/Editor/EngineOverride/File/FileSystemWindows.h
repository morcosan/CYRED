// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredModule_File.h"


namespace CYRED
{
	class FileSystemWindows: public FileSystem
	{
	public:
		FileSystemWindows() {}
		virtual ~FileSystemWindows() {};


	public:
		bool	DeleteDir	( cchar* dirPath )							override;
		bool	CreateDir	( cchar* parentPath, cchar* dirName )	override;
	};
}