// Copyright (c) 2015 Morco (www.morco.ro)
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
			HIERARCHY
			, ATTRIBUTES
			, SCENE_VIEWPORT
			, ASSETS
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
		/* initializes the panel; must be called once before all else */
		virtual void Initialize	() PURE_VIRTUAL;
		virtual void Update		() {};


	public:
		/* method from QDockWidget for receiving resize event */
		void resizeEvent	( QResizeEvent* e ) override;
		

	protected:
		Bool	_isInitialized;	/* flag to check if panel was initialized */

		/* handle for resize event */
		virtual void _OnResize() {};
	};
}