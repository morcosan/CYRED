// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Material.h"
#include "Shader.h"
#include "../../File/FileManager.h"
#include "../../Event/EventManager.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"


using namespace CYRED;



Material::Material()
	: Asset( AssetType::MATERIAL )
	, _shader( NULL )
	, _isWireframe( FALSE )
	, _lineWidth( 1.0f )
	, _faceCulling( FaceCulling::CULL_BACK )
{
}


void Material::LoadUniqueID()
{
	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_MATERIAL );
	}

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Material>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	Memory::FreeArray( fileData );
}


void Material::LoadFullFile()
{
	bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_MATERIAL );
	}

	char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Material>( fileData, this );

	// free memory for file
	Memory::FreeArray( fileData );

	_emitEvents = oldEmitEvents;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Material::ClearAsset()
{
	_shader = NULL;
	_isWireframe = FALSE;
	_lineWidth = 1.0f;
	_faceCulling = FaceCulling::CULL_BACK;
	_isTemporary = TRUE;
	ClearProperties();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


Asset* Material::Clone()
{
	return _BuildClone( Memory::Alloc<Material>() );
}


const char* CYRED::Material::GetExtension()
{
	if ( _useExtension ) {
		return FileManager::FILE_FORMAT_MATERIAL;
	}
	return NULL;
}


Shader* Material::GetShader() const
{
	return _shader;
}


bool Material::IsWireframe() const
{
	return _isWireframe;
}


float Material::GetLineWidth() const
{
	return _lineWidth;
}


FaceCulling Material::GetFaceCulling() const
{
	return _faceCulling;
}


void Material::SetShader( Shader* shader )
{
	_shader = shader;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Material::SetWireframe( bool value )
{
	_isWireframe = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Material::SetLineWidth( float value )
{
	_lineWidth = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Material::SetFaceCulling( FaceCulling value )
{
	_faceCulling = value;

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Material::SetProperty( const char* name, int value )
{
	for ( int i = 0; i < _properties.Size(); ++i )
	{
		if ( _properties[i].name == name )
		{
			_properties[i].data.SetInt( value );
			return;
		}
	}

	_Property property;
	property.name = name;
	property.data.SetInt( value );
	_properties.Add( property );

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Material::SetProperty( const char* name, float value )
{
	for ( int i = 0; i < _properties.Size(); ++i )
	{
		if ( _properties[i].name == name )
		{
			_properties[i].data.SetFloat( value );
			return;
		}
	}

	_Property property;
	property.name = name;
	property.data.SetFloat( value );
	_properties.Add( property );

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Material::SetProperty( const char* name, const Vector2& value )
{
	for ( int i = 0; i < _properties.Size(); ++i )
	{
		if ( _properties[i].name == name )
		{
			_properties[i].data.SetVector2( value );
			return;
		}
	}

	_Property property;
	property.name = name;
	property.data.SetVector2( value );
	_properties.Add( property );

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Material::SetProperty( const char* name, const Vector3& value )
{
	for ( int i = 0; i < _properties.Size(); ++i )
	{
		if ( _properties[i].name == name )
		{
			_properties[i].data.SetVector3( value );
			return;
		}
	}

	_Property property;
	property.name = name;
	property.data.SetVector3( value );
	_properties.Add( property );

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Material::SetProperty( const char* name, const Vector4& value )
{
	for ( int i = 0; i < _properties.Size(); ++i )
	{
		if ( _properties[i].name == name )
		{
			_properties[i].data.SetVector4( value );
			return;
		}
	}

	_Property property;
	property.name = name;
	property.data.SetVector4( value );
	_properties.Add( property );

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


void Material::SetProperty( const char* name, Texture* value )
{
	for ( int i = 0; i < _properties.Size(); ++i )
	{
		if ( _properties[i].name == name )
		{
			_properties[i].data.SetReference( value );
			return;
		}
	}

	_Property property;
	property.name = name;
	property.data.SetReference( value );
	_properties.Add( property );

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


int Material::GetPropertiesCount() const
{
	return _properties.Size();
}


DataUnion& Material::GetPropertyDataAt( int index ) const
{
	ASSERT( index < _properties.Size() );

	return _properties[index].data;
}


const char* Material::GetPropertyNameAt( int index ) const
{
	ASSERT( index < _properties.Size() );

	return _properties[index].name.GetChar();
}


void Material::ClearProperties()
{
	_properties.Clear();

	if ( _emitEvents ) {
		EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, this );
	}
}


