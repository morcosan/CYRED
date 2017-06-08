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
		virtual void	ClearScreen		( float r, float g, float b )	PURE_VIRTUAL;
		virtual void	ResetDepth		()								PURE_VIRTUAL;
		virtual void	Render			( ComponentType compType, Node* target, GameObject* cameraGO,
										  DataArray<GameObject*>& lightsGO )	PURE_VIRTUAL;

		virtual Vector4	ReadPixel		( int x, int y )				PURE_VIRTUAL;

		virtual void	OnResize		()								PURE_VIRTUAL;
		virtual void	DisplayOnScreen	()								PURE_VIRTUAL;


	public:
		void Initialize	( GL* glAPI, GLContext* glContext );


	protected:
		virtual void	 _OnInitialize	()								PURE_VIRTUAL;


	protected:
		static uint _textVBO;


	protected:
		GL*			_gl;
		GLContext*	_glContext;


	private:
		void _CreateTextVBO();
	};
}