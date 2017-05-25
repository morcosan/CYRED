// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
			cchar* UNIFORM_IS_LOOPING	= "isLooping";
			cchar* UNIFORM_LIFETIME	= "lifetime";
			cchar* UNIFORM_DELTA_TIME	= "deltaTime";

			cchar* UNIFORM_STATE_RATIO	= "stateRatio";


		public:
			ForwardRenderer() {}
			virtual ~ForwardRenderer();


		public:
			/*****
			* @desc: clear the previous frame
			*/
			void ClearScreen	( float r, float g, float b )			override;

			/*****
			* @desc: clear the depth buffer; new rendering goes over anything before
			*/
			void ResetDepth		()										override;

			/*****
			* @desc: render the given component from given gameobject and its children
			* @params: 
			* 		compType	- the component to render
			* 		target		- the target gameobject
			* 		cameraGO	- camera
			* 		lightsGO	- the list of lights to be used
			*/
			void Render			( ComponentType compType, Node* target, GameObject* cameraGO,
								  DataArray<GameObject*>& lightsGO )	override;

			/*****
			* @desc: read the pixel from renderer at given location
			* @params: 
			* 		x - location on x axis
			* 		y - location on y axis
			* @assert: canvas and renderer are set
			*/
			Vector4	ReadPixel	( int x, int y )						override;

			void OnResize		()										override;
			void DisplayOnScreen()										override;


		private:
			/*****
			* @desc: apply specific initialization per renderer; 
			*		 called at the end of Initialize()
			*/
			void _OnInitialize	() override;


		private:
			Transform*	_currCameraTran;
			Camera*		_currCameraCam;


		private:
			void _RecRenderMesh		( GameObject* gameObject, DataArray<GameObject*>& lightsGO );
			void _RecRenderMorph	( GameObject* gameObject, DataArray<GameObject*>& lightsGO );
			void _RecRenderParticles( GameObject* gameObject );

			void _BindMaterial		( Material* material );
		};
	}
}