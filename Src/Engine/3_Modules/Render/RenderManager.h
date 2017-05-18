// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "../../2_BuildingBlocks/Component.h"


namespace CYRED
{
	class GL;
	class GLContext;
	class Renderer;
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
		* @desc: initialize manager by creating first canvas, with id 0
		* @params: 
		* 		glContext	- the context of the main window
		* 		gl			- the OpenGL API
		*/
		virtual void	Initialize		( GLContext* glContext, GL* gl )	PURE_VIRTUAL;

		/*****
		* @desc: destroy the manager
		* @assert: it was first initialized
		*/
		virtual void	Finalize		()									PURE_VIRTUAL;
		
		/*****
		* @desc: create a new canvas and activate it
		* @params: 
		* 		glContext - the context of the window
		* @return: the index of the new canvas
		*/
		virtual int		CreateCanvas	( GLContext* glContext )			PURE_VIRTUAL;

		/*****
		* @desc: change the current canvas
		* @params: 
		* 		canvasID - id of the new canvas
		* @assert: canvas exists
		*/
		virtual void	SwitchCanvas	( int canvasID )					PURE_VIRTUAL;

		/*****
		* @desc: create a new renderer for current canvas
		* @params: 
		* 		rendererType - type of the renderer
		* @assert: canvas is set
		*/
		virtual void	CreateRenderer	( RendererType rendererType )		PURE_VIRTUAL;

		/*****
		* @desc: change the current renderer
		* @params: 
		* 		rendererType - type of the renderer
		* @assert: canvas is set
		*/
		virtual void	SwitchRenderer	( RendererType rendererType )		PURE_VIRTUAL;

		/*****
		* @desc: clear the previous frame
		*/
		virtual void ClearScreen	()										PURE_VIRTUAL;

		/*****
		* @desc: render the given component from given gameobject and its children
		* @params: 
		* 		compType	- the component to render
		* 		target		- the target gameobject
		* 		cameraGO	- camera
		* 		lights		- the list of lights to be used
		* @assert: canvas and renderer are set
		*/
		virtual void Render			( ComponentType compType, Node* target, GameObject* cameraGO,
									  GameObject** lights )						PURE_VIRTUAL;

		/*****
		* @desc: force resize for given canvas
		* @params: 
		* 		canvasID - id of canvas
		* @assert: canvas exists
		*/
		virtual void OnResize		( int canvasID )						PURE_VIRTUAL;
	};
}





