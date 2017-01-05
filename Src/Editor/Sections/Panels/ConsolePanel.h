// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"
#include "CyredModule_Event.h"
#include "../Panel.h"


class QVBoxLayout;


namespace CYRED
{
	ABSTRACT class ConsolePanel : public Panel, public IEventListener
	{
		const Char* const	PANEL_TITLE = "Console Panel";
		const Vector2		MIN_SIZE	= Vector2( 100, 100 );


	public:
		ConsolePanel();
		virtual ~ConsolePanel() {}


	public:
		void A_ClearLogs();


	public:
		void Initialize	() override;
		void Finalize	() override;
		void OnEvent	( EventType eType, void* eData )	override;


	protected:
		QVBoxLayout* _logsLayout;


	protected:
		void _AddLine( const Char* messages, Bool isError );
	};
}