// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"


namespace CYRED
{
	namespace Enum_ImageType
	{
		enum Enum
		{
			PNG
			, BMP
		};
	}
	typedef Enum_ImageType::Enum	ImageType;
}


namespace CYRED
{
	class DLL FileSystem
	{
	public:
		FileSystem() {}
		virtual ~FileSystem() {};


	public:
		virtual char*	ReadFile	( const char* filePath, OUT int& fileSize );
		/* returns TRUE on success */
		virtual bool	WriteFile	( const char* filePath, const char* buffer );
		/* returns TRUE on success */
		virtual bool	DeleteFile	( const char* filePath );
		/* returns TRUE on success */
		virtual bool	CopyFile	( const char* srcPath, const char* dstPath );

		/* returns TRUE on success */
		virtual bool	DeleteDir	( const char* dirPath );
		/* returns TRUE on success */
		virtual bool	CreateDir	( const char* parentPath, const char* dirName );

		virtual uchar*	ReadImage	( const char* filePath, OUT int *width, 
									  OUT int *height, OUT int *channels );
		/* returns TRUE on success */
		virtual bool	WriteImage	( const char* filePath, const uchar* imageBuffer, 
									  int width, int height, int channels,
									  ImageType type );
	};
}