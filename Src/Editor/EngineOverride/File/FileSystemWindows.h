// Copyright (c) 2015 Morco (www.morco.ro)
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
		bool	DeleteDir	( const char* dirPath )							override;
		bool	CreateDir	( const char* parentPath, const char* dirName )	override;
	};
}