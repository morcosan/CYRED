// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "FileManager.h"


namespace CYRED
{
	namespace NonAPI
	{
		class FileManagerImpl : public FileManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( FileManagerImpl )


		public:
			void Initialize		( FileSystem* fileSystem, MeshLoader* meshLoader )	override;
			void Finalize		() override;

			char*	ReadFile	( cchar* filePath, OUT int& fileSize )		override;
			char*	ReadFile	( cchar* filePath )						override;
			bool	WriteFile	( cchar* filePath, cchar* buffer )	override;
			bool	DeleteFile	( cchar* filePath )						override;
			bool	CopyFile	( cchar* srcPath, cchar* dstPath )	override;
			
			bool	DeleteDir	( cchar* dirPath )							override;
			bool	CreateDir	( cchar* parentPath, cchar* dirName )	override;

			uchar*	ReadImage	( cchar* filePath, OUT int *width, 
								  OUT int *height, OUT int *channels )			override;
			bool	WriteImage	( cchar* filePath, const uchar* imageBuffer, 
								  int width, int height, int channels,
								  ImageType type )								override;
			
			bool	ImportMesh	( cchar* data, int dataSize, 
								  OUT DataArray<Vertex>& vertices, 
								  OUT DataArray<int>& indices )					override;
			bool	LoadMesh	( cchar* data, OUT DataArray<Vertex>& vertices, 
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