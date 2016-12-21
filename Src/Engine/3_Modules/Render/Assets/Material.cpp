// Copyright (c) 2015 Morco (www.morco.ro)
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

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Material>( fileData, this, DeserFlag::UID_ONLY );

	// free memory for file
	Memory::FreeArray( fileData );
}


void Material::LoadFullFile()
{
	Bool oldEmitEvents = _emitEvents;
	_emitEvents = FALSE;

	// create path
	FiniteString filePath( "%s%s", _dirPath.GetChar(), _name.GetChar() );
	// add extension of needed
	if ( _useExtension ) {
		filePath.Set( "%s%s", filePath.GetChar(), FileManager::FILE_FORMAT_MATERIAL );
	}

	Char* fileData = FileManager::Singleton()->ReadFile( filePath.GetChar() );
	FileManager::Singleton()->Deserialize<Material>( fileData, this );

	// free memory for file
	Memory::FreeArray( fileData );

	_emitEvents = oldEmitEvents;

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
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

	if ( _emitEvents )
	{
		EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, this );
	}
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
}


void Material::SetWireframe( bool value )
{
	_isWireframe = value;
}


void Material::SetLineWidth( float value )
{
	_lineWidth = value;
}


void Material::SetFaceCulling( FaceCulling value )
{
	_faceCulling = value;
}


void Material::SetProperty( const Char* name, Int value )
{
	for ( UInt i = 0; i < _properties.Size(); ++i )
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
}


void Material::SetProperty( const Char* name, Float value )
{
	for ( UInt i = 0; i < _properties.Size(); ++i )
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
}


void Material::SetProperty( const Char* name, const Vector2& value )
{
	for ( UInt i = 0; i < _properties.Size(); ++i )
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
}


void Material::SetProperty( const Char* name, const Vector3& value )
{
	for ( UInt i = 0; i < _properties.Size(); ++i )
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
}


void Material::SetProperty( const Char* name, const Vector4& value )
{
	for ( UInt i = 0; i < _properties.Size(); ++i )
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
}


void Material::SetProperty( const Char* name, Texture* value )
{
	for ( UInt i = 0; i < _properties.Size(); ++i )
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
}


UInt Material::GetPropertiesCount()
{
	return _properties.Size();
}


DataUnion& Material::GetPropertyDataAt( UInt index )
{
	ASSERT( index < _properties.Size() );

	return _properties[index].data;
}


const Char* Material::GetPropertyNameAt( UInt index )
{
	ASSERT( index < _properties.Size() );

	return _properties[index].name.GetChar();
}


void Material::ClearProperties()
{
	_properties.Clear();
}


