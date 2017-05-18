// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"
#include "../Panel.h"

#include <QtGui\qwindow.h>


class QHBoxLayout;

namespace CYRED
{
	class GLContext;
	class InputReceiverQT;
}


namespace CYRED
{
	ABSTRACT class Panel_Viewport : public Panel
	{
	public:
		Panel_Viewport( int panelIndex );
		virtual ~Panel_Viewport() {}


	public:
		void Initialize	() override;
		void Finalize	() override;
		void Update		() override;


	public:
		void		Initialize		( bool isPrimary );
		GLContext*	GetGLContext	();


	protected:
		class _QtWindow : public QWindow
		{
		public:
			_QtWindow( int windowIndex, InputReceiverQT* inputReceiver ) 
				: _windowIndex( windowIndex )
				, _inputReceiver( inputReceiver )
			{
			} 

		protected:
			void keyPressEvent		( QKeyEvent* e )	override;
			void keyReleaseEvent	( QKeyEvent* e )	override;
			void mouseMoveEvent		( QMouseEvent* e )	override;
			void mousePressEvent	( QMouseEvent* e )	override;
			void mouseReleaseEvent	( QMouseEvent* e )	override;
			void wheelEvent			( QWheelEvent* e )	override;
			void focusOutEvent		( QFocusEvent* e )	override;

		protected:
			int					_windowIndex;
			InputReceiverQT*	_inputReceiver;
		};


	protected:
		void _OnResize() override;


	protected:
		virtual const char*	_GetPanelTitle	() PURE_VIRTUAL;
		virtual Vector2		_GetPanelMinSize() PURE_VIRTUAL;
		virtual void		_OnInitialize	() PURE_VIRTUAL;
		virtual void		_OnFinalize		() PURE_VIRTUAL;
		virtual void		_OnUpdate		() PURE_VIRTUAL;


	protected:
		GLContext*		_glContext;
		_QtWindow*		_qtWindow;
		int				_canvasSlot;
		bool			_mustResize;
		int				_panelIndex;
		bool			_isFirstUpdate;
		GameObject*		_cameraGO;

		QHBoxLayout*	_qtTopBarLayout;


	protected:
		void	_CreateCanvasSlot();
	};
}