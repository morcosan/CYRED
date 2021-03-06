// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredApplication.h"
#include "CyredBuildingBlocks.h"

#include "Fragments\Panel.h"

#include <QtWidgets/QMainWindow>


class QApplication;
class QTime;

namespace CYRED
{
	class MenuBar;
	class ViewportPanel;
	class InputReceiverWindows;
	class SelectorPopup;
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
		void	Run			( Int& argc, Char* argv[] );
				
		/* create a new panel for editor				*/
		Panel*	NewPanel	( PanelType type, UInt panelIndex = 0, Bool isPrimary = TRUE );
		/* show a message in status bar					*/
		void	ShowStatus	( const Char* message );
		/* changes the skin of editor */
		void	ApplySkin	( const Char* skinName );

		void	ShowSelectorPopup( const Char* type, void* qtSelectorPtr );

		/* returns the input receiver instance			*/
		InputReceiverWindows* GetInputReceiver();


	protected:
		class _QtMainWindow;	/* internal class wrapper for QMainWindow	*/
		class _QtWindow;		/* internal class wrapper for QWindow		*/

		QApplication*	_qtApp;			/* the main QT object for application		*/
		_QtMainWindow*	_qtMainWindow;	/* the main window							*/
		MenuBar*		_menuBar;		/* the manu bar of window					*/
		QStatusBar*		_qtStatusBar;	/* the status bar of window					*/
		QTime*			_qtTime;		/* object for measuring time passed			*/
		SelectorPopup*	_selectorPopup;	/* popup window for selecting from a list	*/

		Bool			_shouldExit;	/* flag that is used to exit the main loop	*/
		Bool			_isPlayMode;	/* flag that indicates "play in editor"		*/

		Char*			_skinStylesheet;	/* stylesheet for customizing editor	*/	

		DataArray<Panel*>			_panels;	/* list with panel without rendering	*/
		DataArray<GameObject*>		_cameras;	/* list with default camera GOs			*/
		ViewportPanel*				_mainViewport;

		InputReceiverWindows*		_inputReceiver;	/* object for receiving input events	*/


	protected:
		/* prepares the application for exit				*/
		/* called once before closing the editor			*/
		void _CleanUp				();
		/* the main loop of application						*/
		/* called continuously, every frame					*/
		void _UpdateLoop			();

		/* create singletons for manager classes			*/
		void _CreateManagers		();
		/* initialize singletons							*/
		void _InitializeManagers	();
		/* finalize singletons								*/
		void _FinalizeManagers		();
		/* destroy the singletons							*/
		void _DestroyManagers		();

		/* creates the editor window						*/
		void _CreateMainWindow		();
		/* creates menu bar and adds it to window			*/
		void _CreateMenuBar			();
		/* creates status bar and adds it to window			*/
		void _CreateStatusBar		();

		void _CreateSelectorPopup	();

		/* initializes the default cameras for viewports	*/
		void _CreateCameras			();
		/* update for the scripts on each camera			*/
		void _UpdateCameras			();

		void _ReadConfigFile		();
		void _ReadProjectFile		();
	};
}