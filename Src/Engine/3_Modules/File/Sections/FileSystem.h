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
		virtual char*	ReadFile	( cchar* filePath, OUT int& fileSize );
		/* returns TRUE on success */
		virtual bool	WriteFile	( cchar* filePath, cchar* buffer );
		/* returns TRUE on success */
		virtual bool	DeleteFile	( cchar* filePath );
		/* returns TRUE on success */
		virtual bool	CopyFile	( cchar* srcPath, cchar* dstPath );

		/* returns TRUE on success */
		virtual bool	DeleteDir	( cchar* dirPath );
		/* returns TRUE on success */
		virtual bool	CreateDir	( cchar* parentPath, cchar* dirName );

		virtual uchar*	ReadImage	( cchar* filePath, OUT int *width, 
									  OUT int *height, OUT int *channels );
		/* returns TRUE on success */
		virtual bool	WriteImage	( cchar* filePath, const uchar* imageBuffer, 
									  int width, int height, int channels,
									  ImageType type );
	};
}