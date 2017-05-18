// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "../Assets/Shader.h"
#include "../../../2_BuildingBlocks/Component.h"


namespace CYRED
{
	class GL;
	class GLContext;
	class Node;
	class GameObject;
}


namespace CYRED
{
	//!		expected to change rarely or never, but change technique instead
	ABSTRACT class DLL Renderer
	{
	public:
		Renderer() {}
		virtual ~Renderer() {}

		
	public:
		/*****
		* @desc: clear the previous frame
		*/
		virtual void ClearScreen	()					PURE_VIRTUAL;

		/*****
		* @desc: render the given component from given gameobject and its children
		* @params: 
		* 		compType	- the component to render
		* 		target		- the target gameobject
		* 		cameraGO	- camera
		* 		lights		- the list of lights to be used
		*/
		virtual void Render			( ComponentType compType, Node* target, GameObject* cameraGO,
									  GameObject** lights )	PURE_VIRTUAL;

		virtual void OnResize		()					PURE_VIRTUAL;
		virtual void DisplayOnScreen()					PURE_VIRTUAL;


	public:
		/*****
		* @desc: prepare the renderer
		* @params: 
		* 		glAPI		- the global GL API
		* 		glContext	- the context of the canvas
		*/
		void Initialize		( GL* glAPI, GLContext* glContext );


	protected:
		/*****
		* @desc: apply specific initialization per renderer; 
		*		 called at the end of Initialize()
		*/
		virtual void _OnInitialize	()					PURE_VIRTUAL;

		


	protected:
		static Shader* _screenQuadShader;


	protected:
		GL*			_gl;
		GLContext*	_glContext;
	};
}