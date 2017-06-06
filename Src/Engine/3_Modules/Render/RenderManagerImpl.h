// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
	namespace NonAPI
	{
		class RenderManagerImpl : public RenderManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( RenderManagerImpl )


		public:
			void	Initialize		( GLContext* glContext, GL* gl )		override;
			void	Finalize		()										override;
			
			int		CreateCanvas	( GLContext* glContext )				override;
			void	SwitchCanvas	( int canvasID )						override;
			void	CreateRenderer	( RendererType rendererType )			override;
			void	SwitchRenderer	( RendererType rendererType )			override;
			void	ClearScreen		( float r, float g, float b )			override;
			void	ResetDepth		()										override;
			void	Render			( ComponentType compType, Node* target, GameObject* cameraGO,
									  DataArray<GameObject*>& lightsGO )	override;
			void	SwapBuffers		()										override;
			void	OnResize		( int canvasID )						override;
			Vector4	ReadPixel		( int x, int y )						override;


		public:
			void CreateMeshBuffers		( OUT uint& vbo, OUT uint& ibo,
										  DataArray<Vertex>& vertices, DataArray<int>& indices );
			void CreateMorphBuffers		( OUT uint& vbo, OUT uint& ibo,
										  DataArray<MorphVertex>& vertices, DataArray<int>& indices );
			void CreateParticleBuffers	( OUT uint& vbo, OUT uint& ibo,
										  DataArray<ParticleVertex>& vertices, DataArray<int>& indices );
			void DeleteBuffers			( uint vbo, uint ibo );
			int  CreateShaderProgram	( cchar* vertexCode, cchar* geometryCode,
										  cchar* fragmentCode );
			void DeleteShaderProgram	( int programID );
			int  GetUniformLocation		( int programID, cchar* uniform );
			int  GetUniformsCount		( int programID );
			void GetUniformInfo			( int programID, int index, int buffSize,
										  OUT int* length, OUT char* name );
			void CreateTexture2D		( OUT uint& textureID, int width, int height, 
										  int channels, bool hasMipmap,
										  uchar* imageBuffer );
			void CreateCubeMapTexture	( OUT uint& textureID, int width, int height, 
										  int channels, bool hasMipmap,
										  uchar* imageBuffer_PosX,
										  uchar* imageBuffer_NegX,
										  uchar* imageBuffer_PosY,
										  uchar* imageBuffer_NegY,
										  uchar* imageBuffer_PosZ,
										  uchar* imageBuffer_NegZ );
			void DeleteTexture			( uint textureID );
			void CalculateTanBitangents	( DataArray<Vertex>& vertices, DataArray<int>& indices );


		private:
			struct _Canvas
			{
				GLContext*							glContext;	// encapsulation over window
				DataMap<RendererType, Renderer*>	renderers;	// list of renderers
			};

			int						_currCanvas;	// the id of current canvas
			RendererType			_currRenderer;	// the current renderer for current canvas
			DataArray<_Canvas>		_canvases;		// list of canvases
			GL*						_gl;			// GL API

			bool _IsShaderCompiled	( int shaderID )	const;
			bool _IsProgramLinked	( int programID )	const;
		};
	}
}
