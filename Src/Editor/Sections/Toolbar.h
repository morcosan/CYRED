// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		void A_PlayButton	( bool checked );
		void A_PauseButton	( bool checked );


	public:
		void SetPlayButton	( bool isActive );
		void SetPauseButton	( bool isActive );


	protected:
		QToolButton* _playButton;
		QToolButton* _pauseButton;
	};
}