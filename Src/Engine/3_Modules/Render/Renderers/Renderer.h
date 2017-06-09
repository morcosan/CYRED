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
	class Transform;
	class Camera;
}


namespace CYRED
{
	//!		expected to change rarely or never, but change technique instead
	ABSTRACT class DLL Renderer
	{
	public:
		cchar* const UNIFORM_IS_LOOPING		= "isLooping";
		cchar* const UNIFORM_LIFETIME		= "lifetime";
		cchar* const UNIFORM_DELTA_TIME		= "deltaTime";
		cchar* const UNIFORM_STATE_RATIO	= "stateRatio";
		cchar* const UNIFORM_TEXT_COLOR		= "textColor";
		cchar* const UNIFORM_TEXT_TEXTURE	= "textTexture";


	public:
		Renderer() {}
		virtual ~Renderer() {}

		
	public:
		virtual void	ClearScreen		( float r, float g, float b )				PURE_VIRTUAL;
		virtual void	ResetDepth		()											PURE_VIRTUAL;
		virtual void	Render			( int layer, ComponentType compType, Node* target,
										  DataArray<GameObject*>& lightsGO )		PURE_VIRTUAL;

		virtual Vector4	ReadPixel		( int x, int y )							PURE_VIRTUAL;

		virtual void	OnResize		()											PURE_VIRTUAL;
		virtual void	DisplayOnScreen	()											PURE_VIRTUAL;


	public:
		void	Initialize		( GL* glAPI, GLContext* glContext );
		void	SwitchCamera	( Transform* cameraTran, Camera* camera );


	protected:
		virtual void	 _OnInitialize	()	PURE_VIRTUAL;


	protected:
		static uint _textVBO;


	protected:
		GL*			_gl;
		GLContext*	_glContext;
		int			_currLayer;
		Transform*	_currCameraTran;
		Camera*		_currCamera;

	private:
		void _CreateTextVBO();
	};
}