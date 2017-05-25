// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "GLImpl_3_0.h"

#include "GLEW\Include\glew.h"


using namespace CYRED;


GLImpl_3_0::GLImpl_3_0()
	: _isInitialized( FALSE )
{
}


void GLImpl_3_0::Initialize()
{
	glewExperimental = true;     // This flag helps prevent errors with modern OpenGL
	GLenum glewError = glewInit();
	if ( glewError != GLEW_OK )
	{
		//( "glew init error", (cchar*) glewGetErrorString( glewError) );
	}

	_isInitialized = TRUE;
}


void GLImpl_3_0::Viewport( int x, int y, int width, int height )
{
	ASSERT( _isInitialized );
	glViewport( x, y, width, height );
}


void GLImpl_3_0::ClearColor( float r, float g, float b, float a )
{
	ASSERT( _isInitialized );
	glClearColor( r, g, b, a );
}


void GLImpl_3_0::Clear( int bitMask )
{
	ASSERT( _isInitialized );

	int glBitMask = 0;

	if ( (bitMask & GLBufferBit::COLOR_BUFFER_BIT) != 0 )
	{
		glBitMask |= GL_COLOR_BUFFER_BIT;
	}
	if ( (bitMask & GLBufferBit::DEPTH_BUFFER_BIT) != 0 )
	{
		glBitMask |= GL_DEPTH_BUFFER_BIT;
	}

	glClear( glBitMask );
}


void GLImpl_3_0::GenBuffers( int n, OUT int* buffers )
{
	ASSERT( _isInitialized );
	glGenBuffers( n, buffers );
}


void GLImpl_3_0::DeleteBuffers( int n, int* buffers )
{
	ASSERT( _isInitialized );

	glDeleteBuffers( n, buffers );
}


void GLImpl_3_0::BindBuffer( GLBuffer target, int buffer )
{
	ASSERT( _isInitialized );

	int glTarget = 0;

	switch ( target )
	{
		case GLBuffer::ARRAY_BUFFER:
			glTarget = GL_ARRAY_BUFFER;
			break;

		case GLBuffer::ELEMENT_ARRAY_BUFFER:
			glTarget = GL_ELEMENT_ARRAY_BUFFER;
			break;
	}

	glBindBuffer( glTarget, buffer );
}


void GLImpl_3_0::BufferData( GLBuffer target, int size, const void * data, GLDrawType usage )
{
	ASSERT( _isInitialized );

	int glTarget = 0;
	int glUsage = 0;

	switch ( target )
	{
		case GLBuffer::ARRAY_BUFFER:
			glTarget = GL_ARRAY_BUFFER;
			break;

		case GLBuffer::ELEMENT_ARRAY_BUFFER:
			glTarget = GL_ELEMENT_ARRAY_BUFFER;
			break;
	}

	switch ( usage )
	{
		case GLDrawType::STATIC_DRAW:
			glUsage = GL_STATIC_DRAW;
			break;
	}

	glBufferData( glTarget, size, data, glUsage );
}


void GLImpl_3_0::GetShaderiv( int shader, GLShaderInfo pname, OUT int* params )
{
	ASSERT( _isInitialized );

	switch ( pname )
	{
		case GLShaderInfo::COMPILE_STATUS:
			glGetShaderiv( shader, GL_COMPILE_STATUS, params );
			break;
	}
}


void GLImpl_3_0::GetShaderInfoLog( int shader, int maxLength, OUT int* size, OUT char* logs )
{
	ASSERT( _isInitialized );
	glGetShaderInfoLog( shader, maxLength, size, logs );
}


void GLImpl_3_0::GetProgramiv( int program, GLProgramInfo pname, OUT int* params )
{
	ASSERT( _isInitialized );

	switch ( pname )
	{
		case GLProgramInfo::LINK_STATUS:
			glGetProgramiv( program, GL_LINK_STATUS, params );
			break;

		case GLProgramInfo::VALIDATE_STATUS:
			glGetProgramiv( program, GL_VALIDATE_STATUS, params );
			break;

		case GLProgramInfo::ACTIVE_UNIFORMS:
			glGetProgramiv( program, GL_ACTIVE_UNIFORMS, params );
			break;
	}
}


