// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"


namespace CYRED
{
	class GL;
	class Node;
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
		virtual void Render( int* buffers, Node* root, GameObject* cameraGO, 
							 bool useAllScenes )	PURE_VIRTUAL;


	public:
		void SetAPI			( GL* glAPI );

		void SetViewportSize( int width, int height );


	protected:
		GL*		_gl;
		int	_viewportWidth;
		int	_viewportHeight;


	protected:
		virtual void _ClearScreen()	PURE_VIRTUAL;
	};
}