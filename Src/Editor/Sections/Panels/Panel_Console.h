// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredBuildingBlocks.h"
#include "CyredModule_Event.h"
#include "../Panel.h"


class QVBoxLayout;


namespace CYRED
{
	ABSTRACT class Panel_Console : public Panel, public IEventListener
	{
		cchar* const	PANEL_TITLE = "Console";
		const Vector2	MIN_SIZE	= Vector2( 100, 100 );


	public:
		Panel_Console();
		virtual ~Panel_Console() {}


	public:
		void A_ClearLogs();


	public:
		void Initialize	() override;
		void Finalize	() override;
		void OnEvent	( EventType eType, void* eData )	override;


	private:
		QVBoxLayout* _logsLayout;


	private:
		void _AddLine( cchar* messages, bool isError );
	};
}