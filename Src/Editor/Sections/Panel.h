// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"

#include <QtWidgets/QDockWidget>


namespace CYRED
{
	namespace Enum_PanelType
	{
		enum Enum
		{
			ATTRIBUTES
			, SCENE_VIEWPORT
			, SCENE_HIERARCHY
			, ASSETS
			, CONSOLE
			, PREFAB_HIERARCHY
			, PREFAB_VIEWPORT
			, GAME_VIEWPORT
			, ISOLATE_HIERARCHY
			, ISOLATE_VIEWPORT
		};
	}
	typedef Enum_PanelType::Enum	PanelType;
}


namespace CYRED
{
	/* the most basic container inside editor window */
	/* it is resizable and movable */
	ABSTRACT class Panel : public QDockWidget
	{
	public:
		Panel();
		virtual ~Panel() {}


	public:
		virtual void Initialize	()					PURE_VIRTUAL;
		virtual void Finalize	()					PURE_VIRTUAL;
		virtual void Update		( bool isRuntime ) {};


	public:
		/* method from QDockWidget for receiving resize event */
		void resizeEvent	( QResizeEvent* e ) override;
		

	protected:
		bool	_isInitialized;	/* flag to check if panel was initialized */


	protected:
		/* handle for resize event */
		virtual void _OnResize() {};
	};
}