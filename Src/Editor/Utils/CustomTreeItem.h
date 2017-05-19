// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"

#include "QtWidgets\qtreewidget.h"


namespace CYRED
{
	class GameObject;
	class Asset;
}


namespace CYRED
{
	class CustomTreeItem : public QTreeWidgetItem
	{
	public:
		CustomTreeItem() {}
		virtual ~CustomTreeItem()
		{
			while ( childCount() > 0 ) {
				Memory::Free( takeChild(0) );
			}
		}


	public:
		Asset*		asset;
		int			assetIndex;
		GameObject* gameObject;
	};
}