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
		class PickingRenderer : public Renderer
		{
		public:
			PickingRenderer() {}
			virtual ~PickingRenderer();


		public:
			void	ClearScreen		( float r, float g, float b )				override;
			void	ResetDepth		()											override;
			void	Render			( int layer, ComponentType compType, Node* target,
									  DataArray<GameObject*>& lightsGO )		override;

			Vector4	ReadPixel		( int x, int y )							override;

			void	OnResize		()											override;
			void	DisplayOnScreen	()											override;


		private:
			void	_OnInitialize	()											override;


		private:
			uint _frameBufferID;
			uint _colorBufferID;
			uint _depthBufferID;

			//uint	_screenQuadID;


		private:
			void _CreateBuffers		( int width, int height );
			void _ResizeBuffers		( int width, int height );
			//void _RenderScreenQuad	( Texture* texture, Shader* shader );
			//void _GenerateScreenQuad();

			void _RecRenderMesh		( GameObject* gameObject, DataArray<GameObject*>& lightsGO );
			void _RecRenderMorph	( GameObject* gameObject, DataArray<GameObject*>& lightsGO );
			void _RecRenderParticles( GameObject* gameObject );
			void _RecRenderText3D	( GameObject* gameObject );

			void _BindMaterial		( Material* material );
		};
	}
}