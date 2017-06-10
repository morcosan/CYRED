// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Shader.h"
#include "../RenderManagerImpl.h"
#include "../../File/FileManager.h"
#include "../../Event/EventManager.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"


using namespace CYRED;



Shader::Shader()
	: Asset( AssetType::SHADER )
	, _programID( INVALID_SHADER )
{
}


Shader::~Shader()
{
	NonAPI::RenderManagerImpl::Singleton()->DeleteShaderProgram( _programID );
	_programID = INVALID_SHADER;
}


void Shader::LoadUniqueID()
{
	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_SHADER );
	}

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Shader>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	ARRAY_FREE( fileData );
}


void Shader::LoadFullFile()
{
	bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_SHADER );
	}

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Shader>( fileData, this );

	// free memory for file
	ARRAY_FREE( fileData );

	_emitEvents = oldEmitEvents;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


void Shader::ClearAsset()
{
	NonAPI::RenderManagerImpl::Singleton()->DeleteShaderProgram( _programID );
	_programID = INVALID_SHADER;

	_isTemporary = TRUE;

	// clear file paths
	_vertexFilePath == "";
	_geometryFilePath == "";
	_fragmentFilePath == "";
	// clear uniforms
	_uniforms.Clear();

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


Asset* Shader::Clone()
{
	return _BuildClone( new Shader() );
}


cchar* CYRED::Shader::GetExtension()
{
	if ( _useExtension ) {
		return FileManager::FILE_FORMAT_SHADER;
	}

	return NULL;
}


void Shader::SetShaderFiles( cchar* vertexPath, cchar* geometryPath, cchar* fragmentPath )
{
	_vertexFilePath		= vertexPath;
	_geometryFilePath	= geometryPath;
	_fragmentFilePath	= fragmentPath;

	if ( _emitEvents ) {
		EventManager::Singleton()->PushEvent( EventType::ASSET_UPDATE, this );
	}
}


void Shader::GetShaderFiles( OUT cchar** vertexPath, OUT cchar** geometryPath, 
							 OUT cchar** fragmentPath ) const
{
	*vertexPath		= _vertexFilePath.GetChar();
	*geometryPath	= _geometryFilePath.GetChar();
	*fragmentPath	= _fragmentFilePath.GetChar();
}


void Shader::LoadShaderProgram()
{
	FiniteString vertexPath( "%s%s", _dirPath.GetChar(), _vertexFilePath.GetChar() );
	FiniteString geometryPath( "%s%s", _dirPath.GetChar(), _geometryFilePath.GetChar() );
	FiniteString fragmentPath( "%s%s", _dirPath.GetChar(), _fragmentFilePath.GetChar() );

	char* vertexCode	= FileManager::Singleton()->ReadFile( vertexPath.GetChar() );
	char* geometryCode	= FileManager::Singleton()->ReadFile( geometryPath.GetChar() );
	char* fragmentCode	= FileManager::Singleton()->ReadFile( fragmentPath.GetChar() );

	// sanity check
	if ( vertexCode != NULL && geometryCode != NULL && fragmentCode != NULL ) {
		NonAPI::RenderManagerImpl* manager = NonAPI::RenderManagerImpl::Singleton();
		// delete old program
		manager->DeleteShaderProgram( _programID );
		// create nw program
		_programID = manager->CreateShaderProgram( vertexCode, geometryCode, fragmentCode );
		// clear uniforms
		_uniforms.Clear();

		// add uniforms
		if ( _programID != INVALID_SHADER )	{
			int total = manager->GetUniformsCount( _programID );

			for( int i = 0; i < total; ++i ) 
			{
				int nameLength = -1;
				char name[100]; // 100 char max, supposedly enough

				manager->GetUniformInfo( _programID, i, sizeof( name ) - 1, &nameLength, name );
				name[nameLength] = 0;

				int location = manager->GetUniformLocation( _programID, name );
				if ( location != INVALID_UNIFORM )
				{
					_uniforms.Set( name, location );
				}
			}
		}
	}

	PTR_FREE( vertexCode );
	PTR_FREE( geometryCode );
	PTR_FREE( fragmentCode );
}


int Shader::GetProgramID() const
{
	return _programID;
}


int Shader::GetUniformLocation( cchar* uniform ) const
{
	if ( _programID != INVALID_SHADER )
	{
		if ( _uniforms.Has( uniform ) )
		{
			return _uniforms.Get( uniform );
		}
	}

	return INVALID_UNIFORM;
}