void GLImpl_3_0::GetProgramInfoLog( int program, int maxLength, OUT int* size, OUT char* logs )
{
	ASSERT( _isInitialized );
	glGetProgramInfoLog( program, maxLength, size, logs );
}


void GLImpl_3_0::ValidateProgram( int program )
{
	ASSERT( _isInitialized );
	glValidateProgram( program );
}


int GLImpl_3_0::CreateProgram()
{
	ASSERT( _isInitialized );
	return glCreateProgram();
}


void GLImpl_3_0::DeleteProgram( int program )
{
	ASSERT( _isInitialized );
	glDeleteProgram( program );
}


int GLImpl_3_0::CreateShader( GLShader shaderType )
{
	ASSERT( _isInitialized );

	int glShaderType = 0;

	switch ( shaderType )
	{
		case GLShader::VERTEX_SHADER:
			glShaderType = GL_VERTEX_SHADER;
			break;

		case GLShader::GEOMETRY_SHADER:
			glShaderType = GL_GEOMETRY_SHADER;
			break;

		case GLShader::FRAGMENT_SHADER:
			glShaderType = GL_FRAGMENT_SHADER;
			break;
	}

	return glCreateShader( glShaderType );
}


void GLImpl_3_0::DeleteShader( int shader )
{
	ASSERT( _isInitialized );
	glDeleteShader( shader );
}


void GLImpl_3_0::ShaderSource( int shader, int count, cchar** string, const int* length )
{
	ASSERT( _isInitialized );
	glShaderSource( shader, count, string, length );
}


void GLImpl_3_0::CompileShader( int shader )
{
	ASSERT( _isInitialized );
	glCompileShader( shader );
}


void GLImpl_3_0::AttachShader( int program, int shader )
{
	ASSERT( _isInitialized );
	glAttachShader( program, shader );
}


void GLImpl_3_0::DetachShader( int program, int shader )
{
	ASSERT( _isInitialized );
	glDetachShader( program, shader );
}


void GLImpl_3_0::LinkProgram( int program )
{
	ASSERT( _isInitialized );
	glLinkProgram( program );
}


int GLImpl_3_0::GetUniformLocation( int program, cchar* name )
{
	ASSERT( _isInitialized );
	return glGetUniformLocation( program, name );
}


void GLImpl_3_0::GetActiveUniform( int program, int index, int buffSize, OUT int* length, 
								   OUT int* size, OUT GLUniform& type, OUT char* name )
{
	int glType = GL_ZERO;

	glGetActiveUniform( program, index, buffSize, length, size, &glType, name );

	switch ( glType )
	{
		case GL_BOOL:
			type = GLUniform::BOOL;
			break;

		case GL_FLOAT:
			type = GLUniform::FLOAT;
			break;

		case GL_FLOAT_VEC2:
			type = GLUniform::FLOAT_VEC2;
			break;

		case GL_FLOAT_VEC3:
			type = GLUniform::FLOAT_VEC3;
			break;

		case GL_FLOAT_VEC4:
			type = GLUniform::FLOAT_VEC4;
			break;

		case GL_SAMPLER_2D:
			type = GLUniform::SAMPLER_2D;
			break;

		case GL_SAMPLER_CUBE:
			type = GLUniform::SAMPLER_CUBE;
			break;
	}
}


void GLImpl_3_0::GenFramebuffers( int size, OUT int* framebuffers )
{
	ASSERT( _isInitialized );
	glGenFramebuffers( size, framebuffers );
}


void GLImpl_3_0::DeleteFramebuffers( int size, int* framebuffers )
{
	ASSERT( _isInitialized );
	glDeleteFramebuffers( size, framebuffers );
}


void GLImpl_3_0::BindFramebuffer( GLFrameBuffer target, int framebuffer )
{
	ASSERT( _isInitialized );

	switch ( target )
	{
		case GLFrameBuffer::FRAMEBUFFER:
			glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );
			break;
	}
}


void GLImpl_3_0::GenTextures( int size, OUT int* textures )
{
	ASSERT( _isInitialized );

	glGenTextures( size, textures );
}


void GLImpl_3_0::DeleteTextures( int size, int* textures )
{
	ASSERT( _isInitialized );

	glDeleteTextures( size, textures );
}


