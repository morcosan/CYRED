// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "FileManagerImpl.h"
#include "Sections\MeshLoader.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition
DEFINE_LOCAL_SINGLETON( FileManager, FileManagerImpl )
DEFINE_LOCAL_SINGLETON_IMPL( FileManagerImpl )


const char* const FileManager::FILE_FORMAT_MESH		= ".mesh";
const char* const FileManager::FILE_FORMAT_MORPH	= ".morph";
const char* const FileManager::FILE_FORMAT_MATERIAL	= ".mat";
const char* const FileManager::FILE_FORMAT_SHADER	= ".shader";
const char* const FileManager::FILE_FORMAT_TEXTURE	= ".tex";
const char* const FileManager::FILE_FORMAT_SCENE	= ".scene";
const char* const FileManager::FILE_FORMAT_SCRIPT	= ".script";
const char* const FileManager::FILE_FORMAT_PREFAB	= ".prefab";
const char* const FileManager::DIR_ASSETS			= "Assets/";


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

	Memory::Free( _fileSystem );
	_fileSystem = NULL;
}


void FileManagerImpl::SetSerializeSystem( SerializeSystem* serializeSystem )
{
	Memory::Free( _serializeSystem );

	_serializeSystem = serializeSystem;
}


char* FileManagerImpl::ReadFile( const char* filePath, OUT int& fileSize )
{
	ASSERT( _isInitialized );
	return _fileSystem->ReadFile( filePath, fileSize );
}


char* FileManagerImpl::ReadFile( const char* filePath )
{
	ASSERT( _isInitialized );
	int fileSize;
	return _fileSystem->ReadFile( filePath, fileSize );
}


bool FileManagerImpl::WriteFile( const char* filePath, const char* buffer )
{
	ASSERT( _isInitialized );
	return _fileSystem->WriteFile( filePath, buffer );
}


bool FileManagerImpl::DeleteFile( const char* filePath )
{
	ASSERT( _isInitialized );
	return _fileSystem->DeleteFile( filePath );
}


bool FileManagerImpl::CopyFile( const char* srcPath, const char* dstPath )
{
	ASSERT( _isInitialized );
	return _fileSystem->CopyFile( srcPath, dstPath );
}


bool FileManagerImpl::DeleteDir( const char* dirPath )
{
	ASSERT( _isInitialized );
	return _fileSystem->DeleteDir( dirPath );
}


bool FileManagerImpl::CreateDir( const char* parentPath, const char* dirName )
{
	ASSERT( _isInitialized );
	return _fileSystem->CreateDir( parentPath, dirName );
}


uchar* FileManagerImpl::ReadImage( const char* filePath, OUT int* width, OUT int* height, 
								   OUT int* channels )
{
	ASSERT( _isInitialized );
	return _fileSystem->ReadImage( filePath, width, height, channels );
}


bool FileManagerImpl::WriteImage( const char* filePath, const uchar* imageBuffer, 
								  int width, int height, int channels, ImageType type )
{
	ASSERT( _isInitialized );
	return _fileSystem->WriteImage( filePath, imageBuffer, width, height, channels, type );
}


bool FileManagerImpl::ImportMesh( const char* data, int dataSize, 
								  OUT DataArray<Vertex>& vertices, 
								  OUT DataArray<int>& indices )
{
	ASSERT( _isInitialized );
	return _meshLoader->ImportMesh( data, dataSize, vertices, indices );
}


bool FileManagerImpl::LoadMesh( const char* data, OUT DataArray<Vertex>& vertices, 
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


