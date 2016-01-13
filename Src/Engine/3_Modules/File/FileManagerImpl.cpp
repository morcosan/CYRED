// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "FileManagerImpl.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition of SceneManager
DEFINE_LOCAL_SINGLETON( FileManager, FileManagerImpl )

DEFINE_LOCAL_SINGLETON_IMPL( FileManagerImpl )


const Char* const FileManager::FILE_FORMAT_MESH		= ".mesh";
const Char* const FileManager::FILE_FORMAT_MATERIAL	= ".mat";
const Char* const FileManager::FILE_FORMAT_SHADER	= ".shader";
const Char* const FileManager::FILE_FORMAT_TEXTURE	= ".tex";
const Char* const FileManager::FILE_FORMAT_SCENE	= ".scene";


void FileManagerImpl::Initialize( FileSystem* fileSystem )
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	ASSERT( fileSystem != NULL );

	_fileSystem = fileSystem;
}


void FileManagerImpl::Finalize()
{
	ASSERT( _isInitialized );

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

