// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		const char*		PANEL_TITLE = "Scene Viewport";
		const Vector2	MIN_SIZE	= Vector2( 400, 200 );


	public:
		SceneViewport( int panelIndex );
		virtual ~SceneViewport() {}


	public:
		void SetCamera( GameObject* cameraGO );


	public:
		void A_CameraButton();


	private:
		static DataMap<TechniqueType, int>	_techSlots;


	private:
		QComboBox*		_qtCameraDropdown;
		QPushButton*	_qtCameraButton;


	private:
		virtual const char*	_GetPanelTitle	() override;
		virtual Vector2		_GetPanelMinSize() override;
		virtual void		_OnInitialize	() override;
		virtual void		_OnFinalize		() override;
		virtual void		_OnUpdate		() override;


	private:
		void _RecCollectLights	( GameObject* gameObject, DataArray<GameObject*>& lightsGO );
	};
}