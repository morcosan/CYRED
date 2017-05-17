// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "Technique.h"


namespace CYRED
{
	class GL_3_0;
	class Material;
	class Transform;
	class Camera;
	class Light;
}


namespace CYRED
{
	namespace NotAPI
	{
		class ForwardTechnique : public Technique
		{
		public:
			const char* UNIFORM_IS_LOOPING	= "isLooping";
			const char* UNIFORM_LIFETIME	= "lifetime";
			const char* UNIFORM_DELTA_TIME	= "deltaTime";

			const char* UNIFORM_STATE_RATIO	= "stateRatio";


		public:
			ForwardTechnique() {}
			virtual ~ForwardTechnique() {}


		public:
			virtual void Render( int* buffers, Node* root, GameObject* cameraGO, 
								 bool useAllScenes )	override;


		protected:
			virtual void _ClearScreen() override;


		protected:
			int	_mainFramebufferID;

			Transform*	_cameraTran;
			Camera*		_cameraComp;
			Light*		_cameraLight;

			void _RecRenderMesh		( GameObject* gameObject, bool useAllScenes );
			void _RecRenderMorph	( GameObject* gameObject, bool useAllScenes );
			void _RecRenderParticles( GameObject* gameObject, bool useAllScenes );

			void _BindMaterial		( Material* material );
		};
	}
}