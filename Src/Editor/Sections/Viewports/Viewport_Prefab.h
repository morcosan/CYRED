// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Event.h"

#include "Viewport_WithGizmo.h"


namespace CYRED
{
	class GameObject;
	class Prefab;
}


namespace CYRED
{
	class Viewport_Prefab : public Viewport_WithGizmo, public IEventListener
	{
		cchar* const PANEL_TITLE = "Prefab Viewport";


	public:
		Viewport_Prefab( int panelIndex );
		virtual ~Viewport_Prefab() {}


	public:
		void OnEvent( EventType eType, void* eData )	override;


	private:
		virtual cchar*	_GetPanelTitle	() override;
		virtual void	_OnInitialize	() override;
		virtual void	_OnFinalize		() override;
		virtual void	_OnUpdate		() override;


	private:
		Prefab*		_targetPrefab;


	private:
		bool _IsPickingInput();
	};
}