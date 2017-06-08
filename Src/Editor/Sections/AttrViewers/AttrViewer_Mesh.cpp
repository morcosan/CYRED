// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "AttrViewer_Mesh.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"


using namespace CYRED;



void AttrViewer_Mesh::_OnInitialize()
{
	_CreateAttrString	( ATTR_NAME, ATTR_NAME, AttrFlag::EDIT_FINISH, CallbackGroup::GROUP_1 );

	DataArray<cchar*> meshTypes;
	meshTypes.Add( MESH_TYPE_LINE );
	meshTypes.Add( MESH_TYPE_POLYGON );
	_CreateAttrDropdown	( ATTR_MESH_TYPE, ATTR_MESH_TYPE, meshTypes, AttrFlag::NONE, CallbackGroup::GROUP_1 );

	DataArray<cchar*> loadTypes;
	loadTypes.Add( LOAD_TYPE_EXTERNAL );
	loadTypes.Add( LOAD_TYPE_SCRIPTED );
	_CreateAttrDropdown	( ATTR_LOAD_TYPE, ATTR_LOAD_TYPE, loadTypes, AttrFlag::NONE, CallbackGroup::GROUP_2 );

	_CreateAttrString	( ATTR_FILE_PATH, ATTR_FILE_PATH );

	_CreateAttrBool		( ATTR_CLEAR_BUFFER, ATTR_CLEAR_BUFFER );
	
	_AddToPanel( TITLE );
}


void AttrViewer_Mesh::_OnChangeTarget( void* target )
{
	_target = CAST_S( Mesh*, target );

	// reset colorize
	_Colorize( TRUE, TRUE );
}


void AttrViewer_Mesh::_UpdateGUI()
{
	_WriteAttrString( ATTR_NAME, _target->GetName() );

	{
		int typeIndex = 0;
		switch ( _target->GetMeshType() ) {
			case MeshType::LINE:		typeIndex = 0;	break;
			case MeshType::POLYGON:		typeIndex = 1;	break;
		}
		_WriteAttrDropdown( ATTR_MESH_TYPE, typeIndex );
	}
	{
		int typeIndex = 0;
		switch ( _target->GetLoadType() ) {
			case MeshLoadType::EXTERNAL:	typeIndex = 0;	break;
			case MeshLoadType::SCRIPTED:	typeIndex = 1;	break;		
		}
		_WriteAttrDropdown( ATTR_LOAD_TYPE, typeIndex );
	}

	_WriteAttrBool( ATTR_CLEAR_BUFFER, _target->DoesClearBuffersOnBind() );

	switch ( _target->GetLoadType() ) {
		case MeshLoadType::EXTERNAL:
		case MeshLoadType::SCRIPTED:
			_WriteAttrString( ATTR_FILE_PATH, _target->GetExternalPath() );
			break;
	}
}


void AttrViewer_Mesh::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );

	_target->SetName( _ReadAttrString( ATTR_NAME ).GetChar() );

	if ( _activatedGroup == CallbackGroup::GROUP_2 ) {
		int typeIndex = _ReadAttrDropdown( ATTR_LOAD_TYPE );
		switch ( typeIndex ) {
			case 0:	_target->SetLoadType( MeshLoadType::EXTERNAL );	break;
			case 1:	_target->SetLoadType( MeshLoadType::SCRIPTED );	break;
		}
	}
	else {
		_target->SetClearBuffersOnBind( _ReadAttrBool( ATTR_CLEAR_BUFFER ) );

		int typeIndex = _ReadAttrDropdown( ATTR_MESH_TYPE );
		switch ( typeIndex ) {
			case 0:	_target->SetMeshType( MeshType::LINE );		break;
			case 1:	_target->SetMeshType( MeshType::POLYGON );	break;
		}

		switch ( _target->GetLoadType() ) {
			case MeshLoadType::EXTERNAL:
			case MeshLoadType::SCRIPTED:
				_target->SetExternalPath( _ReadAttrString( ATTR_FILE_PATH ).GetChar() );
				break;
		}
	}

	_target->SetEmitEvents( TRUE );

	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, _target );
}
