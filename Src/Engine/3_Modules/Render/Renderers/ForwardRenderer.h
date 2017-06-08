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
	namespace NonAPI
	{
		class ForwardRenderer : public Renderer
		{
			cchar* UNIFORM_IS_LOOPING	= "isLooping";
			cchar* UNIFORM_LIFETIME		= "lifetime";
			cchar* UNIFORM_DELTA_TIME	= "deltaTime";
			cchar* UNIFORM_STATE_RATIO	= "stateRatio";
			cchar* UNIFORM_TEXT_COLOR	= "textColor";
			cchar* UNIFORM_TEXT_TEXTURE	= "textTexture";


		public:
			ForwardRenderer() {}
			virtual ~ForwardRenderer() {}


		public:
			void ClearScreen	( float r, float g, float b )			override;
			void ResetDepth		()										override;
			void Render			( ComponentType compType, Node* target, GameObject* cameraGO,
								  DataArray<GameObject*>& lightsGO )	override;

			Vector4	ReadPixel	( int x, int y )						override;

			void OnResize		()										override;
			void DisplayOnScreen()										override;


		private:
			void _OnInitialize	() override;


		private:
			Transform*	_currCameraTran;
			Camera*		_currCameraCam;


		private:
			void _RecRenderMesh		( GameObject* gameObject, DataArray<GameObject*>& lightsGO );
			void _RecRenderMorph	( GameObject* gameObject, DataArray<GameObject*>& lightsGO );
			void _RecRenderParticles( GameObject* gameObject );
			void _RecRenderText3D	( GameObject* gameObject );

			void _BindMaterial		( Material* material );
		};
	}
}