// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Component.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"
#include "../../../2_BuildingBlocks/Math/Vector4.h"
#include "../Assets/Shader.h"

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
		virtual void ClearScreen	( float r, float g, float b )	PURE_VIRTUAL;

		/*****
		* @desc: clear the depth buffer; new rendering goes over anything before
		*/
		virtual void ResetDepth		()								PURE_VIRTUAL;

		/*****
		* @desc: render the given component from given gameobject and its children
		* @params: 
		* 		compType	- the component to render
		* 		target		- the target gameobject
		* 		cameraGO	- camera
		* 		lightsGO	- the list of lights to be used
		*/
		virtual void Render			( ComponentType compType, Node* target, GameObject* cameraGO,
									  DataArray<GameObject*>& lightsGO )	PURE_VIRTUAL;

		/*****
		* @desc: read the pixel from renderer at given location
		* @params: 
		* 		x - location on x axis
		* 		y - location on y axis
		* @assert: canvas and renderer are set
		*/
		virtual Vector4	ReadPixel	( int x, int y )				PURE_VIRTUAL;

		virtual void OnResize		()								PURE_VIRTUAL;
		virtual void DisplayOnScreen()								PURE_VIRTUAL;


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