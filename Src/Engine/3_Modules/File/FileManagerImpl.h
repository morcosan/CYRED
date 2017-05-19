// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
			void Initialize		( FileSystem* fileSystem, MeshLoader* meshLoader )	override;
			void Finalize		() override;

			char*	ReadFile	( const char* filePath, OUT int& fileSize )		override;
			char*	ReadFile	( const char* filePath )						override;
			bool	WriteFile	( const char* filePath, const char* buffer )	override;
			bool	DeleteFile	( const char* filePath )						override;
			bool	CopyFile	( const char* srcPath, const char* dstPath )	override;
			
			bool	DeleteDir	( const char* dirPath )							override;
			bool	CreateDir	( const char* parentPath, const char* dirName )	override;

			uchar*	ReadImage	( const char* filePath, OUT int *width, 
								  OUT int *height, OUT int *channels )			override;
			bool	WriteImage	( const char* filePath, const uchar* imageBuffer, 
								  int width, int height, int channels,
								  ImageType type )								override;
			
			bool	ImportMesh	( const char* data, int dataSize, 
								  OUT DataArray<Vertex>& vertices, 
								  OUT DataArray<int>& indices )					override;
			bool	LoadMesh	( const char* data, OUT DataArray<Vertex>& vertices, 
								  OUT DataArray<int>& indices )					override;
			String	SaveMesh	( DataArray<Vertex>& vertices, 
								  DataArray<int>& indices )						override;

			void SetSerializeSystem( SerializeSystem* serializeSystem )			override;


		private:
			FileSystem*	_fileSystem;
			MeshLoader*	_meshLoader;
		};
	}
}