void GLImpl_3_0::BindTexture( GLTexture target, int texture )
{
	ASSERT( _isInitialized );

	switch ( target )
	{
		case GLTexture::TEXTURE_2D:
			glBindTexture( GL_TEXTURE_2D, texture );
			break;

		case GLTexture::CUBE_MAP:
			glBindTexture( GL_TEXTURE_CUBE_MAP, texture );
			break;
	}
}


void GLImpl_3_0::TexImage2D( GLTextureImage target, int level, GLTexInternal internalformat, 
							 int width, int height, int border, GLTexFormat format, 
							 GLVarType type, const void* data )
{
	ASSERT( _isInitialized );

	int glTarget = 0;
	int glInternalformat = 0;
	int glFormat = 0;
	int glType = 0;

	switch ( target )
	{
		case GLTextureImage::TEXTURE_2D:
			glTarget = GL_TEXTURE_2D;
			break;

		case GLTextureImage::CUBE_MAP_POSITIVE_X:
			glTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
			break;

		case GLTextureImage::CUBE_MAP_NEGATIVE_X:
			glTarget = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
			break;

		case GLTextureImage::CUBE_MAP_POSITIVE_Y:
			glTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
			break;

		case GLTextureImage::CUBE_MAP_NEGATIVE_Y:
			glTarget = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
			break;

		case GLTextureImage::CUBE_MAP_POSITIVE_Z:
			glTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
			break;

		case GLTextureImage::CUBE_MAP_NEGATIVE_Z:
			glTarget = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
			break;
	}

	switch ( internalformat )
	{
		case GLTexInternal::RGB:
			glInternalformat = GL_RGB;
			break;

		case GLTexInternal::RGBA:
			glInternalformat = GL_RGBA;
			break;

		case GLTexInternal::DEPTH_COMPONENT16:
			glInternalformat = GL_DEPTH_COMPONENT16;
			break;

		case GLTexInternal::LUMINANCE:
			glInternalformat = GL_LUMINANCE;
			break;
	}

	switch ( format )
	{
		case GLTexFormat::RGB:
			glFormat = GL_RGB;
			break;

		case GLTexFormat::RGBA:
			glFormat = GL_RGBA;
			break;

		case GLTexFormat::DEPTH_COMPONENT:
			glFormat = GL_DEPTH_COMPONENT;
			break;

		case GLTexFormat::LUMINANCE:
			glFormat = GL_LUMINANCE;
			break;
	}

	switch ( type )
	{
		case GLVarType::UNSIGNED_BYTE:
			glType = GL_UNSIGNED_BYTE;
			break;

		case GLVarType::UNSIGNED_SHORT:
			glType = GL_UNSIGNED_SHORT;
			break;
	}

	glTexImage2D( glTarget, level, glInternalformat, width, height, border, 
						 glFormat, glType, data );
}


void GLImpl_3_0::TexParameteri( GLTexture target, GLTexParamType name, GLTexParamValue param )
{
	ASSERT( _isInitialized );

	int glTarget = 0;
	int glName = 0;
	int glParam = 0;

	switch ( target )
	{
		case GLTexture::TEXTURE_2D:
			glTarget = GL_TEXTURE_2D;
			break;

		case GLTexture::CUBE_MAP:
			glTarget = GL_TEXTURE_CUBE_MAP;
			break;
	}

	switch ( name )
	{
		case GLTexParamType::WRAP_S:
			glName = GL_TEXTURE_WRAP_S;
			break;

		case GLTexParamType::WRAP_T:
			glName = GL_TEXTURE_WRAP_T;
			break;

		case GLTexParamType::WRAP_R:
			glName = GL_TEXTURE_WRAP_R;
			break;

		case GLTexParamType::MAG_FILTER:
			glName = GL_TEXTURE_MAG_FILTER;
			break;

		case GLTexParamType::MIN_FILTER:
			glName = GL_TEXTURE_MIN_FILTER;
			break;
	}

	switch ( param )
	{
		case GLTexParamValue::CLAMP_TO_EDGE:
			glParam = GL_CLAMP_TO_EDGE;
			break;

		case GLTexParamValue::NEAREST:
			glParam = GL_NEAREST;
			break;

		case GLTexParamValue::LINEAR:
			glParam = GL_LINEAR;
			break;

		case GLTexParamValue::REPEAT:
			glParam = GL_REPEAT;
			break;

		case GLTexParamValue::LINEAR_MIPMAP_LINEAR:
			glParam = GL_LINEAR_MIPMAP_LINEAR;
			break;
	}

	glTexParameteri( glTarget, glName, glParam );
}


