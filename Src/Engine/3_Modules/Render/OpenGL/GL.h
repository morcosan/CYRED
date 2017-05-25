// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"


namespace CYRED
{
	namespace Enum_GLBufferAttachment
	{
		enum Enum
		{
			COLOR_ATTACHMENT0
			, DEPTH_ATTACHMENT
		};
	}
	namespace Enum_GLTexParamValue
	{
		enum Enum
		{
			CLAMP_TO_EDGE
			, NEAREST
			, LINEAR
			, REPEAT
			, LINEAR_MIPMAP_LINEAR
		};
	}
	namespace Enum_GLTexParamType
	{
		enum Enum
		{
			WRAP_S
			, WRAP_T
			, WRAP_R
			, MAG_FILTER
			, MIN_FILTER
		};
	}
	namespace Enum_GLTexFormat
	{
		enum Enum
		{
			RGB
			, RGBA
			, DEPTH_COMPONENT
			, LUMINANCE
		};
	}
	namespace Enum_GLTexInternal
	{
		enum Enum
		{
			RGB
			, RGBA
			, RGBA32F
			, DEPTH_COMPONENT16
			, LUMINANCE
		};
	}
	namespace Enum_GLTexture
	{
		enum Enum
		{
			TEXTURE_2D
			, CUBE_MAP
		};
	}
	namespace Enum_GLTextureImage
	{
		enum Enum
		{
			TEXTURE_2D
			, CUBE_MAP_POSITIVE_X
			, CUBE_MAP_NEGATIVE_X
			, CUBE_MAP_POSITIVE_Y
			, CUBE_MAP_NEGATIVE_Y
			, CUBE_MAP_POSITIVE_Z
			, CUBE_MAP_NEGATIVE_Z
		};
	}
	namespace Enum_GLBufferStatus
	{
		enum Enum
		{
			COMPLETE
			, INCOMPLETE_ATTACHMENT
			, UNSUPPORTED
			, INCOMPLETE_MISSING_ATTACHMENT
		};
	}
	namespace Enum_GLShader
	{
		enum Enum
		{
			VERTEX_SHADER
			, GEOMETRY_SHADER 
			, FRAGMENT_SHADER
		};
	}
	namespace Enum_GLShaderInfo
	{
		enum Enum
		{
			COMPILE_STATUS
		};
	}
	namespace Enum_GLProgramInfo
	{
		enum Enum
		{
			LINK_STATUS
			, VALIDATE_STATUS
			, ACTIVE_UNIFORMS
		};
	}
	namespace Enum_GLBufferBit
	{
		enum Enum
		{
			COLOR_BUFFER_BIT = 0x1
			, DEPTH_BUFFER_BIT = 0x2
		};
	}
	namespace Enum_GLBuffer
	{
		enum Enum
		{
			ARRAY_BUFFER
			, ELEMENT_ARRAY_BUFFER
		};
	}
	namespace Enum_GLBaseBuffer
	{
		enum Enum
		{
			SHADER_STORAGE_BUFFER
		};
	}
	namespace Enum_GLFrameBuffer
	{
		enum Enum
		{
			FRAMEBUFFER
		};
	}
	namespace Enum_GLDrawType
	{
		enum Enum
		{
			STATIC_DRAW
		};
	}
	namespace Enum_GLVarType
	{
		enum Enum
		{
			FLOAT
			, UNSIGNED_BYTE
			, UNSIGNED_SHORT
			, UNSIGNED_INT
		};
	}
	namespace Enum_GLPolygonFace
	{
		enum Enum
		{
			FRONT_AND_BACK
		};
	}
	namespace Enum_GLPolygonMode
	{
		enum Enum
		{
			POINT
			, LINE
			, FILL
		};
	}
	namespace Enum_GLCapability
	{
		enum Enum
		{
			ALPHA_TEST
			, BLEND
			, CULL_FACE
			, DEPTH_TEST
		};
	}
	namespace Enum_GLDrawMode
	{
		enum Enum
		{
			TRIANGLES
			, LINES
			, POINTS
		};
	}
	namespace Enum_GLBlendMode
	{
		enum Enum
		{
			FUNC_ADD
			, FUNC_SUBTRACT
			, FUNC_REVERSE_SUBTRACT
		};
	}
	namespace Enum_GLCullFace
	{
		enum Enum
		{
			FRONT
			, BACK
			, FRONT_AND_BACK
		};
	}
	namespace Enum_GLUniform
	{
		enum Enum
		{
			FLOAT
			, FLOAT_VEC2
			, FLOAT_VEC3
			, FLOAT_VEC4
			, SAMPLER_2D
			, SAMPLER_CUBE 
			, BOOL
		};
	}
	namespace Enum_GLBlendFactor
	{
		enum Enum
		{
			ONE
			, ZERO
			, SRC_COLOR
			, ONE_MINUS_SRC_COLOR
			, DST_COLOR
			, ONE_MINUS_DST_COLOR
			, SRC_ALPHA
			, ONE_MINUS_SRC_ALPHA
			, DST_ALPHA
			, ONE_MINUS_DST_ALPHA
		};
	}
	namespace Enum_GLDepthFunc
	{
		enum Enum
		{
			NEVER
			, LESS
			, EQUAL
			, LEQUAL
			, GREATER
			, NOTEQUAL
			, GEQUAL
			, ALWAYS
		};
	}
	namespace Enum_GLAlignType
	{
		enum Enum
		{
			PACK_ALIGNMENT
			, UNPACK_ALIGNMENT
		};
	}
	namespace Enum_GLAlignValue
	{
		enum Enum
		{
			VALUE_1
			, VALUE_2
			, VALUE_4
			, VALUE_8
		};
	}
	namespace Enum_GLPixelFormat
	{
		enum Enum
		{
			RGB
			, RGBA
		};
	}

