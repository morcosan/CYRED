// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"
#include "../Panel.h"

#include <QtGui\qwindow.h>

class QHBoxLayout;

namespace CYRED
{
	class GameObject;
	class GLContext;
	class InputReceiverQT;
}


namespace CYRED
{
	ABSTRACT class Panel_Viewport : public Panel
	{
		const Vector2	MIN_SIZE = Vector2( 200, 200 );


	public:
		Panel_Viewport( int panelIndex );
		virtual ~Panel_Viewport() {}


	public:
		void Initialize			()					override;
		void Finalize			()					override;
		void Update				( bool isRuntime )	override;


	public:
		virtual void LoadGizmo	() {}


	public:
		void		Initialize	( bool isPrimary );
		GLContext*	GetGLContext();


	private:
		void _OnResize() override;


	private:
		virtual cchar*	_GetPanelTitle	()					PURE_VIRTUAL;
		virtual void	_OnInitialize	()					PURE_VIRTUAL;
		virtual void	_OnFinalize		()					PURE_VIRTUAL;
		virtual void	_OnUpdate		( bool isRuntime )	PURE_VIRTUAL;


	protected:
		class _QtWindow : public QWindow
		{
		public:
			_QtWindow( int windowIndex, InputReceiverQT* inputReceiver ) 
				: _windowIndex( windowIndex )
				, _inputReceiver( inputReceiver )
			{
			} 

		private:
			void keyPressEvent		( QKeyEvent* e )	override;
			void keyReleaseEvent	( QKeyEvent* e )	override;
			void mouseMoveEvent		( QMouseEvent* e )	override;
			void mousePressEvent	( QMouseEvent* e )	override;
			void mouseReleaseEvent	( QMouseEvent* e )	override;
			void wheelEvent			( QWheelEvent* e )	override;
			void focusOutEvent		( QFocusEvent* e )	override;

		private:
			int					_windowIndex;
			InputReceiverQT*	_inputReceiver;
		};

		GLContext*		_glContext;
		_QtWindow*		_qtWindow;
		QWidget*		_qtContainer;
		int				_canvasSlot;
		bool			_mustResize;
		int				_panelIndex;
		bool			_isFirstUpdate;
		GameObject*		_cameraGO;

		QHBoxLayout*	_qtTopBarLayout;


	protected:
		void	_RecCollectLayers( GameObject* root, OUT DataArray<int>& layers );

	private:
		void	_CreateCanvasSlot();
	};
}