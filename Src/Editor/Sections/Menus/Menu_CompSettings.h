// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"

#include "QtWidgets\qmenu.h"

class QTreeWidget;

namespace CYRED
{
	class Component;
}


namespace CYRED
{
	class Menu_CompSettings : public QMenu
	{
		cchar* const MENU_REMOVE	= "Remove";


	public:
		Menu_CompSettings( QWidget* qtParent );
		virtual ~Menu_CompSettings() {}


	public:
		void Open( Component* target );


	public:
		void A_ShowMenu	();
		void A_Remove	();


	private:
		QWidget*	_qtParent;
		Component*	_target;
	};
}