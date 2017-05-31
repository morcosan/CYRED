// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Event.h"

#include "Viewport_WithGizmo.h"

class QComboBox;

namespace CYRED
{
	class Prefab;
}


namespace CYRED
{
	class Viewport_Scene : public Viewport_WithGizmo, public IEventListener
	{
		cchar* const PANEL_TITLE = "Scene Viewport";


	public:
		Viewport_Scene( int panelIndex );
		virtual ~Viewport_Scene() {}


	public:
		void OnEvent	( int eventType, void* eventData )	override;


	private:
		virtual cchar*	_GetPanelTitle	() override;
		virtual void	_OnInitialize	() override;
		virtual void	_OnFinalize		() override;
		virtual void	_OnUpdate		() override;

	private:
		QComboBox*		_qtCameraDropdown;


	private:
		bool _IsPickingInput();

	};
}