void GLImpl_3_0::GenerateMipmap( GLTexture target )
{
	ASSERT( _isInitialized );

	switch ( target )
	{
		case GLTexture::TEXTURE_2D:
			glGenerateMipmap( GL_TEXTURE_2D );
			break;

		case GLTexture::CUBE_MAP:
			glGenerateMipmap( GL_TEXTURE_CUBE_MAP );
			break;
	}
}


void GLImpl_3_0::PixelStorei( GLAlignType type, GLAlignValue value )
{
	ASSERT( _isInitialized );

	int glType = 0;
	int glValue = 0;

	switch ( type )
	{
		case GLAlignType::PACK_ALIGNMENT:
			glType = GL_PACK_ALIGNMENT;
			break;

		case GLAlignType::UNPACK_ALIGNMENT:
			glType = GL_UNPACK_ALIGNMENT;
			break;
	}

	switch ( value )
	{
		case GLAlignValue::VALUE_1:
			glValue = 1;
			break;

		case GLAlignValue::VALUE_2:
			glValue = 2;
			break;

		case GLAlignValue::VALUE_4:
			glValue = 4;
			break;

		case GLAlignValue::VALUE_8:
			glValue = 8;
			break;
	}

	glPixelStorei( glType, glValue );
}


void GLImpl_3_0::FramebufferTexture2D( GLFrameBuffer target, GLBufferAttachment attachment, 
									   GLTexture texTarget, int texture, int level )
{
	ASSERT( _isInitialized );

	int glTarget = 0;
	int glAttachment = 0;
	int glTexTarget = 0;

	switch ( target )
	{
		case GLFrameBuffer::FRAMEBUFFER:
			glTarget = GL_FRAMEBUFFER;
			break;
	}

	switch ( attachment )
	{
		case GLBufferAttachment::COLOR_ATTACHMENT0:
			glAttachment = GL_COLOR_ATTACHMENT0;
			break;

		case GLBufferAttachment::DEPTH_ATTACHMENT:
			glAttachment = GL_DEPTH_ATTACHMENT;
			break;
	}

	switch ( texTarget )
	{
		case GLTexture::TEXTURE_2D:
			glTexTarget = GL_TEXTURE_2D;
			break;
	}

	glFramebufferTexture2D( glTarget, glAttachment, glTexTarget, texture, level );
}


GLBufferStatus GLImpl_3_0::CheckFramebufferStatus( GLFrameBuffer target )
{
	ASSERT( _isInitialized );

	int glTarget = 0;

	switch ( target )
	{
		case GLFrameBuffer::FRAMEBUFFER:
			glTarget = GL_FRAMEBUFFER;
			break;
	}

	int status = glCheckFramebufferStatus( glTarget );
	
	switch ( status )
	{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			return GLBufferStatus::INCOMPLETE_ATTACHMENT;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			return GLBufferStatus::INCOMPLETE_MISSING_ATTACHMENT;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			return GLBufferStatus::UNSUPPORTED;
	}

	return GLBufferStatus::COMPLETE;
}


void GLImpl_3_0::UseProgram( int programID )
{
	glUseProgram( programID );
}


void GLImpl_3_0::EnableVertexAttribArray( int index )
{
	glEnableVertexAttribArray( index );
}


void GLImpl_3_0::VertexAttribPointer( int index, int size, GLVarType type, bool normalized, 
									  int stride, const void* data )
{
	int glType = 0;

	switch ( type )
	{
		case GLVarType::FLOAT:
			glType = GL_FLOAT;
			break;
	}

	glVertexAttribPointer( index, size, glType, normalized, stride, data );
}


void GLImpl_3_0::Uniform1i( int location, int value )
{
	glUniform1i( location, value );
}


