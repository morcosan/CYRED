// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"

#include "../Panels/Panel_Viewport.h"


namespace CYRED
{
	ABSTRACT class Viewport_Game : public Panel_Viewport
	{
		const Vector2	MIN_SIZE	= Vector2( 400, 250 );
		cchar* const	PANEL_TITLE = "Game Viewport";


	public:
		Viewport_Game( int panelIndex );
		virtual ~Viewport_Game() {}


	private:
		virtual cchar*	_GetPanelTitle	() override;
		virtual Vector2	_GetPanelMinSize() override;
		virtual void	_OnInitialize	() override;
		virtual void	_OnFinalize		() override;
		virtual void	_OnUpdate		() override;


	protected:
		GameObject*	_RecFindMainCamera	( Node* root );
		void		_RecCollectLights	( Node* root, DataArray<GameObject*>& lightsGO );
	};
}