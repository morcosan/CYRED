// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "Sections/FileSystem.h"
#include "Serialize/SerializeSystem.h"
#include "../../2_BuildingBlocks/Data/DataArray.h"
#include "../Render/OpenGL/Vertex.h"


namespace CYRED
{
	class MeshLoader;
}


namespace CYRED
{
	ABSTRACT class DLL FileManager
	{
		DECLARE_LOCAL_SINGLETON( FileManager )


	public:
		static cchar* const FILE_FORMAT_MESH;
		static cchar* const FILE_FORMAT_MORPH;
		static cchar* const FILE_FORMAT_MATERIAL;
		static cchar* const FILE_FORMAT_SHADER;
		static cchar* const FILE_FORMAT_TEXTURE;
		static cchar* const FILE_FORMAT_SCENE;
		static cchar* const FILE_FORMAT_SCRIPT;
		static cchar* const FILE_FORMAT_PREFAB;
		
		static cchar* const DIR_ASSETS;		


	public:
		virtual void	Initialize	( FileSystem* fileSystem, MeshLoader* meshLoader )	PURE_VIRTUAL;
		virtual void	Finalize	() PURE_VIRTUAL;

		virtual char*	ReadFile	( cchar* filePath, OUT int& fileSize )		PURE_VIRTUAL;
		virtual char*	ReadFile	( cchar* filePath )						PURE_VIRTUAL;
		/* returns TRUE on success */
		virtual bool	WriteFile	( cchar* filePath, cchar* buffer )	PURE_VIRTUAL;
		/* returns TRUE on success */
		virtual bool	DeleteFile	( cchar* filePath )						PURE_VIRTUAL;
		/* returns TRUE on success */
		virtual bool	CopyFile	( cchar* srcPath, cchar* dstPath )	PURE_VIRTUAL;


		/* returns TRUE on success */
		virtual bool	DeleteDir	( cchar* dirPath )							PURE_VIRTUAL;
		/* returns TRUE on success */
		virtual bool	CreateDir	( cchar* parentPath, cchar* dirName )	PURE_VIRTUAL;


		virtual uchar*	ReadImage	( cchar* filePath, OUT int *width, 
									  OUT int *height, OUT int *channels )			PURE_VIRTUAL;
		/* returns TRUE on success */
		virtual bool	WriteImage	( cchar* filePath, const uchar* imageBuffer, 
									  int width, int height, int channels,
									  ImageType type )								PURE_VIRTUAL;
		
		/* returns TRUE on success */
		virtual bool	ImportMesh	( cchar* data, int dataSize, 
									  OUT DataArray<Vertex>& vertices, 
									  OUT DataArray<int>& indices )				PURE_VIRTUAL;
		/* returns TRUE on success */
		virtual bool	LoadMesh	( cchar* data, OUT DataArray<Vertex>& vertices, 
									  OUT DataArray<int>& indices )				PURE_VIRTUAL;
		virtual String	SaveMesh	( DataArray<Vertex>& vertices, 
									  DataArray<int>& indices )					PURE_VIRTUAL;

		/* it takes ownership of the new object */
		virtual void SetSerializeSystem( SerializeSystem* serializeSystem )			PURE_VIRTUAL;

		/* requires a SerializeSystem assigned */
		template <class T> String	Serialize	( const T* object );
		/* requires a SerializeSystem assigned */
		template <class T> void		Deserialize	( cchar* data, OUT T* object,
												  DeserFlag flag = DeserFlag::FULL );	
	

	protected:
		SerializeSystem*	_serializeSystem;
	};



	template<class T>
	String FileManager::Serialize( const T* object )
	{
		ASSERT( _serializeSystem != NULL );

		return _serializeSystem->Serialize<T>( object );
	}


	template<class T>
	void FileManager::Deserialize( cchar* data, OUT T* object,
								   DeserFlag flag )
	{
		ASSERT( _serializeSystem != NULL );
		if ( data == NULL ) {
			return;
		}
		_serializeSystem->Deserialize<T>( data, object, flag );
	}
}