void GLImpl_3_0::Uniform1f( int location, float value )
{
	glUniform1f( location, value );
}


void GLImpl_3_0::Uniform2fv( int location, int size, const float* value )
{
	glUniform2fv( location, size, value );
}


void GLImpl_3_0::Uniform3fv( int location, int size, const float * value )
{
	glUniform3fv( location, size, value );
}


void GLImpl_3_0::Uniform4fv( int location, int size, const float * value )
{
	glUniform4fv( location, size, value );
}


void GLImpl_3_0::UniformMatrix4fv( int location, int count, bool transpose, const float* value )
{
	glUniformMatrix4fv( location, count, transpose, value );
}


void GLImpl_3_0::ActiveTexture( int index )
{
	glActiveTexture( GL_TEXTURE0 + index );
}


void GLImpl_3_0::PolygonMode( GLPolygonFace face, GLPolygonMode mode )
{
	int glFace = 0;
	int glMode = 0;

	switch ( face )
	{
		case GLPolygonFace::FRONT_AND_BACK:
			glFace = GL_FRONT_AND_BACK;
			break;
	}

	switch ( mode )
	{
		case GLPolygonMode::FILL:
			glMode = GL_FILL;
			break;

		case GLPolygonMode::LINE:
			glMode = GL_LINE;
			break;

		case GLPolygonMode::POINT:
			glMode = GL_POINT;
			break;
	}

	glPolygonMode( glFace, glMode );
}


void GLImpl_3_0::Enable( GLCapability capability )
{
	switch ( capability )
	{
		case GLCapability::ALPHA_TEST:
			glEnable( GL_ALPHA_TEST );
			break;

		case GLCapability::BLEND:
			glEnable( GL_BLEND );
			break;

		case GLCapability::CULL_FACE:
			glEnable( GL_CULL_FACE );
			break;

		case GLCapability::DEPTH_TEST:
			glEnable( GL_DEPTH_TEST );
			break;
	}
}


void GLImpl_3_0::Disable( GLCapability capability )
{
	switch ( capability )
	{
		case GLCapability::ALPHA_TEST:
			glDisable( GL_ALPHA_TEST );
			break;

		case GLCapability::BLEND:
			glDisable( GL_BLEND );
			break;

		case GLCapability::CULL_FACE:
			glDisable( GL_CULL_FACE );
			break;

		case GLCapability::DEPTH_TEST:
			glDisable( GL_DEPTH_TEST );
			break;
	}
}


void GLImpl_3_0::CullFace( GLCullFace mode )
{
	switch ( mode )
	{
		case GLCullFace::BACK:
			glCullFace( GL_BACK );
			break;

		case GLCullFace::FRONT:
			glCullFace( GL_FRONT );
			break;

		case GLCullFace::FRONT_AND_BACK:
			glCullFace( GL_FRONT_AND_BACK );
			break;
	}
}


void GLImpl_3_0::BlendEquation( GLBlendMode mode )
{
	switch ( mode )
	{
		case GLBlendMode::FUNC_ADD:
			glBlendEquation( GL_FUNC_ADD );
			break;

		case GLBlendMode::FUNC_REVERSE_SUBTRACT:
			glBlendEquation( GL_FUNC_REVERSE_SUBTRACT );
			break;

		case GLBlendMode::FUNC_SUBTRACT:
			glBlendEquation( GL_FUNC_SUBTRACT );
			break;
	}
}


