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
	//!		controls the rendering flow
	ABSTRACT class DLL RenderManager
	{
		DECLARE_LOCAL_SINGLETON( RenderManager )


	public:
		//!		creates the main canvas with id 0, then initializes the manager
		virtual void Initialize		( GLContext* glContext, GL* gl )		PURE_VIRTUAL;
		virtual void Finalize		()										PURE_VIRTUAL;

		//!		add technique to internal list
		//!		returns the id for technique
		virtual UInt NewTechnique	( TechniqueType techType )				PURE_VIRTUAL;

		//!		allows you to add custom technique
		virtual UInt NewTechnique	( Technique* technique )				PURE_VIRTUAL;

		//!		creates a new canvas for a window
		//!		returns the id for the new canvas
		virtual UInt NewCanvas		( GLContext* glContext )				PURE_VIRTUAL;


		virtual void ChangeRenderer	( UInt canvasID, RendererType type )	PURE_VIRTUAL;

		//!		you can create your own renderer
		virtual void ChangeRenderer	( UInt canvasID, Renderer* renderer )	PURE_VIRTUAL;

		virtual void ChangeTechnique( UInt canvasID, UInt techID )			PURE_VIRTUAL;


		virtual void Render			( UInt canvasID, Node* root, GameObject* cameraGO,
									  Bool useAllScenes )					PURE_VIRTUAL;

		virtual void OnResize		( UInt canvasID )						PURE_VIRTUAL;
	};
}





