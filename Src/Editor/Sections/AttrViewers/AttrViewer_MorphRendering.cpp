// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#include "AttrViewer_MorphRendering.h"
#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Event.h"
#include "../Selectors/Selector_Material.h"
#include "../Selectors/Selector_Morph.h"


using namespace CYRED;



void AttrViewer_MorphRendering::_OnInitialize()
{
	_CreateAttrSelector( ATTR_MORPH, ATTR_MORPH,	Selector_Morph::TYPE,	
						 AttrFlag::NONE, CallbackGroup::GROUP_2 );
	_CreateAttrSelector( ATTR_MATERIAL, ATTR_MATERIAL, Selector_Material::TYPE );

	_CreateAttrFloat( ATTR_DUR_STATE,			ATTR_DUR_STATE );
	_CreateAttrFloat( ATTR_DUR_STATE_CHANGE,	ATTR_DUR_STATE_CHANGE );
	_CreateAttrBool	( ATTR_IS_PLAYING,			ATTR_IS_PLAYING );
	
	_CreateInnerAttribute( InnerAttrType::ENABLED );
	_CreateInnerAttribute( InnerAttrType::SETTINGS );

	_AddToPanel( TITLE );
}


void AttrViewer_MorphRendering::_OnChangeTarget( void* target )
{
	_target = CAST_S( MorphRendering*, target );

	// prepare settings
	DataUnion attr;
	_WriteInnerAttribute( InnerAttrType::SETTINGS, attr.SetReference( _target ) );
}


void AttrViewer_MorphRendering::_UpdateGUI()
{
	Morph* morph = _target->GetMorph();
	cchar* morphName = (morph == NULL) ? Selector_Morph::OPTION_NULL : morph->GetName();
	_WriteAttrSelector( ATTR_MORPH, morph, morphName );

	Material* material = _target->GetMaterial();
	cchar* matName = (material == NULL) ? Selector_Material::OPTION_NULL : material->GetName();
	_WriteAttrSelector( ATTR_MATERIAL, material, matName );

	_WriteAttrFloat	( ATTR_DUR_STATE,			_target->GetDurationState() );
	_WriteAttrFloat	( ATTR_DUR_STATE_CHANGE,	_target->GetDurationStateChange() );
	_WriteAttrBool	( ATTR_IS_PLAYING,			_target->IsPlaying() );

	if ( _target->IsEnabled() != _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() )
	{
		DataUnion attr;
		_WriteInnerAttribute( InnerAttrType::ENABLED, attr.SetBool( _target->IsEnabled() ) );

		_Colorize( _target->IsEnabled() );
	}
}


void AttrViewer_MorphRendering::_UpdateTarget()
{
	_target->SetEmitEvents( FALSE );

	if ( _activatedGroup == CallbackGroup::GROUP_2 )
	{
		_target->SetMorph( CAST_S( Morph*, _ReadAttrSelector( ATTR_MORPH ) ) );
	}
	_target->SetMaterial( CAST_S( Material*, _ReadAttrSelector( ATTR_MATERIAL ) ) );

	_target->SetDurationState		( _ReadAttrFloat( ATTR_DUR_STATE ) );
	_target->SetDurationStateChange	( _ReadAttrFloat( ATTR_DUR_STATE_CHANGE ) );
	_target->SetIsPlaying			( _ReadAttrBool( ATTR_IS_PLAYING ) );

	_target->SetEnabled( _ReadInnerAttribute( InnerAttrType::ENABLED ).GetBool() );

	_target->SetEmitEvents( TRUE );

	// emit event manually
	++_ignoreUpdateGUI;
	EventManager::Singleton()->EmitEvent( EventType::COMPONENT_UPDATE, _target );

	_Colorize( _target->IsEnabled() );
}
