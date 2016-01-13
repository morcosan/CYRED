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
			void Render			( Scene* scene, GameObject* cameraGO )	override;
			void OnResize		()										override;
			void DisplayOnScreen()										override;


		protected:
			void _OnInitialize() override;


		protected:
			UInt _mainFramebufferID;
			UInt _mainColorbufferID;
			UInt _mainDepthbufferID;

			UInt _screenQuadID;

			void _CreateMainBuffers	( UInt width, UInt height );
			void _ResizeMainBuffers	( UInt width, UInt height );
			void _RenderScreenQuad	( Texture* texture, Shader* shader );
			void _GenerateScreenQuad();
		};
	}
}