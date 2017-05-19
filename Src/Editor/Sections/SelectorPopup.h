// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"

#include "QtWidgets\qwidget.h"
#include "QtWidgets\qlistwidget.h"



namespace CYRED
{
	class Selector;
}


namespace CYRED
{
	class SelectorPopup : public QWidget
	{
	public:
		const Vector2		MIN_SIZE	= Vector2( 70, 70 );


	public:
		SelectorPopup( QWidget* parent );
		virtual ~SelectorPopup();


	public:
		void A_Apply		( QListWidgetItem* item );

		void DisplayPopup	( const char* type, void* qtSelector );
		void Initialize		();

		/* it takes ownership of objects */
		void AddSelector	( const char* type, Selector* selector );

		void ClearList		();
		void AddToList		( const char* itemName, void* itemRef );
		void SetSelected	( int index );
		void SetPopupTitle	( const char* title );


	protected:
		struct _ListItem : public QListWidgetItem
		{
			void* valueRef;
		};


		QListWidget*	_qtList;

		void*			_qtSelectorPtr;

		DataMap<String, Selector*> _selectors;
	};
}