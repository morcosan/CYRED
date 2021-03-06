// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"


namespace CYRED
{
	ABSTRACT class DLL GLContext
	{
	public:
		GLContext() : _width(0), _height(0) {}
		virtual ~GLContext() {};


		virtual void MakeCurrent		()						PURE_VIRTUAL;
		virtual void SwapBuffers		()						PURE_VIRTUAL;

		virtual void OnResize			()						PURE_VIRTUAL;

		virtual void SetSharedContext	( GLContext* other )	PURE_VIRTUAL;

		UInt GetWidth	();
		UInt GetHeight	();


	protected:
		UInt _width;
		UInt _height;
	};
}