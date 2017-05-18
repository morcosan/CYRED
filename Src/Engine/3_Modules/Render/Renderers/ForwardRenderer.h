// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "Renderer.h"


namespace CYRED
{
	class Texture;
	class Shader;
	class Material;
	class Transform;
	class Camera;
	class Light;
}


namespace CYRED
{
	namespace NotAPI
	{
		class ForwardRenderer : public Renderer
		{
			const char* UNIFORM_IS_LOOPING	= "isLooping";
			const char* UNIFORM_LIFETIME	= "lifetime";
			const char* UNIFORM_DELTA_TIME	= "deltaTime";

			const char* UNIFORM_STATE_RATIO	= "stateRatio";


		public:
			ForwardRenderer() {}
			virtual ~ForwardRenderer();


		public:
			/*****
			* @desc: clear the previous frame
			*/
			void ClearScreen	()					override;

			/*****
			* @desc: render the given component from given gameobject and its children
			* @params: 
			* 		compType	- the component to render
			* 		target		- the target gameobject
			* 		cameraGO	- camera
			* 		lights		- the list of lights to be used
			*/
			void Render			( ComponentType compType, Node* target, GameObject* cameraGO,
								  GameObject** lights )	override;

			void OnResize		()						override;
			void DisplayOnScreen()						override;


		protected:
			/*****
			* @desc: apply specific initialization per renderer; 
			*		 called at the end of Initialize()
			*/
			void _OnInitialize	() override;


		protected:
			uint _mainFramebufferID;
			uint _mainColorbufferID;
			uint _mainDepthbufferID;

			uint _screenQuadID;

			Transform*		_currCameraTran;
			Camera*			_currCameraCam;
			GameObject**	_currLights;


		protected:
			void _CreateMainBuffers	( int width, int height );
			void _ResizeMainBuffers	( int width, int height );
			void _RenderScreenQuad	( Texture* texture, Shader* shader );
			void _GenerateScreenQuad();

			void _RecRenderMesh		( GameObject* gameObject );
			void _RecRenderMorph	( GameObject* gameObject );
			void _RecRenderParticles( GameObject* gameObject );

			void _BindMaterial		( Material* material );
		};
	}
}