// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredModule_Event.h"

#include "Viewport_WithGizmo.h"

namespace CYRED
{
	class GameObject;
}


namespace CYRED
{
	class Viewport_Isolate : public Viewport_WithGizmo, public IEventListener
	{
		cchar* const PANEL_TITLE = "Isolate Viewport";


	public:
		Viewport_Isolate( int panelIndex );
		virtual ~Viewport_Isolate() {}


	public:
		void OnEvent( int eventType, void* eventData )	override;


	private:
		virtual cchar*	_GetPanelTitle	()					override;
		virtual void	_OnInitialize	()					override;
		virtual void	_OnFinalize		()					override;
		virtual void	_OnUpdate		( bool isRuntime )	override;


	private:
		GameObject*	_target;
		int			_openEventType;


	private:
		bool _IsPickingInput();
	};
}