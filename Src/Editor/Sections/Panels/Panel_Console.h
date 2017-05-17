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
	ABSTRACT class Panel_Console : public Panel, public IEventListener
	{
		const char* const	PANEL_TITLE = "Console";
		const Vector2		MIN_SIZE	= Vector2( 100, 100 );

		const char* const	ICON_INFO	= "info_icon.png";
		const char* const	ICON_ERROR	= "error_icon.png";


	public:
		Panel_Console();
		virtual ~Panel_Console() {}


	public:
		void A_ClearLogs();


	public:
		void Initialize	() override;
		void Finalize	() override;
		void OnEvent	( EventType eType, void* eData )	override;


	protected:
		QVBoxLayout* _logsLayout;


	protected:
		void _AddLine( const char* messages, bool isError );
	};
}