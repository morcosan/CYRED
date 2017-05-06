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
}


namespace CYRED
{
	namespace NotAPI
	{
		class ForwardTechnique : public Technique
		{
		public:
			const Char* UNIFORM_IS_LOOPING	= "isLooping";
			const Char* UNIFORM_LIFETIME	= "lifetime";
			const Char* UNIFORM_DELTA_TIME	= "deltaTime";

			const Char* UNIFORM_STATE_RATIO	= "stateRatio";


		public:
			ForwardTechnique() {}
			virtual ~ForwardTechnique() {}


		public:
			virtual void Render( UInt* buffers, Node* root, GameObject* cameraGO )	override;


		protected:
			virtual void _ClearScreen() override;


		protected:
			UInt	_mainFramebufferID;

			Transform*	_cameraTran;
			Camera*		_cameraComp;

			void _RecRenderMesh		( GameObject* gameObject );
			void _RecRenderMorph	( GameObject* gameObject );
			void _RecRenderParticles( GameObject* gameObject );

			void _BindMaterial		( Material* material );
		};
	}
}