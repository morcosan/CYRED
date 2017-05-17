// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Material.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"
#include "../Selectors/Selector_Shader.h"
#include "../Selectors/Selector_Texture.h"


using namespace CYRED;



void AttrViewer_Material::_OnInitialize()
{
	_CreateAttrString	( ATTR_NAME,		ATTR_NAME,		AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );
	_CreateAttrSelector	( ATTR_SHADER,		ATTR_SHADER,	Selector_Shader::TYPE );
	_CreateAttrBool		( ATTR_WIREFRAME,	ATTR_WIREFRAME );
	_CreateAttrFloat	( ATTR_LINE_WIDTH,	ATTR_LINE_WIDTH );
	
	DataArray<const char*> cullFaceTypes;
	cullFaceTypes.Add( "CULL_BACK" );
	cullFaceTypes.Add( "CULL_FRONT" );
	cullFaceTypes.Add( "CULL_NONE" );
	_CreateAttrDropdown	( ATTR_CULL_FACE, ATTR_CULL_FACE, cullFaceTypes );

	{
		DataArray<AttrStruct> propertiesScheme;

		DataArray<AttrStruct> propTypeScheme;
		propTypeScheme.Add( AttrStruct{ ATTR_PROP_NAME, AttrType::STRING } );
		propTypeScheme.Add( AttrStruct{ ATTR_PROP_VALUE } );

		AttrStruct propStruct;
		propStruct.attrType = AttrType::LIST;
		propStruct.listType = AttrType::STRUCT;

		{
			propTypeScheme[1].attrType = AttrType::FLOAT;
			propStruct.attrName = ATTR_PROP_TYPE_FLOAT;
			propStruct.structScheme = propTypeScheme;
			propertiesScheme.Add( propStruct );
		}
		{
			propTypeScheme[1].attrType = AttrType::INT;
			propStruct.attrName = ATTR_PROP_TYPE_INT;
			propStruct.structScheme = propTypeScheme;
			propertiesScheme.Add( propStruct );
		}
		{
			propTypeScheme[1].attrType = AttrType::VECTOR2;
			propStruct.attrName = ATTR_PROP_TYPE_VECTOR2;
			propStruct.structScheme = propTypeScheme;
			propertiesScheme.Add( propStruct );
		}
		{
			propTypeScheme[1].attrType = AttrType::VECTOR3;
			propStruct.attrName = ATTR_PROP_TYPE_VECTOR3;
			propStruct.structScheme = propTypeScheme;
			propertiesScheme.Add( propStruct );
		}
		{
			propTypeScheme[1].attrType = AttrType::VECTOR4;
			propStruct.attrName = ATTR_PROP_TYPE_VECTOR4;
			propStruct.structScheme = propTypeScheme;
			propertiesScheme.Add( propStruct );
		}
		{
			propTypeScheme[1].attrType = AttrType::SELECTOR;
			propTypeScheme[1].selectorDataType = Selector_Texture::TYPE;
			propStruct.attrName = ATTR_PROP_TYPE_TEXTURE;
			propStruct.structScheme = propTypeScheme;
			propertiesScheme.Add( propStruct );
		}

		_CreateAttrStruct( ATTR_PROPERTIES, ATTR_PROPERTIES, propertiesScheme );
	}
	
	_AddToPanel( TITLE );
}


void AttrViewer_Material::_OnChangeTarget( void* target )
{
	_target = CAST_S( Material*, target );
}


void AttrViewer_Material::_OnUpdateGUI()
{
	_WriteAttrString( ATTR_NAME, _target->GetName() );

	Shader* shader = _target->GetShader();
	const char* shaderName = (shader == NULL) ? Selector_Shader::OPTION_NULL : shader->GetName();
	_WriteAttrSelector( ATTR_SHADER,	 shader, shaderName );

	_WriteAttrBool( ATTR_WIREFRAME,	_target->IsWireframe() );
	_WriteAttrFloat( ATTR_LINE_WIDTH, _target->GetLineWidth() );

	int cullFaceIndex = 0;
	switch ( _target->GetFaceCulling() )
	{
		case FaceCulling::CULL_BACK:
			cullFaceIndex = 0;
			break;

		case FaceCulling::CULL_FRONT:
			cullFaceIndex = 1;
			break;

		case FaceCulling::CULL_NONE:
			cullFaceIndex = 2;
			break;
	}
	_WriteAttrDropdown( ATTR_CULL_FACE,	cullFaceIndex );

	{
		DataUnion attrValue;
		int totalProperties = _target->GetPropertiesCount();
		int totalFloats	= 0;
		int totalInts		= 0;
		int totalVec2s		= 0;
		int totalVec3s		= 0;
		int totalVec4s		= 0;
		int totalTexs		= 0;

		_WriteAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_INT, 0 );
		_WriteAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_FLOAT, 0 );
		_WriteAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR2, 0 );
		_WriteAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR3, 0 );
		_WriteAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR4, 0 );
		_WriteAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_TEXTURE, 0 );

		for ( int i = 0; i < totalProperties; ++i )
		{
			const char* name = _target->GetPropertyNameAt( i );

			DataUnion& data = _target->GetPropertyDataAt( i );
			switch ( data.GetValueType() )
			{
				case DataUnion::INT:
				{
					++totalInts;
					_WriteAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_INT, totalInts );

					_WriteAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_INT, totalInts - 1, 
											ATTR_PROP_NAME, attrValue.SetString( name ) );
					_WriteAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_INT,
											totalInts - 1, ATTR_PROP_VALUE, data );
					break;
				}

				case DataUnion::FLOAT:
				{
					++totalFloats;
					_WriteAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_FLOAT, totalFloats );
					
					_WriteAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_FLOAT, totalFloats - 1, 
											ATTR_PROP_NAME, attrValue.SetString( name ) );
					_WriteAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_FLOAT,
											totalFloats - 1, ATTR_PROP_VALUE, data );
					break;
				}

				case DataUnion::VECTOR2:
				{
					++totalVec2s;
					_WriteAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR2, totalVec2s );
					
					_WriteAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR2, totalVec2s - 1, 
											ATTR_PROP_NAME, attrValue.SetString( name ) );
					_WriteAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR2,
											totalVec2s - 1, ATTR_PROP_VALUE, data );
					break;
				}

				case DataUnion::VECTOR3:
				{
					++totalVec3s;
					_WriteAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR3, totalVec3s );
					
					_WriteAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR3, totalVec3s - 1, 
											ATTR_PROP_NAME, attrValue.SetString( name ) );
					_WriteAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR3,
											totalVec3s - 1, ATTR_PROP_VALUE, data );
					break;
				}

				case DataUnion::VECTOR4:
				{
					++totalVec4s;
					_WriteAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR4, totalVec4s );
					
					_WriteAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR4, totalVec4s - 1, 
											ATTR_PROP_NAME, attrValue.SetString( name ) );
					_WriteAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR4,
											totalVec4s - 1, ATTR_PROP_VALUE, data );
					break;
				}

				case DataUnion::REFERENCE:
				{
					++totalTexs;
					_WriteAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_TEXTURE, totalTexs );
					
					_WriteAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_TEXTURE, totalTexs - 1, 
											ATTR_PROP_NAME, attrValue.SetString( name ) );
					
					Texture* texture = CAST_S( Texture*, data.GetReference() );
					const char* textureName = (texture == NULL) ? 
												Selector_Texture::OPTION_NULL : 
												texture->GetName();
					_WriteAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_TEXTURE,
											totalTexs - 1, ATTR_PROP_VALUE, data, textureName );
					break;
				}
			}
		}
	}
}


