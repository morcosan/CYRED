// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "FileManagerImpl.h"
#include "Sections\MeshLoader.h"


using namespace CYRED;
using namespace NonAPI;


//! deferred definition
DEFINE_LOCAL_SINGLETON( FileManager, FileManagerImpl )
DEFINE_LOCAL_SINGLETON_IMPL( FileManagerImpl )


cchar* const FileManager::FILE_FORMAT_MESH		= ".mesh";
cchar* const FileManager::FILE_FORMAT_MORPH	= ".morph";
cchar* const FileManager::FILE_FORMAT_MATERIAL	= ".mat";
cchar* const FileManager::FILE_FORMAT_SHADER	= ".shader";
cchar* const FileManager::FILE_FORMAT_TEXTURE	= ".tex";
cchar* const FileManager::FILE_FORMAT_SCENE	= ".scene";
cchar* const FileManager::FILE_FORMAT_SCRIPT	= ".script";
cchar* const FileManager::FILE_FORMAT_PREFAB	= ".prefab";
cchar* const FileManager::DIR_ASSETS			= "Assets/";


void FileManagerImpl::Initialize( FileSystem* fileSystem, MeshLoader* meshLoader )
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	ASSERT( fileSystem != NULL );
	_fileSystem = fileSystem;

	ASSERT( meshLoader != NULL );
	_meshLoader = meshLoader;
}


void FileManagerImpl::Finalize()
{
	if ( !_isInitialized )
	{
		return;
	}

	PTR_FREE( _fileSystem );
	_fileSystem = NULL;
}


void FileManagerImpl::SetSerializeSystem( SerializeSystem* serializeSystem )
{
	PTR_FREE( _serializeSystem );

	_serializeSystem = serializeSystem;
}


char* FileManagerImpl::ReadFile( cchar* filePath, OUT int& fileSize )
{
	ASSERT( _isInitialized );
	return _fileSystem->ReadFile( filePath, fileSize );
}


char* FileManagerImpl::ReadFile( cchar* filePath )
{
	ASSERT( _isInitialized );
	int fileSize;
	return _fileSystem->ReadFile( filePath, fileSize );
}


bool FileManagerImpl::WriteFile( cchar* filePath, cchar* buffer )
{
	ASSERT( _isInitialized );
	return _fileSystem->WriteFile( filePath, buffer );
}


bool FileManagerImpl::DeleteFile( cchar* filePath )
{
	ASSERT( _isInitialized );
	return _fileSystem->DeleteFile( filePath );
}


bool FileManagerImpl::CopyFile( cchar* srcPath, cchar* dstPath )
{
	ASSERT( _isInitialized );
	return _fileSystem->CopyFile( srcPath, dstPath );
}


bool FileManagerImpl::DeleteDir( cchar* dirPath )
{
	ASSERT( _isInitialized );
	return _fileSystem->DeleteDir( dirPath );
}


bool FileManagerImpl::CreateDir( cchar* parentPath, cchar* dirName )
{
	ASSERT( _isInitialized );
	return _fileSystem->CreateDir( parentPath, dirName );
}


uchar* FileManagerImpl::ReadImage( cchar* filePath, OUT int* width, OUT int* height, 
								   OUT int* channels )
{
	ASSERT( _isInitialized );
	return _fileSystem->ReadImage( filePath, width, height, channels );
}


bool FileManagerImpl::WriteImage( cchar* filePath, const uchar* imageBuffer, 
								  int width, int height, int channels, ImageType type )
{
	ASSERT( _isInitialized );
	return _fileSystem->WriteImage( filePath, imageBuffer, width, height, channels, type );
}


bool FileManagerImpl::ImportMesh( cchar* data, int dataSize, 
								  OUT DataArray<Vertex>& vertices, 
								  OUT DataArray<int>& indices )
{
	ASSERT( _isInitialized );
	return _meshLoader->ImportMesh( data, dataSize, vertices, indices );
}


bool FileManagerImpl::LoadMesh( cchar* data, OUT DataArray<Vertex>& vertices, 
								OUT DataArray<int>& indices )
{
	ASSERT( _isInitialized );
	return _meshLoader->LoadMesh( data, vertices, indices );
}


String FileManagerImpl::SaveMesh( DataArray<Vertex>& vertices, DataArray<int>& indices )
{
	ASSERT( _isInitialized );
	return _meshLoader->SaveMesh( vertices, indices );
}


