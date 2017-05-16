// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredModule_Render.h"
#include "../Panels/Panel_Viewport.h"


class QHBoxLayout;
class QComboBox;
class QPushButton;


namespace CYRED
{
	class PrefabViewport : public Panel_Viewport
	{
	public:
		const Char*		PANEL_TITLE = "Prefab Viewport";
		const Vector2	MIN_SIZE	= Vector2( 400, 200 );


	public:
		PrefabViewport( UInt panelIndex );
		virtual ~PrefabViewport() {}


	public:
		void SetCamera( GameObject* cameraGO );


	protected:
		static DataMap<TechniqueType, UInt>	_techSlots;

		static UInt	GetSlotForTechnique( TechniqueType type );


	protected:
		virtual const Char*	_GetPanelTitle	() override;
		virtual Vector2		_GetPanelMinSize() override;
		virtual void		_OnInitialize	() override;
		virtual void		_OnUpdate		() override;
	};
}