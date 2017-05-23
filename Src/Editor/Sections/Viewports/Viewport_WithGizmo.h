// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"

#include "../Panels/Panel_Viewport.h"

namespace CYRED
{
	class GameObject;
	class Prefab;
}


namespace CYRED
{
	ABSTRACT class Viewport_WithGizmo : public Panel_Viewport
	{
	public:
		const char*	const	PANEL_TITLE			= "Scene Viewport";
		const Vector2		MIN_SIZE			= Vector2( 400, 250 );
		const char* const	GIZMO_GRID			= "GizmoGrid";
		const char* const	GIZMO_AXIS			= "GizmoAxis";
		const char* const	GIZMO_BACKGROUND	= "GizmoBackground";
		const char* const	GIZMO_POINT_LIGHT	= "GizmoPointLight";
		const char* const	GIZMO_DIR_LIGHT		= "GizmoDirLight";
		const char* const	GIZMO_SPOT_LIGHT	= "GizmoSpotLight";
		const char* const	GIZMO_ORTHO_CAMERA	= "GizmoOrthoCamera";
		const char* const	GIZMO_PERSP_CAMERA	= "GizmoPerspCamera";
		const char* const	GIZMO_PIVOT			= "GizmoPivot";


	public:
		Viewport_WithGizmo( int panelIndex );
		virtual ~Viewport_WithGizmo() {}


	public:
		void LoadGizmo	() override;


	public:
		void SetCamera( GameObject* cameraGO );


	protected:
		GameObject*		_selectedGO;
		Prefab*			_gizmoGrid;
		Prefab*			_gizmoAxis;
		Prefab*			_gizmoBackground;
		Prefab*			_gizmoPointLight;
		Prefab*			_gizmoDirLight;
		Prefab*			_gizmoSpotLight;
		Prefab*			_gizmoOrthoCamera;
		Prefab*			_gizmoPerspCamera;
		Prefab*			_gizmoPivot;


	private:
		virtual const char*	_GetPanelTitle	() override;
		virtual Vector2		_GetPanelMinSize() override;


	protected:
		bool _IsRenderingReady	();
		void _RecCollectLights	( GameObject* gameObject, DataArray<GameObject*>& lightsGO );
		void _RenderGizmo		();
	};
}