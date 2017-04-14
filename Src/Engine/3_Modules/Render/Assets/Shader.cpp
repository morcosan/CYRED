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

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Shader>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	Memory::FreeArray( fileData );
}


void Shader::LoadFullFile()
{
	Bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_SHADER );
	}

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
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


const Char* CYRED::Shader::GetExtension()
{
	if ( _useExtension ) {
		return FileManager::FILE_FORMAT_SHADER;
	}

	return NULL;
}


void Shader::SetShaderFiles( const Char* rendererType, const Char* vertexPath,
							 const Char* geometryPath, const Char* fragmentPath )
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


void Shader::GetShaderFiles( const Char* rendererType, OUT const Char** vertexPath, 
							 OUT const Char** geometryPath, OUT const Char** fragmentPath ) const
{
	if ( _shaderFiles.Has( rendererType ) )
	{
		_FilesPaths* paths = _shaderFiles.Get( rendererType );
		*vertexPath = paths->vertex.GetChar();
		*geometryPath = paths->geometry.GetChar();
		*fragmentPath = paths->fragment.GetChar();
	}
}


void Shader::ChangeRenderer( const Char* rendererType )
{
	if ( _shaderFiles.Has( rendererType ) )
	{
		_FilesPaths* paths = _shaderFiles.Get( rendererType );

		FiniteString vertexPath( "%s%s", _dirPath.GetChar(), paths->vertex.GetChar() );

		FiniteString geometryPath( "%s%s", _dirPath.GetChar(), paths->geometry.GetChar() );

		FiniteString fragmentPath( "%s%s", _dirPath.GetChar(), paths->fragment.GetChar() );

		Char* vertexCode	= FileManager::Singleton()->ReadFile( vertexPath.GetChar() );
		Char* geometryCode	= FileManager::Singleton()->ReadFile( geometryPath.GetChar() );
		Char* fragmentCode	= FileManager::Singleton()->ReadFile( fragmentPath.GetChar() );

		ChangeRenderer( vertexCode,	geometryCode, fragmentCode );

		Memory::Free( vertexCode );
		Memory::Free( geometryCode );
		Memory::Free( fragmentCode );
	}
}


void Shader::ChangeRenderer( const Char* vertexShader, const Char* geometryShader, 
							 const Char* fragmentShader )
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
		UInt total = manager->GetUniformsCount( _programID );

		for( UInt i = 0; i < total; ++i ) 
		{
			Int nameLength = -1;
			Char name[100]; // 100 char max, supposedly enough

			manager->GetUniformInfo( _programID, i, sizeof( name ) - 1, &nameLength, name );
			name[nameLength] = 0;

			Int location = manager->GetUniformLocation( _programID, name );
			if ( location != INVALID_UNIFORM )
			{
				_uniforms.Set( name, location );
			}
		}
	}
}


Int Shader::GetProgramID() const
{
	return _programID;
}


Int Shader::GetUniformLocation( const Char* uniform ) const
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
