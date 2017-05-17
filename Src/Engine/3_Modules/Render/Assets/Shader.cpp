// Copyright (c) 2015 Morco (www.morco.ro)
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
	NotAPI::RenderManagerImpl::Singleton()->DeleteShaderProgram( _programID );
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
	Memory::FreeArray( fileData );
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
	Memory::FreeArray( fileData );

	_emitEvents = oldEmitEvents;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Shader::ClearAsset()
{
	NotAPI::RenderManagerImpl::Singleton()->DeleteShaderProgram( _programID );
	_programID = INVALID_SHADER;

	_isTemporary = TRUE;

	_shaderFiles.Clear();
	_uniforms.Clear();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


Asset* Shader::Clone()
{
	return _BuildClone( Memory::Alloc<Shader>() );
}


const char* CYRED::Shader::GetExtension()
{
	if ( _useExtension ) {
		return FileManager::FILE_FORMAT_SHADER;
	}

	return NULL;
}


void Shader::SetShaderFiles( const char* rendererType, const char* vertexPath,
							 const char* geometryPath, const char* fragmentPath )
{
	if ( _shaderFiles.Has( rendererType ) )
	{
		_FilesPaths* paths = _shaderFiles.Get( rendererType );
		paths->vertex	= vertexPath;
		paths->geometry = geometryPath;
		paths->fragment = fragmentPath;
	}
	else
	{
		_FilesPaths* paths = Memory::Alloc<_FilesPaths>();
		paths->vertex	= vertexPath;
		paths->geometry = geometryPath;
		paths->fragment = fragmentPath;

		_shaderFiles.Set( rendererType, paths );
	}

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Shader::GetShaderFiles( const char* rendererType, OUT const char** vertexPath, 
							 OUT const char** geometryPath, OUT const char** fragmentPath ) const
{
	if ( _shaderFiles.Has( rendererType ) )
	{
		_FilesPaths* paths = _shaderFiles.Get( rendererType );
		*vertexPath = paths->vertex.GetChar();
		*geometryPath = paths->geometry.GetChar();
		*fragmentPath = paths->fragment.GetChar();
	}
}


void Shader::ChangeRenderer( const char* rendererType )
{
	if ( _shaderFiles.Has( rendererType ) )
	{
		_FilesPaths* paths = _shaderFiles.Get( rendererType );

		FiniteString vertexPath( "%s%s", _dirPath.GetChar(), paths->vertex.GetChar() );

		FiniteString geometryPath( "%s%s", _dirPath.GetChar(), paths->geometry.GetChar() );

		FiniteString fragmentPath( "%s%s", _dirPath.GetChar(), paths->fragment.GetChar() );

		char* vertexCode	= FileManager::Singleton()->ReadFile( vertexPath.GetChar() );
		char* geometryCode	= FileManager::Singleton()->ReadFile( geometryPath.GetChar() );
		char* fragmentCode	= FileManager::Singleton()->ReadFile( fragmentPath.GetChar() );

		ChangeRenderer( vertexCode,	geometryCode, fragmentCode );

		Memory::Free( vertexCode );
		Memory::Free( geometryCode );
		Memory::Free( fragmentCode );
	}
}


void Shader::ChangeRenderer( const char* vertexShader, const char* geometryShader, 
							 const char* fragmentShader )
{
	if ( vertexShader == NULL || geometryShader == NULL || fragmentShader == NULL )
	{
		return;
	}

	NotAPI::RenderManagerImpl* manager = NotAPI::RenderManagerImpl::Singleton();

	// delete old program
	manager->DeleteShaderProgram( _programID );

	_programID = manager->CreateShaderProgram( vertexShader, geometryShader, fragmentShader );
	
	_uniforms.Clear();

	//! add uniforms
	if ( _programID != INVALID_SHADER )
	{
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


int Shader::GetProgramID() const
{
	return _programID;
}


int Shader::GetUniformLocation( const char* uniform ) const
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
