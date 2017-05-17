// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "../Assets/Shader.h"


namespace CYRED
{
	class Technique;
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
		virtual void Render			( Node* root, GameObject* cameraGO, bool useAllScenes)	PURE_VIRTUAL;
		virtual void OnResize		() PURE_VIRTUAL;
		virtual void DisplayOnScreen() PURE_VIRTUAL;


	public:
		void Initialize		( GL* glAPI, GLContext* glContext );
		void SetTechnique	( Technique* technique );


	protected:
		virtual void _OnInitialize() PURE_VIRTUAL;


	protected:
		static Shader* _screenQuadShader;


	protected:
		Technique*	_technique;
		GL*			_gl;
		GLContext*	_glContext;
	};
}