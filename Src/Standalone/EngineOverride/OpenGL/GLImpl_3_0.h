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
				   
		void Viewport	( Int x, Int y, UInt width, UInt height )	override;
		void ClearColor	( Float r, Float g, Float b, Float a )		override;
		void Clear		( UInt bitMask )							override;

		void GenBuffers		( UInt n, OUT UInt* buffers )			override;
		void DeleteBuffers	( UInt n, UInt* buffers )				override;
		void BindBuffer		( GLBuffer target, UInt buffer )		override;
		void BufferData		( GLBuffer target, UInt size, 
								const void* data, GLDrawType usage )	override;

		void GetShaderiv		( UInt shader, GLShaderInfo pname, 
							   	  OUT Int* params )						override;
		void GetShaderInfoLog	( UInt shader, UInt maxLength, 
								  OUT Int* size, OUT Char* logs )		override;
	
		void	GetProgramiv		( UInt program, GLProgramInfo pname, 
									  OUT Int* params )					override;
		void	GetProgramInfoLog	( UInt program, UInt maxLength, 
									  OUT Int* size, OUT Char* logs )	override;
		void	ValidateProgram		( UInt program )					override;
		UInt	CreateProgram		()									override;
		void	DeleteProgram		( UInt program )					override;
		UInt	CreateShader		( GLShader shaderType )				override;
		void	DeleteShader		( UInt shader )						override;
		void	ShaderSource		( UInt shader, UInt count, const Char** string, 
									  const Int* length  )				override;
		void	CompileShader		( UInt shader )						override;
		void	AttachShader		( UInt program, UInt shader )		override;
		void	DetachShader		( UInt program, UInt shader )		override;
		void	LinkProgram			( UInt program )					override;
		Int		GetUniformLocation	( UInt program, const Char* name )	override;
		void	GetActiveUniform	( UInt program, UInt index, Int buffSize,
										OUT Int* length, OUT Int* size, OUT GLUniform& type, 
										OUT Char* name )					override;
	
		void	GenFramebuffers		( UInt size, OUT UInt* framebuffers )		override;
		void	DeleteFramebuffers	( UInt size, UInt* framebuffers )			override;
		void	BindFramebuffer		( GLFrameBuffer target, UInt framebuffer )	override;
		
		void	GenTextures			( UInt size, OUT UInt* textures )			override;
		void	DeleteTextures		( UInt size, UInt* textures )				override;
		void	BindTexture			( GLTexture target, UInt texture )			override;
		void	TexImage2D			( GLTextureImage target, Int level, 
										GLTexInternal internalformat,
 										UInt width, UInt height, Int border, 
										GLTexFormat format, 
										GLVarType type, const void * data )		override;
		void	TexParameteri		( GLTexture target, GLTexParamType name, 
										GLTexParamValue param )					override;
		void	GenerateMipmap		( GLTexture target )						override;
		void	PixelStorei			( GLAlignType type, GLAlignValue value )	override;

		void	FramebufferTexture2D( GLFrameBuffer target, GLBufferAttachment attachment,	
										GLTexture texTarget, 
										UInt texture, Int level )					override;
		
		GLBufferStatus CheckFramebufferStatus( GLFrameBuffer target )		override;

		void UseProgram				( UInt programID )						override;
		void EnableVertexAttribArray( UInt index )							override;
		void VertexAttribPointer	( UInt index, UInt size, GLVarType type, Bool normalized, 
										Int stride, const void* data  )		override;
		
		void Uniform1i	( Int location, Int value )						override;
		void Uniform1f	( Int location, Float value )					override;
		void Uniform2fv	( Int location, Int size, const Float* value )	override;
		void Uniform3fv	( Int location, Int size, const Float* value )	override;
		void Uniform4fv	( Int location, Int size, const Float* value )	override;
		void UniformMatrix4fv	( Int location, Int count, Bool transpose,
 								  const Float *value )							override;
		void ActiveTexture	( UInt index )										override;

		void PolygonMode	( GLPolygonFace face, GLPolygonMode mode )			override;
		void Enable			( GLCapability capability )							override;
		void Disable		( GLCapability capability )							override;
		void CullFace		( GLCullFace mode )									override;
		
		void BlendEquation	( GLBlendMode mode )								override;
		void BlendFunc		( GLBlendFactor src, GLBlendFactor dst )			override;

		void DrawArrays		( GLDrawMode mode, Int first, UInt size )			override;
		void DrawElements	( GLDrawMode mode, Int count, GLVarType type, 
							  const void* indices )								override;

		void DepthFunc	( GLDepthFunc func )								override;
		void DepthMask	( Bool flag )										override;

		void BindBufferBase	( GLBaseBuffer target, UInt index, 
							  UInt buffer )									override;


	protected:
		Bool _isInitialized;
	};
}