	typedef Enum_GLBuffer::Enum				GLBuffer;
	typedef Enum_GLDrawType::Enum			GLDrawType;
	typedef Enum_GLVarType::Enum			GLVarType;
	typedef Enum_GLPolygonFace::Enum		GLPolygonFace;
	typedef Enum_GLPolygonMode::Enum		GLPolygonMode;
	typedef Enum_GLCapability::Enum			GLCapability;
	typedef Enum_GLCullFace::Enum			GLCullFace;
	typedef Enum_GLDrawMode::Enum			GLDrawMode;
	typedef Enum_GLBlendMode::Enum			GLBlendMode;
	typedef Enum_GLBlendFactor::Enum		GLBlendFactor;
	typedef Enum_GLFrameBuffer::Enum		GLFrameBuffer;
	typedef Enum_GLBufferBit::Enum			GLBufferBit;
	typedef Enum_GLShaderInfo::Enum			GLShaderInfo;
	typedef Enum_GLProgramInfo::Enum		GLProgramInfo;
	typedef Enum_GLShader::Enum				GLShader;
	typedef Enum_GLBufferStatus::Enum		GLBufferStatus;
	typedef Enum_GLBufferAttachment::Enum	GLBufferAttachment;
	typedef Enum_GLTexParamType::Enum		GLTexParamType;
	typedef Enum_GLTexParamValue::Enum		GLTexParamValue;
	typedef Enum_GLTexFormat::Enum			GLTexFormat;
	typedef Enum_GLTexInternal::Enum		GLTexInternal;
	typedef Enum_GLTexture::Enum			GLTexture;
	typedef Enum_GLDepthFunc::Enum			GLDepthFunc;
	typedef	Enum_GLUniform::Enum			GLUniform;
	typedef Enum_GLTextureImage::Enum		GLTextureImage;
	typedef Enum_GLAlignType::Enum			GLAlignType;
	typedef Enum_GLAlignValue::Enum			GLAlignValue;
	typedef	Enum_GLBaseBuffer::Enum			GLBaseBuffer;
	typedef	Enum_GLPixelFormat::Enum		GLPixelFormat;
}


