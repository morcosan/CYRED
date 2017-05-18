// Copyright (c) 2015 Morco (www.morco.ro)
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
		const char*		PANEL_TITLE = "Prefab Viewport";
		const Vector2	MIN_SIZE	= Vector2( 400, 200 );


	public:
		PrefabViewport( int panelIndex );
		virtual ~PrefabViewport() {}


	public:
		void OnEvent( EventType eType, void* eData )	override;


	public:
		void SetCamera( GameObject* cameraGO );


	protected:
		static DataMap<TechniqueType, int>	_techSlots;


	protected:
		Prefab* _targetPrefab;


	protected:
		virtual const char*	_GetPanelTitle	() override;
		virtual Vector2		_GetPanelMinSize() override;
		virtual void		_OnInitialize	() override;
		virtual void		_OnFinalize		() override;
		virtual void		_OnUpdate		() override;
	};
}