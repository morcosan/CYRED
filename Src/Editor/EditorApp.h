// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredApplication.h"
#include "CyredBuildingBlocks.h"

#include "Sections\Panel.h"
#include "Sections\Settings\EditorSettings.h"

#include <QtWidgets/QMainWindow>

class QApplication;
class QTime;

namespace CYRED
{
	class MenuBar;
	class Panel_Viewport;
	class InputReceiverQT;
	class SelectorPopup;
	class Toolbar;
}


namespace CYRED
{
	/* main class for editor */
	class EditorApp : public Application
	{
		DECLARE_REMOTE_SINGLETON_IMPL( EditorApp )


	public:
		void Exit() override;


	public:
		/* main function for starting the editor		*/
		/* must be called only once from main()			*/
		void	Run			( int& argc, char* argv[] );

		void	ShowStatus	( cchar* message );

		void	ApplySkin	( cchar* skinName );

		void	StartPlayMode	();				// called to start play mode
		void	StopPlayMode	();				// called to exit play mode
		void	SetPlayPaused	( bool value );	// called to set the play mode flag

		void	ShowSelectorPopup( cchar* type, void* qtSelectorPtr );

		/* returns the input receiver instance			*/
		InputReceiverQT* GetInputReceiver();


	private:
		class _QtMainWindow;	/* internal class wrapper for QMainWindow	*/
		class _QtWindow;		/* internal class wrapper for QWindow		*/

		QApplication*	_qtApp;			/* the main QT object for application		*/
		_QtMainWindow*	_qtMainWindow;	/* the main window							*/
		MenuBar*		_menuBar;		/* the manu bar of window					*/
		QStatusBar*		_qtStatusBar;	/* the status bar of window					*/
		QTime*			_qtTime;		/* object for measuring time passed			*/
		SelectorPopup*	_selectorPopup;	/* popup window for selecting from a list	*/
		Toolbar*		_toolbar;		// main toolbar for editor

		bool			_shouldExit;	/* flag that is used to exit the main loop	*/
		bool			_isPlayMode;	/* flag that indicates "play in editor"		*/
		bool			_shouldStopPlay;/* flag that is used to update play mode */
		bool			_isPlayPaused;	// flag for running the scripts in play mode

		char*			_skinStylesheet;	/* stylesheet for customizing editor	*/	

		DataMap<PanelType, Panel*>	_panels;
		DataArray<GameObject*>		_cameras;

		InputReceiverQT*		_inputReceiver;	/* object for receiving input events	*/


	private:
		/* prepares the application for exit				*/
		/* called once before closing the editor			*/
		void _CleanUp				();
		/* the main loop of application						*/
		/* called continuously, every frame					*/
		void _UpdateLoop			();

		Panel*	_NewPanel	( PanelType type, int viewportIndex = -1 );
		Panel*	_NewPanel	( PanelType type, PanelType splitFrom, PanelSplitType splitType,
							  int viewportIndex = -1 );

		/* create singletons for manager classes			*/
		void _CreateManagers		();
		/* initialize singletons							*/
		void _InitializeManagers	();
		/* finalize singletons								*/
		void _FinalizeManagers		();
		/* destroy the singletons							*/
		void _DestroyManagers		();

		void _FinalizePanels		();

		/* creates the editor window						*/
		void _CreateMainWindow		();
		/* creates menu bar and adds it to window			*/
		void _CreateMenuBar			();
		/* creates status bar and adds it to window			*/
		void _CreateStatusBar		();
		void _CreateToolbar			();

		void _CreateSelectorPopup	();

		/* initializes the default cameras for viewports	*/
		void _CreateCameras			();
		/* update for the scripts on each camera			*/
		void _UpdateCameras			();

		void _ReadConfigFile		();
		void _ReadProjectFile		();

		void _WriteConfigFile		();
	};
}