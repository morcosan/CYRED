// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"


namespace CYRED
{
	class GL;
	class GLContext;
	class Renderer;
	class Technique;
	class Node;
	class GameObject;
}

namespace CYRED
{
	namespace Enums_RendererType
	{
		enum E
		{
			GL_FORWARD,
			GL_DEFERRED
		};
	}
	namespace Enums_TechniqueType
	{
		enum E
		{
			FORWARD_BASIC
		};
	}
	typedef Enums_RendererType::E	RendererType;
	typedef Enums_TechniqueType::E	TechniqueType;
}


namespace CYRED
{
	/*
	state machine for rendering
	*/
	ABSTRACT class DLL RenderManager
	{
		DECLARE_LOCAL_SINGLETON( RenderManager )


	public:
		/*****
		initialize manager by creating first canvas, with id 0
		@params: 
			glContext	- the context of the main window
			gl			- the OpenGL API
		*/
		virtual void	Initialize		( GLContext* glContext, GL* gl )	PURE_VIRTUAL;

		/*****
		destroy the manager
		@assert: it was first initialized
		*/
		virtual void	Finalize		()									PURE_VIRTUAL;
		
		/*****
		create a new canvas and activate it
		@params: 
			glContext - the context of the window
		@return: the index of the new canvas
		*/
		virtual int		CreateCanvas	( GLContext* glContext )			PURE_VIRTUAL;

		/*****
		change the current canvas
		@params: 
			canvasID - id of the new canvas
		@assert: canvas id exists
		*/
		virtual void	SwitchCanvas	( int canvasID )					PURE_VIRTUAL;

		/*****
		create a new renderer for current canvas
		@params: 
			rendererType - type of the renderer
		*/
		virtual void	CreateRenderer	( RendererType rendererType )		PURE_VIRTUAL;

		/*****
		change the current renderer for current canvas
		@params: 
		canvasID - id of the new canvas
		@assert: canvas id exists
		*/
		virtual void	SwitchRenderer	( RendererType rendererType )		PURE_VIRTUAL;



		virtual void Render			( int canvasID, Node* root, GameObject* cameraGO,
									  bool useAllScenes )					PURE_VIRTUAL;

		virtual void OnResize		( int canvasID )						PURE_VIRTUAL;
	};
}





