// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Font.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"


using namespace CYRED;


void AttrViewer_Font::_OnInitialize()
{
	_CreateAttrString	( ATTR_NAME, ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );
	_CreateAttrString	( ATTR_FILE_PATH, ATTR_FILE_PATH );
	_CreateAttrInt		( ATTR_MAX_SIZE, ATTR_MAX_SIZE, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	_AddToPanel( TITLE );
}


void AttrViewer_Font::_OnChangeTarget( void* target )
{
	_target = CAST_S( Font*, target );

	// reset colorize
	_Colorize( TRUE, TRUE );
}


void AttrViewer_Font::_UpdateGUI()
{
	_WriteAttrString( ATTR_NAME,		_target->GetName() );
	_WriteAttrString( ATTR_FILE_PATH,	_target->GetExternalPath() );
	_WriteAttrInt	( ATTR_MAX_SIZE,	_target->GetMaxSize() );
}


void AttrViewer_Font::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );
	{
		_target->SetName		( _ReadAttrString( ATTR_NAME ).GetChar() );
		_target->SetExternalPath( _ReadAttrString( ATTR_FILE_PATH ).GetChar() );
		_target->SetMaxSize		( _ReadAttrInt( ATTR_MAX_SIZE ) );
	}
	_target->SetEmitEvents( TRUE );

	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, _target );
}
