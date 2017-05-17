// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredOpenGL.h"


namespace CYRED
{
	class GLImpl_3_0 : public GL
	{
	public:
		GLImpl_3_0();
		virtual ~GLImpl_3_0() {}

		void Initialize() override;
				   
		void Viewport	( int x, int y, int width, int height )	override;
		void ClearColor	( float r, float g, float b, float a )		override;
		void Clear		( int bitMask )							override;

		void GenBuffers		( int n, OUT int* buffers )			override;
		void DeleteBuffers	( int n, int* buffers )				override;
		void BindBuffer		( GLBuffer target, int buffer )		override;
		void BufferData		( GLBuffer target, int size, 
								const void* data, GLDrawType usage )	override;

		void GetShaderiv		( int shader, GLShaderInfo pname, 
							   	  OUT int* params )						override;
		void GetShaderInfoLog	( int shader, int maxLength, 
								  OUT int* size, OUT char* logs )		override;
	
		void	GetProgramiv		( int program, GLProgramInfo pname, 
									  OUT int* params )					override;
		void	GetProgramInfoLog	( int program, int maxLength, 
									  OUT int* size, OUT char* logs )	override;
		void	ValidateProgram		( int program )					override;
		int	CreateProgram		()									override;
		void	DeleteProgram		( int program )					override;
		int	CreateShader		( GLShader shaderType )				override;
		void	DeleteShader		( int shader )						override;
		void	ShaderSource		( int shader, int count, const char** string, 
									  const int* length  )				override;
		void	CompileShader		( int shader )						override;
		void	AttachShader		( int program, int shader )		override;
		void	DetachShader		( int program, int shader )		override;
		void	LinkProgram			( int program )					override;
		int		GetUniformLocation	( int program, const char* name )	override;
		void	GetActiveUniform	( int program, int index, int buffSize,
										OUT int* length, OUT int* size, OUT GLUniform& type, 
										OUT char* name )					override;
	
		void	GenFramebuffers		( int size, OUT int* framebuffers )		override;
		void	DeleteFramebuffers	( int size, int* framebuffers )			override;
		void	BindFramebuffer		( GLFrameBuffer target, int framebuffer )	override;
		
		void	GenTextures			( int size, OUT int* textures )			override;
		void	DeleteTextures		( int size, int* textures )				override;
		void	BindTexture			( GLTexture target, int texture )			override;
		void	TexImage2D			( GLTextureImage target, int level, 
										GLTexInternal internalformat,
 										int width, int height, int border, 
										GLTexFormat format, 
										GLVarType type, const void * data )		override;
		void	TexParameteri		( GLTexture target, GLTexParamType name, 
										GLTexParamValue param )					override;
		void	GenerateMipmap		( GLTexture target )						override;
		void	PixelStorei			( GLAlignType type, GLAlignValue value )	override;

		void	FramebufferTexture2D( GLFrameBuffer target, GLBufferAttachment attachment,	
										GLTexture texTarget, 
										int texture, int level )					override;
		
		GLBufferStatus CheckFramebufferStatus( GLFrameBuffer target )		override;

		void UseProgram				( int programID )						override;
		void EnableVertexAttribArray( int index )							override;
		void VertexAttribPointer	( int index, int size, GLVarType type, bool normalized, 
										int stride, const void* data  )		override;
		
		void Uniform1i	( int location, int value )						override;
		void Uniform1f	( int location, float value )					override;
		void Uniform2fv	( int location, int size, const float* value )	override;
		void Uniform3fv	( int location, int size, const float* value )	override;
		void Uniform4fv	( int location, int size, const float* value )	override;
		void UniformMatrix4fv	( int location, int count, bool transpose,
 								  const float *value )							override;
		void ActiveTexture	( int index )										override;

		void PolygonMode	( GLPolygonFace face, GLPolygonMode mode )			override;
		void Enable			( GLCapability capability )							override;
		void Disable		( GLCapability capability )							override;
		void CullFace		( GLCullFace mode )									override;
		
		void BlendEquation	( GLBlendMode mode )								override;
		void BlendFunc		( GLBlendFactor src, GLBlendFactor dst )			override;

		void DrawArrays		( GLDrawMode mode, int first, int size )			override;
		void DrawElements	( GLDrawMode mode, int count, GLVarType type, 
							  const void* indices )								override;

		void DepthFunc	( GLDepthFunc func )								override;
		void DepthMask	( bool flag )										override;

		void BindBufferBase	( GLBaseBuffer target, int index, 
							  int buffer )									override;


	protected:
		bool _isInitialized;
	};
}