// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"

#include "QtWidgets\qtreewidget.h"


namespace CYRED
{
	class GameObject;
	class Scene;
}


namespace CYRED
{
	class CustomTreeItem : public QTreeWidgetItem
	{
	public:
		CustomTreeItem() {}
		virtual ~CustomTreeItem() {}


	public:
		Asset*		asset;
		int		assetIndex;
		GameObject* gameObject;
	};
}