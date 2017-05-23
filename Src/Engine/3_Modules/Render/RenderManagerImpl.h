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
	namespace NotAPI
	{
		class RenderManagerImpl : public RenderManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( RenderManagerImpl )


		public:
			/*****
			* @desc: initialize manager by creating first canvas, with id 0
			* @params: 
			* 		glContext	- the context of the main window
			* 		gl			- the OpenGL API
			*/
			void Initialize		( GLContext* glContext, GL* gl )	override;

			/*****
			* @desc: destroy the manager
			* @assert: it was first initialized
			*/
			void Finalize		()									override;

			/*****
			* @desc: create a new canvas and activate it
			* @params: 
			* 		glContext - the context of the window
			* @return: the index of the new canvas
			*/
			int  CreateCanvas	( GLContext* glContext )			override;

			/*****
			* @desc: change the current canvas
			* @params: 
			* 		canvasID - id of the new canvas
			* @assert: canvas exists
			*/
			void SwitchCanvas	( int canvasID )					override;

			/*****
			* @desc: create a new renderer for current canvas
			* @params: 
			* 		rendererType - type of the renderer
			* @assert: canvas is set
			*/
			void CreateRenderer	( RendererType rendererType )		override;

			/*****
			* @desc: change the current renderer
			* @params: 
			* 		rendererType - type of the renderer
			* @assert: canvas is set
			*/
			void SwitchRenderer	( RendererType rendererType )		override;

			/*****
			* @desc: clear the previous frame
			*/
			void ClearScreen	( float r, float g, float b )		override;

			/*****
			* @desc: clear the depth buffer; new rendering goes over anything before
			*/
			void ResetDepth		()									override;

			/*****
			* @desc: render the given component from given gameobject and its children
			* @params: 
			* 		compType	- the component to render
			* 		target		- the target gameobject
			* 		cameraGO	- camera
			* 		lightsGO	- the list of lights to be used
			* @assert: canvas and renderer are set
			*/
			void Render			( ComponentType compType, Node* target, GameObject* cameraGO,
								  DataArray<GameObject*>& lightsGO )	override;

			/*****
			* @desc: display the rendering
			*/
			void SwapBuffers	()									override;

			/*****
			* @desc: force resize for given canvas
			* @params: 
			* 		canvasID - id of canvas
			* @assert: canvas exists
			*/
			void OnResize		( int canvasID )					override;


		public:
			void CreateMeshBuffers		( OUT uint& vbo, OUT uint& ibo,
										  DataArray<Vertex>& vertices, DataArray<int>& indices );
			void CreateMorphBuffers		( OUT uint& vbo, OUT uint& ibo,
										  DataArray<MorphVertex>& vertices, DataArray<int>& indices );
			void CreateParticleBuffers	( OUT uint& vbo, OUT uint& ibo,
										  DataArray<ParticleVertex>& vertices, DataArray<int>& indices );
			void DeleteBuffers			( uint vbo, uint ibo );
			int  CreateShaderProgram	( const char* vertexCode, const char* geometryCode,
										  const char* fragmentCode );
			void DeleteShaderProgram	( int programID );
			int  GetUniformLocation		( int programID, const char* uniform );
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
