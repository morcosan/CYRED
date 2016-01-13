// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredModule_Event.h"
#include "CyredBuildingBlocks.h"
#include "../Panel.h"


class QTreeWidget;
class QPushButton;


namespace CYRED
{
	class AttrViewer;


	class AttributePanel : public Panel, public EventListener
	{
		const Char* const	ATTR_GAMEOBJECT			= "GameObject";
		const Char* const	ATTR_TRANSFORM			= "Transform";
		const Char* const	ATTR_CAMERA				= "Camera";
		const Char* const	ATTR_PARTICLES_EMITTER	= "Particle Emitter";
		const Char* const	ATTR_MATERIAL			= "Material";
		const Char* const	ATTR_MESH				= "Mesh";
		const Char* const	ATTR_TEXTURE			= "Texture";
		const Char* const	ATTR_SHADER				= "Shader";
		const Char* const	ATTR_SCENE				= "Scene";

		const Char*	const	PANEL_TITLE				= "Attributes Panel";
		const Char*	const	ADD_COMPONENT_BUTTON	= "Add Component";
		const Vector2 		MIN_SIZE				= Vector2( 300, 250 );


	public:
		AttributePanel();
		virtual ~AttributePanel();


	public:
		void Initialize	()	override;

		void OnEvent	( EventType eType, EventName eName, void* eSource )	override;


	public:
		//! it will take ownership of the viewers
		void SetAttrViewer( const Char* typeName, AttrViewer* viewer );


	protected:
		DataMap<String, AttrViewer*>	_attrViewers;

		QTreeWidget*		_qtTree;
		QPushButton*		_qtCompButton;
		void*				_target;

		void _Clear();
	};
}