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
	class SceneViewport : public Panel_Viewport
	{
	public:
		const Char*		PANEL_TITLE = "Scene";
		const Vector2	MIN_SIZE	= Vector2( 200, 100 );


	public:
		SceneViewport( UInt panelIndex );
		virtual ~SceneViewport() {}

		// TODO
		void SetCamera( GameObject* cameraGO )
		{
			_cameraGO = cameraGO;
		}


	public:
		void A_CameraButton();


	protected:
		static DataMap<TechniqueType, UInt>	_techSlots;

		static UInt	GetSlotForTechnique( TechniqueType type );


	protected:
		QComboBox*		_qtCameraDropdown;
		QPushButton*	_qtCameraButton;

		virtual const Char*	_GetPanelTitle	() override;
		virtual Vector2		_GetPanelMinSize() override;
		virtual void		_OnInitialize	() override;
		virtual void		_OnUpdate		() override;
	};
}