// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Texture.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"


using namespace CYRED;



void AttrViewer_Texture::OnSelect_Target( void* target )
{
	_target = CAST_S( Texture*, target );
	
	_CreateAttrString	( ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	DataArray<const Char*> types;
	types.Add( TYPE_TEXTURE_2D );
	types.Add( TYPE_CUBE_MAP );
	_CreateAttrDropdown	( ATTR_TYPE, types, AttrFlag::NONE, CallbackGroup::GROUP_2 );

	_CreateAttrBool		( ATTR_HAS_MIPMAP );
	_CreateAttrBool		( ATTR_CLEAR_BUFFER );
	_CreateAttrString	( ATTR_FILE_PATH );
	_CreateAttrString	( ATTR_FILE_PATH_POSX );
	_CreateAttrString	( ATTR_FILE_PATH_NEGX );
	_CreateAttrString	( ATTR_FILE_PATH_POSY );
	_CreateAttrString	( ATTR_FILE_PATH_NEGY );
	_CreateAttrString	( ATTR_FILE_PATH_POSZ );
	_CreateAttrString	( ATTR_FILE_PATH_NEGZ );
	
	_AddToPanel( TITLE );
}


void AttrViewer_Texture::_OnUpdate_GUI()
{
	DataUnion attr;

	_WriteAttribute( ATTR_NAME,			attr.SetString( _target->GetName() ) );

	Int typeIndex = 0;
	switch ( _target->GetType() )
	{
		case TextureType::TEXTURE_2D:
			typeIndex = 0;
			break;

		case TextureType::CUBE_MAP:
			typeIndex = 1;
			break;
	}
	_WriteAttribute( ATTR_TYPE,			attr.SetInt( typeIndex ) );

	_WriteAttribute( ATTR_HAS_MIPMAP,	attr.SetBool( _target->HasMipmap() ) );
	_WriteAttribute( ATTR_CLEAR_BUFFER,	attr.SetBool( _target->DoesClearBufferOnBind() ) );

	switch ( _target->GetType() )
	{
		case TextureType::TEXTURE_2D:
			_WriteAttribute( ATTR_FILE_PATH,	attr.SetString( _target->GetImagePath(0) ) );
			break;

		case TextureType::CUBE_MAP:
			_WriteAttribute( ATTR_FILE_PATH_POSX,	attr.SetString( _target->GetImagePath(0) ) );
			_WriteAttribute( ATTR_FILE_PATH_NEGX,	attr.SetString( _target->GetImagePath(1) ) );
			_WriteAttribute( ATTR_FILE_PATH_POSY,	attr.SetString( _target->GetImagePath(2) ) );
			_WriteAttribute( ATTR_FILE_PATH_NEGY,	attr.SetString( _target->GetImagePath(3) ) );
			_WriteAttribute( ATTR_FILE_PATH_POSZ,	attr.SetString( _target->GetImagePath(4) ) );
			_WriteAttribute( ATTR_FILE_PATH_NEGZ,	attr.SetString( _target->GetImagePath(5) ) );
			break;
	}

	_ChangeVisibility();
}


void AttrViewer_Texture::_OnUpdate_Target()
{
	_target->SetEmitEvents( FALSE );

	_target->SetName( _ReadAttribute( ATTR_NAME ).GetString() );

	if ( _activatedGroup == CallbackGroup::GROUP_2 )
	{
		TextureType type;
		Int typeIndex = _ReadAttribute( ATTR_TYPE ).GetInt();
		switch ( typeIndex )
		{
			case 0:
				type = TextureType::TEXTURE_2D;
				break;

			case 1:
				type = TextureType::CUBE_MAP;
				break;
		}

		_target->SetTextureType( type );

		_ChangeVisibility();
	}
	else
	{
		_target->SetHasMipmap( _ReadAttribute( ATTR_HAS_MIPMAP ).GetBool() );
		_target->SetClearBufferOnBind( _ReadAttribute( ATTR_CLEAR_BUFFER ).GetBool() );

		switch ( _target->GetType() )
		{
			case TextureType::TEXTURE_2D:
				_target->SetImagePath( 0, _ReadAttribute( ATTR_FILE_PATH ).GetString() );
				break;

			case TextureType::CUBE_MAP:
				_target->SetImagePath( 0, _ReadAttribute( ATTR_FILE_PATH_POSX ).GetString() );
				_target->SetImagePath( 1, _ReadAttribute( ATTR_FILE_PATH_NEGX ).GetString() );
				_target->SetImagePath( 2, _ReadAttribute( ATTR_FILE_PATH_POSY ).GetString() );
				_target->SetImagePath( 3, _ReadAttribute( ATTR_FILE_PATH_NEGY ).GetString() );
				_target->SetImagePath( 4, _ReadAttribute( ATTR_FILE_PATH_POSZ ).GetString() );
				_target->SetImagePath( 5, _ReadAttribute( ATTR_FILE_PATH_NEGZ ).GetString() );
				break;
		}
	}

	_target->SetEmitEvents( TRUE );

	EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, _target );
}


void AttrViewer_Texture::_ChangeVisibility()
{
	switch ( _target->GetType() )
	{
		case TextureType::TEXTURE_2D:
			_SetAttrVisibility( ATTR_FILE_PATH, TRUE );
			_SetAttrVisibility( ATTR_FILE_PATH_POSX, FALSE );
			_SetAttrVisibility( ATTR_FILE_PATH_NEGX, FALSE );
			_SetAttrVisibility( ATTR_FILE_PATH_POSY, FALSE );
			_SetAttrVisibility( ATTR_FILE_PATH_NEGY, FALSE );
			_SetAttrVisibility( ATTR_FILE_PATH_POSZ, FALSE );
			_SetAttrVisibility( ATTR_FILE_PATH_NEGZ, FALSE );
			break;

		case TextureType::CUBE_MAP:
			_SetAttrVisibility( ATTR_FILE_PATH, FALSE );
			_SetAttrVisibility( ATTR_FILE_PATH_POSX, TRUE );
			_SetAttrVisibility( ATTR_FILE_PATH_NEGX, TRUE );
			_SetAttrVisibility( ATTR_FILE_PATH_POSY, TRUE );
			_SetAttrVisibility( ATTR_FILE_PATH_NEGY, TRUE );
			_SetAttrVisibility( ATTR_FILE_PATH_POSZ, TRUE );
			_SetAttrVisibility( ATTR_FILE_PATH_NEGZ, TRUE );
			break;
	}

	_UpdateVisibility();
}

