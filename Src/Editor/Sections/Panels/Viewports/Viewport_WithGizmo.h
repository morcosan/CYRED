// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"

#include "../Panel_Viewport.h"

namespace CYRED
{
	class GameObject;
	class Prefab;
}


namespace CYRED
{
	ABSTRACT class Viewport_WithGizmo : public Panel_Viewport
	{
		cchar* const	GIZMO_GRID			= "GizmoGrid";
		cchar* const	GIZMO_AXIS			= "GizmoAxis";
		cchar* const	GIZMO_BACKGROUND	= "GizmoBackground";
		cchar* const	GIZMO_POINT_LIGHT	= "GizmoPointLight";
		cchar* const	GIZMO_DIR_LIGHT		= "GizmoDirLight";
		cchar* const	GIZMO_SPOT_LIGHT	= "GizmoSpotLight";
		cchar* const	GIZMO_ORTHO_CAMERA	= "GizmoOrthoCamera";
		cchar* const	GIZMO_PERSP_CAMERA	= "GizmoPerspCamera";
		cchar* const	GIZMO_PIVOT			= "GizmoPivot";


	public:
		Viewport_WithGizmo( int panelIndex );
		virtual ~Viewport_WithGizmo() {}


	public:
		void LoadGizmo	() override;


	public:
		void SetCamera( GameObject* cameraGO );


	protected:
		DataArray<GameObject*>	_noLightsGO;	// empty lights list

		GameObject*				_selectedGO;
		Prefab*					_gizmoGrid;
		Prefab*					_gizmoAxis;
		Prefab*					_gizmoBackground;
		Prefab*					_gizmoPointLight;
		Prefab*					_gizmoDirLight;
		Prefab*					_gizmoSpotLight;
		Prefab*					_gizmoOrthoCamera;
		Prefab*					_gizmoPerspCamera;
		Prefab*					_gizmoPivot;


	protected:
		bool		_IsRenderingReady	();
		void		_RecCollectLights	( GameObject* root, DataArray<GameObject*>& lightsGO );
		void		_RenderGizmoBefore	();
		void		_RenderGizmoAfter	();
		GameObject*	_RecSearchByUID		( int uid, GameObject* root );
		bool		_RecIsFound			( GameObject* target, GameObject* root );
	};
}