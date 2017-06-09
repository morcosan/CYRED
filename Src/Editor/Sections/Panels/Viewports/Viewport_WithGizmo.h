// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"

#include "../Panel_Viewport.h"

namespace CYRED
{
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
		cchar* const	GIZMO_COLL_BOX		= "GizmoCollisionBox";
		cchar* const	GIZMO_COLL_SPHERE	= "GizmoCollisionSphere";
		cchar* const	GIZMO_DEBUG_TEXT	= "GizmoDebugText";


	public:
		Viewport_WithGizmo( int panelIndex );
		virtual ~Viewport_WithGizmo() {}


	public:
		void LoadGizmo	() override;


	public:
		void SetCamera( GameObject* cameraGO );


	protected:
		DataArray<GameObject*>	_noLightsGO;	// empty lights list

		GameObject*	_selectedGO;
		GameObject*	_gizmoGrid;
		GameObject*	_gizmoAxis;
		GameObject*	_gizmoBackground;
		GameObject*	_gizmoPointLight;
		GameObject*	_gizmoDirLight;
		GameObject*	_gizmoSpotLight;
		GameObject*	_gizmoOrthoCamera;
		GameObject*	_gizmoPerspCamera;
		GameObject*	_gizmoPivot;
		GameObject*	_gizmoCollBox;
		GameObject*	_gizmoCollSphere;
		GameObject*	_gizmoDebugText;


	protected:
		bool		_IsRenderingReady	();
		void		_RecCollectLights	( GameObject* root, DataArray<GameObject*>& lightsGO );
		void		_RenderGizmo		();
		void		_RenderGizmoAfter	();
		GameObject*	_RecSearchByUID		( int uid, GameObject* root );
		bool		_RecIsFound			( GameObject* target, GameObject* root );
		bool		_IsPickingInput		( Node* root, int selectEvent );
	};
}