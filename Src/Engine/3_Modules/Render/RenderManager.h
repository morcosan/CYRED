// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"

#include "../../2_BuildingBlocks/Component.h"
#include "../../2_BuildingBlocks/Data/DataArray.h"
#include "../../2_BuildingBlocks/Math/Vector4.h"

namespace CYRED
{
	class GL;
	class GLContext;
	class Renderer;
	class Node;
	class GameObject;
	class Transform;
	class Camera;
}


namespace CYRED
{
	namespace Enums_RendererType
	{
		enum E
		{
			GL_FORWARD,
			GL_PICKING,
			GL_DEFERRED
		};
	}
	typedef Enums_RendererType::E	RendererType;
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
		virtual void	Initialize		( GLContext* glContext, GL* gl )			PURE_VIRTUAL;
		virtual void	Finalize		()											PURE_VIRTUAL;
			
		virtual int		CreateCanvas	( GLContext* glContext )					PURE_VIRTUAL;
		virtual void	SwitchCanvas	( int canvasID )							PURE_VIRTUAL;
		virtual void	CreateRenderer	( RendererType rendererType )				PURE_VIRTUAL;
		virtual void	SwitchRenderer	( RendererType rendererType )				PURE_VIRTUAL;
		virtual void	ClearScreen		( float r, float g, float b )				PURE_VIRTUAL;
		virtual void	ResetDepth		()											PURE_VIRTUAL;
		virtual void	SwitchCamera	( Transform* cameraTran, Camera* camera )	PURE_VIRTUAL;
		virtual void	Render			( int layer, ComponentType compType, Node* target, 
										  DataArray<GameObject*>& lightsGO )		PURE_VIRTUAL;
		virtual void	SwapBuffers		()											PURE_VIRTUAL;
		virtual void	OnResize		( int canvasID )							PURE_VIRTUAL;
		virtual Vector4	ReadPixel		( int x, int y )							PURE_VIRTUAL;
	};
}

