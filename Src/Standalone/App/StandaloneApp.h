// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredEngine.h"

#include <QtWidgets/QMainWindow>


class QApplication;


namespace CYRED
{
	class GameController;

	namespace NotAPI
	{
		class StandaloneApp : public Application
		{
		public:
			StandaloneApp() {}
			virtual ~StandaloneApp() {}

			void Exit() override;


		public:
			void Run( Int& argc, Char* argv[], GameController* gameController );
			void Run( Int& argc, Char* argv[], Char* configFilePath );


		protected:
			class _QtMainWindow;
			class _QtWindow;

			QApplication*	_qtApp;
			_QtMainWindow*	_qtMainWindow;
			_QtWindow*		_qtWindow;

			GameController* _gameController;
			Bool			_shouldExit;

			UInt			_canvasID;


		protected:
			void _Run( Int &argc, Char *argv[] );

			void _CleanUp();
			void _UpdateLoop();

			void _CreateManagers();
			void _InitializeManagers();
			void _FinalizeManagers();
			void _DestroyManagers();
		};
	}
}