// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"

#include <QtWidgets/qtoolbar.h>


class QToolButton;


namespace CYRED
{
	class Toolbar : public QToolBar
	{
	public:
		Toolbar();
		virtual ~Toolbar() {}


	public:
		void A_PlayButton	( Bool checked );
		void A_PauseButton	( Bool checked );


	public:
		void SetPlayButton	( Bool isActive );
		void SetPauseButton	( Bool isActive );


	protected:
		QToolButton* _playButton;
		QToolButton* _pauseButton;
	};
}