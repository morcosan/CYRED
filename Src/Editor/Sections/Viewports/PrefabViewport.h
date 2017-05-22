// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Render.h"
#include "CyredModule_Event.h"

#include "../Panels/Panel_Viewport.h"


namespace CYRED
{
	class Prefab;
}


namespace CYRED
{
	class PrefabViewport : public Panel_Viewport, public IEventListener
	{
	public:
		const char* const	PANEL_TITLE			= "Prefab Viewport";
		const Vector2		MIN_SIZE			= Vector2( 400, 200 );
		const char* const	GIZMO_GRID			= "GizmoGrid";
		const char* const	GIZMO_AXIS			= "GizmoAxis";
		const char* const	GIZMO_BACKGROUND	= "GizmoBackground";
		const char* const	GIZMO_POINT_LIGHT	= "GizmoPointLight";
		const char* const	GIZMO_DIR_LIGHT		= "GizmoDirLight";
		const char* const	GIZMO_SPOT_LIGHT	= "GizmoSpotLight";


	public:
		PrefabViewport( int panelIndex );
		virtual ~PrefabViewport() {}


	public:
		void OnEvent	( EventType eType, void* eData )	override;
		void LoadGizmo	()									override;


	public:
		void SetCamera( GameObject* cameraGO );


	private:
		static DataMap<TechniqueType, int>	_techSlots;


	private:
		Prefab*		_targetPrefab;
		GameObject*	_selectedGO;
		Prefab*		_gizmoGrid;
		Prefab*		_gizmoAxis;
		Prefab*		_gizmoPointLight;
		Prefab*		_gizmoDirLight;
		Prefab*		_gizmoSpotLight;
		Prefab*		_gizmoBackground;


	private:
		virtual const char*	_GetPanelTitle	() override;
		virtual Vector2		_GetPanelMinSize() override;
		virtual void		_OnInitialize	() override;
		virtual void		_OnFinalize		() override;
		virtual void		_OnUpdate		() override;


	private:
		void _RecCollectLights	( GameObject* gameObject, DataArray<GameObject*>& lightsGO );
	};
}