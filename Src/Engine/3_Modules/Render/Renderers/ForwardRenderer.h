// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "Renderer.h"


namespace CYRED
{
	class Texture;
	class Shader;
}


namespace CYRED
{
	namespace NotAPI
	{
		class ForwardRenderer : public Renderer
		{
		public:
			ForwardRenderer() {}
			virtual ~ForwardRenderer();


		public:
			void Render			( Node* root, GameObject* cameraGO, bool useAllScenes )	override;
			void OnResize		() override;
			void DisplayOnScreen() override;


		protected:
			void _OnInitialize() override;


		protected:
			int _mainFramebufferID;
			int _mainColorbufferID;
			int _mainDepthbufferID;

			int _screenQuadID;

			void _CreateMainBuffers	( int width, int height );
			void _ResizeMainBuffers	( int width, int height );
			void _RenderScreenQuad	( Texture* texture, Shader* shader );
			void _GenerateScreenQuad();
		};
	}
}