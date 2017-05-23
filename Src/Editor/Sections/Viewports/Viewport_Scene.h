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
	public:
		Viewport_Scene( int panelIndex );
		virtual ~Viewport_Scene() {}


	public:
		void OnEvent	( EventType eType, void* eData )	override;


	private:
		QComboBox*		_qtCameraDropdown;


	private:
		virtual void _OnInitialize	() override;
		virtual void _OnFinalize	() override;
		virtual void _OnUpdate		() override;
	};
}