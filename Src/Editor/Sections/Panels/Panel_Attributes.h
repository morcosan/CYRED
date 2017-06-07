// Copyright (c) 2015-2017 Morco (www.morco.ro)
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


	class Panel_Attributes : public Panel, public IEventListener
	{
		cchar* const	ATTR_GAMEOBJECT			= "GameObject";
		cchar* const	ATTR_TRANSFORM			= "Transform";
		cchar* const	ATTR_CAMERA				= "Camera";
		cchar* const	ATTR_LIGHT				= "Light";
		cchar* const	ATTR_PARTICLES_EMITTER	= "ParticleEmitter";
		cchar* const	ATTR_MESH_RENDERING		= "MeshRendering";
		cchar* const	ATTR_MORPH_RENDERING	= "MorphRendering";
		cchar* const	ATTR_SCRIPTER			= "Scripter";
		cchar* const	ATTR_RIGID_BODY			= "RigidBody";
		cchar* const	ATTR_MATERIAL			= "Material";
		cchar* const	ATTR_MESH				= "Mesh";
		cchar* const	ATTR_MORPH				= "Morph";
		cchar* const	ATTR_TEXTURE			= "Texture";
		cchar* const	ATTR_SCRIPT				= "Script";
		cchar* const	ATTR_PREFAB				= "Prefab";
		cchar* const	ATTR_SHADER				= "Shader";
		cchar* const	ATTR_SCENE				= "Scene";
		cchar* const	ATTR_FONT				= "Font";
		cchar* const	ATTR_CYRED_PROJ			= "CyredProj";

		cchar*	const	PANEL_TITLE				= "Attributes";
		cchar*	const	ADD_COMPONENT_BUTTON	= "Add Component";
		const Vector2 	MIN_SIZE				= Vector2( 300, 100 );


	public:
		Panel_Attributes();
		virtual ~Panel_Attributes();


	public:
		void Initialize	()									override;
		void Finalize	()									override;
		void Update		( bool isRuntime )					override;
		void OnEvent	( int eventType, void* eventData )	override;


	public:
		//! it will take ownership of the viewers
		void SetAttrViewer	( cchar* typeName, AttrViewer* viewer );
		void RefreshPanel	();


	private:
		struct _AttrViewer
		{
			bool		needsRefresh;
			AttrViewer* viewer;
		};

		DataMap<String, _AttrViewer*>	_attrViewers;

		QTreeWidget*	_qtTree;
		void*			_target;
		bool			_needsRefresh;


	private:
		void _ClearPanel		();
		void _DisplayGameObject	( GameObject* gameObject );
	};
}