void AttrViewer_Material::_OnUpdateTarget()
{
	_target->SetEmitEvents( FALSE );

	_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );

	_target->SetShader( CAST_S( Shader*, _ReadAttrSelector( ATTR_SHADER ) ) );
	_target->SetWireframe( _ReadAttrBool( ATTR_WIREFRAME ) );
	_target->SetLineWidth( _ReadAttrFloat( ATTR_LINE_WIDTH ) );

	FaceCulling	faceCulling;
	int cullFaceIndex = _ReadAttrDropdown( ATTR_CULL_FACE );
	switch ( cullFaceIndex )
	{
		case 0:
			faceCulling = FaceCulling::CULL_BACK;
			break;

		case 1:
			faceCulling = FaceCulling::CULL_FRONT;
			break;

		case 2:
			faceCulling = FaceCulling::CULL_NONE;
			break;
	}
	_target->SetFaceCulling( faceCulling );

	{
		_target->ClearProperties();

		int totalFloats	= _ReadAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_FLOAT );
		int totalInts		= _ReadAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_INT );
		int totalVec2s		= _ReadAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR2 );
		int totalVec3s		= _ReadAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR3 );
		int totalVec4s		= _ReadAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR4 );
		int totalTexs		= _ReadAttrStrListSize( ATTR_PROPERTIES, ATTR_PROP_TYPE_TEXTURE );

		for ( int i = 0; i < totalFloats; ++i ) {
			DataUnion& propName = _ReadAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_FLOAT, 
														  i, ATTR_PROP_NAME );
			DataUnion& propValue = _ReadAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_FLOAT, 
														  i, ATTR_PROP_VALUE );
			_target->SetProperty( propName.GetString(), propValue.GetFloat() );
		}

		for ( int i = 0; i < totalInts; ++i ) {
			DataUnion& propName = _ReadAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_INT, 
														  i, ATTR_PROP_NAME );
			DataUnion& propValue = _ReadAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_INT, 
														  i, ATTR_PROP_VALUE );
			_target->SetProperty( propName.GetString(), propValue.GetInt() );
		}

		for ( int i = 0; i < totalVec2s; ++i )	{
			DataUnion& propName = _ReadAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR2, 
														  i, ATTR_PROP_NAME );
			DataUnion& propValue = _ReadAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR2, 
														  i, ATTR_PROP_VALUE );
			_target->SetProperty( propName.GetString(), propValue.GetVector2() );
		}

		for ( int i = 0; i < totalVec3s; ++i )	{
			DataUnion& propName = _ReadAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR3, 
														  i, ATTR_PROP_NAME );
			DataUnion& propValue = _ReadAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR3, 
														  i, ATTR_PROP_VALUE );
			_target->SetProperty( propName.GetString(), propValue.GetVector3() );
		}

		for ( int i = 0; i < totalVec4s; ++i )	{
			DataUnion& propName = _ReadAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR4, 
														  i, ATTR_PROP_NAME );
			DataUnion& propValue = _ReadAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_VECTOR4, 
														  i, ATTR_PROP_VALUE );
			_target->SetProperty( propName.GetString(), propValue.GetVector4() );
		}

		for ( int i = 0; i < totalTexs; ++i ) {
			DataUnion& propName = _ReadAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_TEXTURE, 
														  i, ATTR_PROP_NAME );
			DataUnion& propValue = _ReadAttrStrListIndex( ATTR_PROPERTIES, ATTR_PROP_TYPE_TEXTURE, 
														  i, ATTR_PROP_VALUE );
			_target->SetProperty( propName.GetString(), CAST_S( Texture*, propValue.GetReference() ) );
		}
	}

	_target->SetEmitEvents( TRUE );

	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, _target );
}

