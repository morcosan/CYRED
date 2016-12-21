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
		Bool	DeleteDir	( const Char* dirPath )							override;
		Bool	CreateDir	( const Char* parentPath, const Char* dirName )	override;
	};
}