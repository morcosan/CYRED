// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "RenderManager.h"
#include "../../2_BuildingBlocks/Data/DataMap.h"
#include "../../2_BuildingBlocks/Data/DataArray.h"
#include "OpenGL\Vertex.h"
#include "OpenGL\MorphVertex.h"
#include "OpenGL\ParticleVertex.h"


namespace CYRED
{
	class Renderer;
}


namespace CYRED
{
	namespace NotAPI
	{
		class RenderManagerImpl : public RenderManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( RenderManagerImpl )


		public:
			void Initialize		( GLContext* glContext, GL* gl )		override;
			void Finalize		()										override;
			UInt NewTechnique	( TechniqueType techType )				override;
			UInt NewTechnique	( Technique* technique )				override;
			UInt NewCanvas		( GLContext* glContext )				override;
			void ChangeRenderer	( UInt canvasID, RendererType type )	override;
			void ChangeRenderer	( UInt canvasID, Renderer* renderer )	override;
			void ChangeTechnique( UInt canvasID, UInt techID )			override;
			void Render			( UInt canvasID, Node* root, 
								  GameObject* cameraGO )				override;
			void OnResize		( UInt canvasID )						override;


		public:
			void CreateMeshBuffers		( OUT UInt& vbo, OUT UInt& ibo,
										  DataArray<Vertex>& vertices, DataArray<UInt>& indices );
			void CreateMorphBuffers		( OUT UInt& vbo, OUT UInt& ibo,
										  DataArray<MorphVertex>& vertices, DataArray<UInt>& indices );
			void CreateParticleBuffers	( OUT UInt& vbo, OUT UInt& ibo,
										  DataArray<ParticleVertex>& vertices, DataArray<UInt>& indices );
			void DeleteBuffers			( UInt vbo, UInt ibo );
			UInt CreateShaderProgram	( const Char* vertexCode, const Char* geometryCode,
										  const Char* fragmentCode );
			void DeleteShaderProgram	( UInt programID );
			UInt GetUniformLocation		( UInt programID, const Char* uniform );
			UInt GetUniformsCount		( UInt programID );
			void GetUniformInfo			( UInt programID, UInt index, Int buffSize,
										  OUT Int* length, OUT Char* name );
			void CreateTexture2D		( OUT UInt& textureID, UInt width, UInt height, 
										  UInt channels, Bool hasMipmap,
										  UChar* imageBuffer );
			void CreateCubeMapTexture	( OUT UInt& textureID, UInt width, UInt height, 
										  UInt channels, Bool hasMipmap,
										  UChar* imageBuffer_PosX,
										  UChar* imageBuffer_NegX,
										  UChar* imageBuffer_PosY,
										  UChar* imageBuffer_NegY,
										  UChar* imageBuffer_PosZ,
										  UChar* imageBuffer_NegZ );
			void DeleteTexture			( UInt textureID );


		protected:
			struct _Canvas
			{
				GLContext*	glContext;
				Renderer*	renderer;
			};

			DataArray<_Canvas>		_canvases;
			GL*						_gl;
			DataArray<Technique*>	_techniques;

			void _SetMainCanvas( GLContext* glContext );

			Bool _IsShaderCompiled	( UInt shaderID )	const;
			Bool _IsProgramLinked	( UInt programID )	const;
		};
	}
}
