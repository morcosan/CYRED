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


	class AttributePanel : public Panel, public IEventListener
	{
		const Char* const	ATTR_GAMEOBJECT			= "GameObject";
		const Char* const	ATTR_TRANSFORM			= "Transform";
		const Char* const	ATTR_CAMERA				= "Camera";
		const Char* const	ATTR_PARTICLES_EMITTER	= "ParticleEmitter";
		const Char* const	ATTR_MESH_RENDERING		= "MeshRendering";
		const Char* const	ATTR_MORPH_RENDERING	= "MorphRendering";
		const Char* const	ATTR_SCRIPTER			= "Scripter";
		const Char* const	ATTR_MATERIAL			= "Material";
		const Char* const	ATTR_MESH				= "Mesh";
		const Char* const	ATTR_MORPH				= "Morph";
		const Char* const	ATTR_TEXTURE			= "Texture";
		const Char* const	ATTR_SCRIPT				= "Script";
		const Char* const	ATTR_SHADER				= "Shader";
		const Char* const	ATTR_SCENE				= "Scene";
		const Char* const	ATTR_CYRED_PROJ			= "CyredProj";

		const Char*	const	PANEL_TITLE				= "Attributes Panel";
		const Char*	const	ADD_COMPONENT_BUTTON	= "Add Component";
		const Vector2 		MIN_SIZE				= Vector2( 300, 250 );


	public:
		AttributePanel();
		virtual ~AttributePanel();


	public:
		void Initialize	()	override;
		void Finalize	()	override;
		void Update		()	override;
		void OnEvent	( EventType eType, void* eData )	override;


	public:
		//! it will take ownership of the viewers
		void SetAttrViewer	( const Char* typeName, AttrViewer* viewer );
		void RefreshPanel	();


	protected:
		struct _AttrViewer
		{
			Bool		needsRefresh;
			AttrViewer* viewer;
		};

		DataMap<String, _AttrViewer*>	_attrViewers;

		QTreeWidget*	_qtTree;
		void*			_target;
		Bool			_needsRefresh;

		void _ClearPanel		();
		void _DisplayGameObject	();
	};
}