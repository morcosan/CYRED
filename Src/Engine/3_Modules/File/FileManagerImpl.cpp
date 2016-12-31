// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "FileManagerImpl.h"
#include "Sections\MeshLoader.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition
DEFINE_LOCAL_SINGLETON( FileManager, FileManagerImpl )
DEFINE_LOCAL_SINGLETON_IMPL( FileManagerImpl )


const Char* const FileManager::FILE_FORMAT_MESH		= ".mesh";
const Char* const FileManager::FILE_FORMAT_MORPH	= ".morph";
const Char* const FileManager::FILE_FORMAT_MATERIAL	= ".mat";
const Char* const FileManager::FILE_FORMAT_SHADER	= ".shader";
const Char* const FileManager::FILE_FORMAT_TEXTURE	= ".tex";
const Char* const FileManager::FILE_FORMAT_SCENE	= ".scene";
const Char* const FileManager::FILE_FORMAT_SCRIPT	= ".script";
const Char* const FileManager::DIR_ASSETS			= "Assets/";


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


Char* FileManagerImpl::ReadFile( const Char* filePath, OUT Int& fileSize )
{
	ASSERT( _isInitialized );
	return _fileSystem->ReadFile( filePath, fileSize );
}


Char* FileManagerImpl::ReadFile( const Char* filePath )
{
	ASSERT( _isInitialized );
	Int fileSize;
	return _fileSystem->ReadFile( filePath, fileSize );
}


Bool FileManagerImpl::WriteFile( const Char* filePath, const Char* buffer )
{
	ASSERT( _isInitialized );
	return _fileSystem->WriteFile( filePath, buffer );
}


Bool FileManagerImpl::DeleteFile( const Char* filePath )
{
	ASSERT( _isInitialized );
	return _fileSystem->DeleteFile( filePath );
}


Bool FileManagerImpl::CopyFile( const Char* srcPath, const Char* dstPath )
{
	ASSERT( _isInitialized );
	return _fileSystem->CopyFile( srcPath, dstPath );
}


Bool FileManagerImpl::DeleteDir( const Char* dirPath )
{
	ASSERT( _isInitialized );
	return _fileSystem->DeleteDir( dirPath );
}


Bool FileManagerImpl::CreateDir( const Char* parentPath, const Char* dirName )
{
	ASSERT( _isInitialized );
	return _fileSystem->CreateDir( parentPath, dirName );
}


UChar* FileManagerImpl::ReadImage( const Char* filePath, OUT Int* width, OUT Int* height, 
								   OUT Int* channels )
{
	ASSERT( _isInitialized );
	return _fileSystem->ReadImage( filePath, width, height, channels );
}


Bool FileManagerImpl::WriteImage( const Char* filePath, const UChar* imageBuffer, 
								  Int width, Int height, Int channels, ImageType type )
{
	ASSERT( _isInitialized );
	return _fileSystem->WriteImage( filePath, imageBuffer, width, height, channels, type );
}


Bool FileManagerImpl::ImportMesh( const Char* data, UInt dataSize, 
								  OUT DataArray<Vertex>& vertices, 
								  OUT DataArray<UInt>& indices )
{
	ASSERT( _isInitialized );
	return _meshLoader->ImportMesh( data, dataSize, vertices, indices );
}


Bool FileManagerImpl::LoadMesh( const Char* data, OUT DataArray<Vertex>& vertices, 
								OUT DataArray<UInt>& indices )
{
	ASSERT( _isInitialized );
	return _meshLoader->LoadMesh( data, vertices, indices );
}


String FileManagerImpl::SaveMesh( DataArray<Vertex>& vertices, DataArray<UInt>& indices )
{
	ASSERT( _isInitialized );
	return _meshLoader->SaveMesh( vertices, indices );
}


