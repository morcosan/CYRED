// Copyright (c) 2015 Morco (www.morco.ro)
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
	ABSTRACT class DLL FileSystem
	{
	public:
		FileSystem() {}
		virtual ~FileSystem() {};


	public:
		virtual Char*	ReadFile	( const Char* filePath, OUT Int& fileSize )		PURE_VIRTUAL;
		/* returns TRUE on success */
		virtual Bool	WriteFile	( const Char* filePath, const Char* buffer )	PURE_VIRTUAL;
		/* returns TRUE on success */
		virtual Bool	DeleteFile	( const Char* filePath )						PURE_VIRTUAL;
		/* returns TRUE on success */
		virtual Bool	CopyFile	( const Char* srcPath, const Char* dstPath )	PURE_VIRTUAL;

		virtual UChar*	ReadImage	( const Char* filePath, OUT Int *width, 
									  OUT Int *height, OUT Int *channels )			PURE_VIRTUAL;
		/* returns TRUE on success */
		virtual Bool	WriteImage	( const Char* filePath, const UChar* imageBuffer, 
									  Int width, Int height, Int channels,
									  ImageType type )								PURE_VIRTUAL;
	};
}