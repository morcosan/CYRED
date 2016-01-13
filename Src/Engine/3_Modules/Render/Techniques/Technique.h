// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"


namespace CYRED
{
	class GL;
	class Scene;
	class GameObject;
}


namespace CYRED
{
	//!		it is expected to change every frame
	ABSTRACT class DLL Technique
	{
	public:
		Technique() {}
		virtual ~Technique() {}


	public:
		virtual void Render( UInt* buffers, Scene* scene, GameObject* cameraGO )	PURE_VIRTUAL;


	public:
		void SetAPI			( GL* glAPI );

		void SetViewportSize	( UInt width, UInt height );


	protected:
		GL*		_gl;
		UInt	_viewportWidth;
		UInt	_viewportHeight;


	protected:
		virtual void _ClearScreen()	PURE_VIRTUAL;
	};
}