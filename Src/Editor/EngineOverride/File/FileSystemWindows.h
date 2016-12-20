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
		Char*	ReadFile	( const Char* filePath, OUT Int& fileSize )		override;
		Bool	WriteFile	( const Char* filePath, const Char* buffer )	override;
		Bool	DeleteFile	( const Char* filePath )						override;
		Bool	CopyFile	( const Char* srcPath, const Char* dstPath )	override;

		Bool	DeleteDir	( const Char* dirPath )							override;
		Bool	CreateDir	( const Char* parentPath, const Char* dirName )	override;

		UChar*	ReadImage	( const Char* filePath, OUT Int *width, 
							  OUT Int *height, OUT Int *channels )			override;
		Bool	WriteImage	( const Char* filePath, const UChar* imageBuffer, 
							  Int width, Int height, Int channels,
							  ImageType type )								override;
	};
}