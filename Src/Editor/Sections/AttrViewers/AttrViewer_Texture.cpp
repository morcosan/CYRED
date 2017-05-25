// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Texture.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"


using namespace CYRED;



void AttrViewer_Texture::_OnInitialize()
{
	_CreateAttrString	( ATTR_NAME, ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	DataArray<cchar*> textureTypes;
	textureTypes.Add( TYPE_TEXTURE_2D );
	textureTypes.Add( TYPE_CUBE_MAP );
	_CreateAttrDropdown	( ATTR_TEXTURE_TYPE, ATTR_TEXTURE_TYPE, textureTypes, AttrFlag::NONE, 
						  CallbackGroup::GROUP_2 );

	DataArray<cchar*> loadTypes;
	loadTypes.Add( TYPE_SCRIPTED );
	loadTypes.Add( TYPE_EXTERNAL );
	_CreateAttrDropdown	( ATTR_LOAD_TYPE, ATTR_LOAD_TYPE, loadTypes );

	_CreateAttrBool		( ATTR_HAS_MIPMAP,		ATTR_HAS_MIPMAP );
	_CreateAttrBool		( ATTR_CLEAR_BUFFER,	ATTR_CLEAR_BUFFER );
	_CreateAttrString	( ATTR_FILE_PATH,		ATTR_FILE_PATH );
	_CreateAttrString	( ATTR_FILE_PATH_POSX,	ATTR_FILE_PATH_POSX );
	_CreateAttrString	( ATTR_FILE_PATH_NEGX,	ATTR_FILE_PATH_NEGX );
	_CreateAttrString	( ATTR_FILE_PATH_POSY,	ATTR_FILE_PATH_POSY );
	_CreateAttrString	( ATTR_FILE_PATH_NEGY,	ATTR_FILE_PATH_NEGY );
	_CreateAttrString	( ATTR_FILE_PATH_POSZ,	ATTR_FILE_PATH_POSZ );
	_CreateAttrString	( ATTR_FILE_PATH_NEGZ,	ATTR_FILE_PATH_NEGZ );
	
	_AddToPanel( TITLE );
}


void AttrViewer_Texture::_OnChangeTarget( void* target )
{
	_target = CAST_S( Texture*, target );

	// reset colorize
	_Colorize( TRUE, TRUE );
}


void AttrViewer_Texture::_UpdateGUI()
{
	_WriteAttrString( ATTR_NAME, _target->GetName() );

	{
		int typeIndex = 0;
		switch ( _target->GetTextureType() ) {
			case TextureType::TEXTURE_2D:	typeIndex = 0;	break;
			case TextureType::CUBE_MAP:		typeIndex = 1;	break;
		}
		_WriteAttrDropdown( ATTR_TEXTURE_TYPE, typeIndex );
	}
	{
		int typeIndex = 0;
		switch ( _target->GetLoadType() ) {
			case TextureLoadType::SCRIPTED:	typeIndex = 0;	break;
			case TextureLoadType::EXTERNAL:	typeIndex = 1;	break;
		}
		_WriteAttrDropdown( ATTR_LOAD_TYPE, typeIndex );
	}

	_WriteAttrBool( ATTR_HAS_MIPMAP, _target->HasMipmap() );
	_WriteAttrBool( ATTR_CLEAR_BUFFER, _target->DoesClearBufferOnBind() );

	switch ( _target->GetTextureType() ) {
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

	// change ui based on options
	_ChangeVisibility();
}


void AttrViewer_Texture::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );

	_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );

	if ( _activatedGroup == CallbackGroup::GROUP_2 ) {
		TextureType textureType;
		int typeIndex = _ReadAttrDropdown( ATTR_TEXTURE_TYPE );
		switch ( typeIndex ) {
			case 0:	textureType = TextureType::TEXTURE_2D;	break;
			case 1:	textureType = TextureType::CUBE_MAP;	break;
		}
		_target->SetTextureType( textureType );

		// change ui based on options
		_ChangeVisibility();
	}
	else {
		TextureLoadType loadType;
		int typeIndex = _ReadAttrDropdown( ATTR_LOAD_TYPE );
		switch ( typeIndex ) {
			case 0:	loadType = TextureLoadType::SCRIPTED;	break;
			case 1:	loadType = TextureLoadType::EXTERNAL;	break;
		}
		_target->SetLoadType( loadType );

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
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, _target );
}


void AttrViewer_Texture::_ChangeVisibility()
{
	switch ( _target->GetTextureType() ) {
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

