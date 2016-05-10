// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Texture.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"


using namespace CYRED;



void AttrViewer_Texture::_OnInitialize()
{
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


void AttrViewer_Texture::_OnChangeTarget( void* target )
{
	_target = CAST_S( Texture*, target );
}


void AttrViewer_Texture::_OnUpdateGUI()
{
	_WriteAttrString( ATTR_NAME, _target->GetName() );

	Int typeIndex = 0;
	switch ( _target->GetTextureType() )
	{
		case TextureType::TEXTURE_2D:
			typeIndex = 0;
			break;

		case TextureType::CUBE_MAP:
			typeIndex = 1;
			break;
	}
	_WriteAttrDropdown( ATTR_TYPE, typeIndex );

	_WriteAttrBool( ATTR_HAS_MIPMAP, _target->HasMipmap() );
	_WriteAttrBool( ATTR_CLEAR_BUFFER, _target->DoesClearBufferOnBind() );

	switch ( _target->GetTextureType() )
	{
		case TextureType::TEXTURE_2D:
			_WriteAttrString( ATTR_FILE_PATH, _target->GetImagePath(0) );
			break;

		case TextureType::CUBE_MAP:
			_WriteAttrString( ATTR_FILE_PATH_POSX, _target->GetImagePath(0) );
			_WriteAttrString( ATTR_FILE_PATH_NEGX, _target->GetImagePath(1) );
			_WriteAttrString( ATTR_FILE_PATH_POSY, _target->GetImagePath(2) );
			_WriteAttrString( ATTR_FILE_PATH_NEGY, _target->GetImagePath(3) );
			_WriteAttrString( ATTR_FILE_PATH_POSZ, _target->GetImagePath(4) );
			_WriteAttrString( ATTR_FILE_PATH_NEGZ, _target->GetImagePath(5) );
			break;
	}

	_ChangeVisibility();
}


void AttrViewer_Texture::_OnUpdateTarget()
{
	_target->SetEmitEvents( FALSE );

	_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );

	if ( _activatedGroup == CallbackGroup::GROUP_2 )
	{
		TextureType type;
		Int typeIndex = _ReadAttrDropdown( ATTR_TYPE );
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
		_target->SetHasMipmap( _ReadAttrBool( ATTR_HAS_MIPMAP ) );
		_target->SetClearBufferOnBind( _ReadAttrBool( ATTR_CLEAR_BUFFER ) );

		switch ( _target->GetTextureType() )
		{
			case TextureType::TEXTURE_2D:
				_target->SetImagePath( 0, _ReadAttrString( ATTR_FILE_PATH ).GetChar() );
				break;

			case TextureType::CUBE_MAP:
				_target->SetImagePath( 0, _ReadAttrString( ATTR_FILE_PATH_POSX ).GetChar() );
				_target->SetImagePath( 1, _ReadAttrString( ATTR_FILE_PATH_NEGX ).GetChar() );
				_target->SetImagePath( 2, _ReadAttrString( ATTR_FILE_PATH_POSY ).GetChar() );
				_target->SetImagePath( 3, _ReadAttrString( ATTR_FILE_PATH_NEGY ).GetChar() );
				_target->SetImagePath( 4, _ReadAttrString( ATTR_FILE_PATH_POSZ ).GetChar() );
				_target->SetImagePath( 5, _ReadAttrString( ATTR_FILE_PATH_NEGZ ).GetChar() );
				break;
		}
	}

	_target->SetEmitEvents( TRUE );

	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::ASSET, EventName::ASSET_CHANGED, _target );
}


void AttrViewer_Texture::_ChangeVisibility()
{
	switch ( _target->GetTextureType() )
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