namespace CYRED
{
	ABSTRACT class DLL GL
	{
	public:
		GL() {}
		virtual ~GL() {};

		virtual void Initialize			()												PURE_VIRTUAL;

		virtual void Viewport			( int x, int y, int width, int height )			PURE_VIRTUAL;
		virtual void ClearColor			( float r, float g, float b, float a )			PURE_VIRTUAL;
		virtual void Clear				( int bitMask )									PURE_VIRTUAL;
		
		virtual void GenBuffers			( int n, OUT uint* buffers )					PURE_VIRTUAL;
		virtual void DeleteBuffers		( int n, uint* buffers )						PURE_VIRTUAL;
		virtual void BindBuffer			( GLBuffer target, int buffer )					PURE_VIRTUAL;
		virtual void BufferData			( GLBuffer target, int size, 
										  const void* data, GLDrawType usage )			PURE_VIRTUAL;

		virtual void GetShaderiv		( int shader, GLShaderInfo pname, 
										  int* params )									PURE_VIRTUAL;
		virtual void GetShaderInfoLog	( int shader, int maxLength, 
										  OUT int* size, OUT char* logs )				PURE_VIRTUAL;
	
		virtual void GetProgramiv		( int program, GLProgramInfo pname, 
										  OUT int* params )								PURE_VIRTUAL;
		virtual void GetProgramInfoLog	( int program, int maxLength, 
										  OUT int* size, OUT char* logs )				PURE_VIRTUAL;
		virtual void ValidateProgram	( int program )									PURE_VIRTUAL;
		virtual int	 CreateProgram		()												PURE_VIRTUAL;
		virtual void DeleteProgram		( int program )									PURE_VIRTUAL;
		virtual int	 CreateShader		( GLShader shaderType )							PURE_VIRTUAL;
		virtual void DeleteShader		( int shader )									PURE_VIRTUAL;
		virtual void ShaderSource		( int shader, int count, cchar** string, 
										  const int* length  )							PURE_VIRTUAL;
		virtual void CompileShader		( int shader )									PURE_VIRTUAL;
		virtual void AttachShader		( int program, int shader )						PURE_VIRTUAL;
		virtual void DetachShader		( int program, int shader )						PURE_VIRTUAL;
		virtual void LinkProgram		( int program )									PURE_VIRTUAL;
		virtual int	 GetUniformLocation	( int program, cchar* name )				PURE_VIRTUAL;
		virtual void GetActiveUniform	( int program, int index, int buffSize,
										  OUT int* length, OUT int* size, OUT GLUniform& type, 
										  OUT char* name )								PURE_VIRTUAL;
	
		virtual void GenFramebuffers	( int size, OUT uint* framebuffers )			PURE_VIRTUAL;
		virtual void DeleteFramebuffers	( int size, uint* framebuffers )				PURE_VIRTUAL;
		virtual void BindFramebuffer	( GLFrameBuffer target, int framebuffer )		PURE_VIRTUAL;
		
		virtual void GenTextures		( int size, OUT uint* textures )				PURE_VIRTUAL;
		virtual void DeleteTextures		( int size, uint* textures )					PURE_VIRTUAL;
		virtual void BindTexture		( GLTexture target, int texture )				PURE_VIRTUAL;
		virtual void TexImage2D			( GLTextureImage target, int level, GLTexInternal internalformat,
 										  int width, int height, int border, GLTexFormat format, 
										  GLVarType type, const void * data )			PURE_VIRTUAL;
		virtual void TexParameteri		( GLTexture target, GLTexParamType name, 
										  GLTexParamValue param )						PURE_VIRTUAL;
		virtual void GenerateMipmap		( GLTexture target )							PURE_VIRTUAL;
		virtual void PixelStorei		( GLAlignType type, GLAlignValue value )		PURE_VIRTUAL;

		virtual void FramebufferTexture2D( GLFrameBuffer target, GLBufferAttachment attachment,	
										  GLTexture textarget, int texture, 
										  int level )									PURE_VIRTUAL;
		
		virtual GLBufferStatus CheckFramebufferStatus( GLFrameBuffer target )			PURE_VIRTUAL;

		virtual void UseProgram				( int programID )							PURE_VIRTUAL;
		virtual void EnableVertexAttribArray( int index )								PURE_VIRTUAL;
		virtual void VertexAttribPointer	( int index, int size, GLVarType type, bool normalized, 
											  int stride, const void* data  )			PURE_VIRTUAL;
		
		virtual void Uniform1i			( int location, int value )						PURE_VIRTUAL;
		virtual void Uniform1f			( int location, float value )					PURE_VIRTUAL;
		virtual void Uniform2fv			( int location, int size, const float* value )	PURE_VIRTUAL;
		virtual void Uniform3fv			( int location, int size, const float* value )	PURE_VIRTUAL;
		virtual void Uniform4fv			( int location, int size, const float* value )	PURE_VIRTUAL;
		virtual void UniformMatrix4fv	( int location, int count, bool transpose,
 										  const float *value )							PURE_VIRTUAL;
		virtual void ActiveTexture		( int index )									PURE_VIRTUAL;

		virtual void PolygonMode		( GLPolygonFace face, GLPolygonMode mode )		PURE_VIRTUAL;
		virtual void LineWidth			( float value )									PURE_VIRTUAL;
		virtual void Enable				( GLCapability capability )						PURE_VIRTUAL;
		virtual void Disable			( GLCapability capability )						PURE_VIRTUAL;
		virtual void CullFace			( GLCullFace mode )								PURE_VIRTUAL;
		
		virtual void BlendEquation		( GLBlendMode mode )							PURE_VIRTUAL;
		virtual void BlendFunc			( GLBlendFactor src, GLBlendFactor dst )		PURE_VIRTUAL;

		virtual void DrawArrays			( GLDrawMode mode, int first, int size )		PURE_VIRTUAL;
		virtual void DrawElements		( GLDrawMode mode, int count, GLVarType type, 
										  const void* indices )							PURE_VIRTUAL;
	
		virtual void DepthFunc			( GLDepthFunc func )							PURE_VIRTUAL;
		virtual void DepthMask			( bool flag )									PURE_VIRTUAL;

		virtual void BindBufferBase		( GLBaseBuffer target, int index, 
										  int buffer )									PURE_VIRTUAL;

		virtual void ReadPixels			( int x, int y, int width, int height, GLPixelFormat format, 
										  GLVarType type, void* data )					PURE_VIRTUAL;

	};
}