void GLImpl_3_0::BlendFunc( GLBlendFactor src, GLBlendFactor dst )
{
	int glSrc = 0;
	int glDst = 0;

	switch ( src )
	{
		case GLBlendFactor::DST_ALPHA:
			glSrc = GL_DST_ALPHA;
			break;

		case GLBlendFactor::DST_COLOR:
			glSrc = GL_DST_COLOR;
			break;

		case GLBlendFactor::ONE:
			glSrc = GL_ONE;
			break;

		case GLBlendFactor::ONE_MINUS_DST_ALPHA:
			glSrc = GL_ONE_MINUS_DST_ALPHA;
			break;

		case GLBlendFactor::ONE_MINUS_DST_COLOR:
			glSrc = GL_ONE_MINUS_DST_COLOR;
			break;

		case GLBlendFactor::ONE_MINUS_SRC_ALPHA:
			glSrc = GL_ONE_MINUS_SRC_ALPHA;
			break;

		case GLBlendFactor::ONE_MINUS_SRC_COLOR:
			glSrc = GL_ONE_MINUS_SRC_COLOR;
			break;

		case GLBlendFactor::SRC_ALPHA:
			glSrc = GL_SRC_ALPHA;
			break;

		case GLBlendFactor::SRC_COLOR:
			glSrc = GL_SRC_COLOR;
			break;

		case GLBlendFactor::ZERO:
			glSrc = GL_ZERO;
			break;
	}

	switch ( dst )
	{
		case GLBlendFactor::DST_ALPHA:
			glDst = GL_DST_ALPHA;
			break;

		case GLBlendFactor::DST_COLOR:
			glDst = GL_DST_COLOR;
			break;

		case GLBlendFactor::ONE:
			glDst = GL_ONE;
			break;

		case GLBlendFactor::ONE_MINUS_DST_ALPHA:
			glDst = GL_ONE_MINUS_DST_ALPHA;
			break;

		case GLBlendFactor::ONE_MINUS_DST_COLOR:
			glDst = GL_ONE_MINUS_DST_COLOR;
			break;

		case GLBlendFactor::ONE_MINUS_SRC_ALPHA:
			glDst = GL_ONE_MINUS_SRC_ALPHA;
			break;

		case GLBlendFactor::ONE_MINUS_SRC_COLOR:
			glDst = GL_ONE_MINUS_SRC_COLOR;
			break;

		case GLBlendFactor::SRC_ALPHA:
			glDst = GL_SRC_ALPHA;
			break;

		case GLBlendFactor::SRC_COLOR:
			glDst = GL_SRC_COLOR;
			break;

		case GLBlendFactor::ZERO:
			glDst = GL_ZERO;
			break;
	}

	glBlendFunc( glSrc, glDst );
}


void GLImpl_3_0::DrawArrays( GLDrawMode mode, int first, int size )
{
	switch ( mode )
	{
		case GLDrawMode::TRIANGLES:
			glDrawArrays( GL_TRIANGLES, first, size );
			break;
	}
}


void GLImpl_3_0::DrawElements( GLDrawMode mode, int count, GLVarType type, const void* indices )
{
	int glMode = 0;
	int glType = 0;

	switch ( mode )
	{
		case GLDrawMode::TRIANGLES:
			glMode = GL_TRIANGLES;
			break;

		case GLDrawMode::LINES:
			glMode = GL_LINES;
			break;

		case GLDrawMode::POINTS:
			glMode = GL_POINTS;
			break;
	}

	switch ( type )
	{
		case GLVarType::UNSIGNED_INT:
			glType = GL_UNSIGNED_INT;
			break;
	}

	glDrawElements( glMode, count, glType, indices );
}


void GLImpl_3_0::DepthFunc( GLDepthFunc func )
{
	switch ( func )
	{
		case GLDepthFunc::ALWAYS:
			glDepthFunc( GL_ALWAYS );
			break;

		case GLDepthFunc::EQUAL:
			glDepthFunc( GL_EQUAL );
			break;

		case GLDepthFunc::GEQUAL:
			glDepthFunc( GL_GEQUAL );
			break;

		case GLDepthFunc::GREATER:
			glDepthFunc( GL_GREATER );
			break;

		case GLDepthFunc::LEQUAL:
			glDepthFunc( GL_LEQUAL );
			break;

		case GLDepthFunc::LESS:
			glDepthFunc( GL_LESS );
			break;

		case GLDepthFunc::NEVER:
			glDepthFunc( GL_NEVER );
			break;

		case GLDepthFunc::NOTEQUAL:
			glDepthFunc( GL_NOTEQUAL );
			break;
	}
}


void GLImpl_3_0::DepthMask( bool flag )
{
	glDepthMask( flag );
}


void GLImpl_3_0::BindBufferBase( GLBaseBuffer target, int index, int buffer )
{
	switch ( target )
	{
		case GLBaseBuffer::SHADER_STORAGE_BUFFER:
			glBindBufferBase( GL_SHADER_STORAGE_BUFFER, index, buffer );
			break;
	}
}





