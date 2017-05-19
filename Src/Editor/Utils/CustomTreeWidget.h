// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"

#include "QtWidgets\qtreewidget.h"


namespace CYRED
{
	class AttrViewer;
}


namespace CYRED
{
	class CustomTreeWidget : public QTreeWidget
	{
	public:
		CustomTreeWidget();
		virtual ~CustomTreeWidget() {}


	public:
		void A_CalculateHeight	();

		void SetAttrViewer		( AttrViewer* attrViewer );


	private:
		AttrViewer* _attrViewer;

		int _CalculateHeightRec( QTreeWidgetItem* item );
	};
}