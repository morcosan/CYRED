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


	class Panel_Attributes : public Panel, public IEventListener
	{
		const char* const	ATTR_GAMEOBJECT			= "GameObject";
		const char* const	ATTR_TRANSFORM			= "Transform";
		const char* const	ATTR_CAMERA				= "Camera";
		const char* const	ATTR_LIGHT				= "Light";
		const char* const	ATTR_PARTICLES_EMITTER	= "ParticleEmitter";
		const char* const	ATTR_MESH_RENDERING		= "MeshRendering";
		const char* const	ATTR_MORPH_RENDERING	= "MorphRendering";
		const char* const	ATTR_SCRIPTER			= "Scripter";
		const char* const	ATTR_MATERIAL			= "Material";
		const char* const	ATTR_MESH				= "Mesh";
		const char* const	ATTR_MORPH				= "Morph";
		const char* const	ATTR_TEXTURE			= "Texture";
		const char* const	ATTR_SCRIPT				= "Script";
		const char* const	ATTR_PREFAB				= "Prefab";
		const char* const	ATTR_SHADER				= "Shader";
		const char* const	ATTR_SCENE				= "Scene";
		const char* const	ATTR_CYRED_PROJ			= "CyredProj";

		const char*	const	PANEL_TITLE				= "Attributes";
		const char*	const	ADD_COMPONENT_BUTTON	= "Add Component";
		const Vector2 		MIN_SIZE				= Vector2( 300, 100 );


	public:
		Panel_Attributes();
		virtual ~Panel_Attributes();


	public:
		void Initialize	()	override;
		void Finalize	()	override;
		void Update		()	override;
		void OnEvent	( EventType eType, void* eData )	override;


	public:
		//! it will take ownership of the viewers
		void SetAttrViewer	( const char* typeName, AttrViewer* viewer );
		void RefreshPanel	();


	protected:
		struct _AttrViewer
		{
			bool		needsRefresh;
			AttrViewer* viewer;
		};

		DataMap<String, _AttrViewer*>	_attrViewers;

		QTreeWidget*	_qtTree;
		void*			_target;
		bool			_needsRefresh;


		void _ClearPanel		();
		void _DisplayGameObject	( GameObject* gameObject );
	};
}