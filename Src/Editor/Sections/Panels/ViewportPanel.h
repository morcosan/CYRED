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
	ABSTRACT class ViewportPanel : public Panel
	{
	public:
		ViewportPanel( UInt panelIndex );
		virtual ~ViewportPanel() {}


	public:
		void Initialize	() override;
		void Finalize	() override;
		void Update		() override;


	public:
		void		Initialize		( Bool isPrimary );
		GLContext*	GetGLContext	();


	protected:
		class _QtWindow : public QWindow
		{
		public:
			_QtWindow( Int windowIndex, InputReceiverQT* inputReceiver ) 
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
			Int						_windowIndex;
			InputReceiverQT*	_inputReceiver;
		};


	protected:
		void _OnResize() override;


	protected:
		virtual const Char*	_GetPanelTitle	() PURE_VIRTUAL;
		virtual Vector2		_GetPanelMinSize() PURE_VIRTUAL;
		virtual void		_OnInitialize	() PURE_VIRTUAL;
		virtual void		_OnUpdate		() PURE_VIRTUAL;


	protected:
		GLContext*		_glContext;
		_QtWindow*		_qtWindow;
		UInt			_canvasSlot;
		Bool			_mustResize;
		UInt			_panelIndex;
		Bool			_isFirstUpdate;
		GameObject*		_cameraGO;

		QHBoxLayout*	_qtTopBarLayout;

		void	_CreateCanvasSlot();
	};
}