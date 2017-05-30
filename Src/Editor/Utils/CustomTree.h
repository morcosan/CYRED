// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"

#include "QtWidgets\qtreewidget.h"

class QMouseEvent;
class QDropEvent;


namespace CYRED
{
	class CustomTree : public QTreeWidget
	{
	public:
		CustomTree() {}
		virtual ~CustomTree() {}


	public:
		void mousePressEvent( QMouseEvent* event )	override;
		void dropEvent		( QDropEvent* event )	override;
	};
}