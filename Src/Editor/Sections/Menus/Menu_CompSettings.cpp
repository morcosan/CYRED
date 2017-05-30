// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Menu_CompSettings.h"

#include "CyredBuildingBlocks.h"
#include "CyredModule_Render.h"
#include "CyredModule_Script.h"

#include "QtWidgets\qwidget.h"


using namespace CYRED;


Menu_CompSettings::Menu_CompSettings( QWidget* qtParent )
	: QMenu( qtParent )
	, _qtParent( qtParent )
{
	QObject::connect( this,	&QMenu::aboutToShow, this, &Menu_CompSettings::A_ShowMenu );
}


void Menu_CompSettings::Open( Component* target )
{
	// store target
	_target = target;
}


void Menu_CompSettings::A_ShowMenu()
{
	// create custom menu
	this->clear();

	// add actions
	QAction* actionRemove = this->addAction( MENU_REMOVE );
	QObject::connect( actionRemove,	&QAction::triggered, this, &Menu_CompSettings::A_Remove );
}


void Menu_CompSettings::A_Remove()
{
	// get gameobject
	GameObject* gameObject = _target->GetGameObject();

	switch ( _target->GetComponentType() ) {
		case ComponentType::TRANSFORM:
			gameObject->RemoveComponent<Transform>();
			break;

		case ComponentType::CAMERA:
			gameObject->RemoveComponent<Camera>();
			break;

		case ComponentType::LIGHT:
			gameObject->RemoveComponent<Light>();
			break;

		case ComponentType::MESH_RENDERING:
			gameObject->RemoveComponent<MeshRendering>();
			break;

		case ComponentType::MORPH_RENDERING:
			gameObject->RemoveComponent<MorphRendering>();
			break;

		case ComponentType::SCRIPTER:
			gameObject->RemoveComponent<Scripter>();
			break;

		case ComponentType::PARTICLE_EMITTER:
			gameObject->RemoveComponent<ParticleEmitter>();
			break;
	}
}

