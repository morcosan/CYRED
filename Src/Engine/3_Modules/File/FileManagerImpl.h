// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "FileManager.h"



namespace CYRED
{
	namespace NotAPI
	{
		class FileManagerImpl : public FileManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( FileManagerImpl )


		public:
			void Initialize	( FileSystem* fileSystem )	override;
			void Finalize		()							override;

			Char*	ReadFile	( const Char* filePath, OUT Int& fileSize )		override;
			Char*	ReadFile	( const Char* filePath )						override;
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
			
			void SetSerializeSystem( SerializeSystem* serializeSystem )			override;


		protected:
			FileSystem*			_fileSystem;
		};
	}
}