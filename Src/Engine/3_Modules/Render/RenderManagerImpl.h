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
			int NewTechnique	( TechniqueType techType )				override;
			int NewTechnique	( Technique* technique )				override;
			int NewCanvas		( GLContext* glContext )				override;
			void ChangeRenderer	( int canvasID, RendererType type )	override;
			void ChangeRenderer	( int canvasID, Renderer* renderer )	override;
			void ChangeTechnique( int canvasID, int techID )			override;
			void Render			( int canvasID, Node* root, GameObject* cameraGO, 
								  bool useAllScenes )					override;
			void OnResize		( int canvasID )						override;


		public:
			void CreateMeshBuffers		( OUT int& vbo, OUT int& ibo,
										  DataArray<Vertex>& vertices, DataArray<int>& indices );
			void CreateMorphBuffers		( OUT int& vbo, OUT int& ibo,
										  DataArray<MorphVertex>& vertices, DataArray<int>& indices );
			void CreateParticleBuffers	( OUT int& vbo, OUT int& ibo,
										  DataArray<ParticleVertex>& vertices, DataArray<int>& indices );
			void DeleteBuffers			( int vbo, int ibo );
			int CreateShaderProgram	( const char* vertexCode, const char* geometryCode,
										  const char* fragmentCode );
			void DeleteShaderProgram	( int programID );
			int GetUniformLocation		( int programID, const char* uniform );
			int GetUniformsCount		( int programID );
			void GetUniformInfo			( int programID, int index, int buffSize,
										  OUT int* length, OUT char* name );
			void CreateTexture2D		( OUT int& textureID, int width, int height, 
										  int channels, bool hasMipmap,
										  uchar* imageBuffer );
			void CreateCubeMapTexture	( OUT int& textureID, int width, int height, 
										  int channels, bool hasMipmap,
										  uchar* imageBuffer_PosX,
										  uchar* imageBuffer_NegX,
										  uchar* imageBuffer_PosY,
										  uchar* imageBuffer_NegY,
										  uchar* imageBuffer_PosZ,
										  uchar* imageBuffer_NegZ );
			void DeleteTexture			( int textureID );


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

			bool _IsShaderCompiled	( int shaderID )	const;
			bool _IsProgramLinked	( int programID )	const;
		};
	}
}
