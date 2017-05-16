// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "RenderManagerImpl.h"

#include "OpenGL\GL.h"
#include "OpenGL\GLContext.h"

#include "Renderers\ForwardRenderer.h"

#include "Techniques\ForwardTechnique.h"

#include "../Debug/DebugManager.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition of RenderManager
DEFINE_LOCAL_SINGLETON( RenderManager, RenderManagerImpl )

DEFINE_LOCAL_SINGLETON_IMPL( RenderManagerImpl )



void RenderManagerImpl::Initialize( GLContext* glContext, GL* gl )
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	_SetMainCanvas( glContext );

	_Canvas& canvas = _canvases[0];		// use the first canvas for initialization
	canvas.glContext->MakeCurrent();

	_gl = gl;
	// before calling it, a context must be active and the window must be displayed
	_gl->Initialize();
}


void RenderManagerImpl::Finalize()
{
	if ( !_isInitialized )
	{
		return;
	}
}


void RenderManagerImpl::_SetMainCanvas( GLContext* glContext )
{
	ASSERT( glContext != NULL );

	_Canvas canvas;
	canvas.glContext = glContext;
	canvas.glContext->Create();
	canvas.renderer = NULL;

	_canvases.Add( canvas );
}


Bool RenderManagerImpl::_IsShaderCompiled( UInt shaderID ) const
{
	Int comp;
	_gl->GetShaderiv( shaderID, GLShaderInfo::COMPILE_STATUS, &comp );

	if( comp == FALSE )
	{
		Char description[512];
		_gl->GetShaderInfoLog( shaderID, sizeof(description), 0, &description[0] );

		DebugManager::Singleton()->Log( description );

		return FALSE;
	}

	return TRUE;
}


Bool RenderManagerImpl::_IsProgramLinked( UInt programID ) const
{
	Int status;
	{
		_gl->GetProgramiv( programID, GLProgramInfo::LINK_STATUS, &status );

		if( status == FALSE )
		{
			Char description[512];
			_gl->GetProgramInfoLog( programID, sizeof(description), 0, &description[0] );

			DebugManager::Singleton()->Log( description );

			return FALSE;
		}
	}
	{
		_gl->ValidateProgram( programID );
		_gl->GetProgramiv( programID, GLProgramInfo::VALIDATE_STATUS, &status );
  	
		if( status == FALSE )
		{
			Char description[256];
			_gl->GetProgramInfoLog( programID, sizeof(description), 0, &description[0] );

			DebugManager::Singleton()->Log( description );
		
			return FALSE;
		}
	}

	return TRUE;
}


UInt RenderManagerImpl::NewTechnique( TechniqueType techType )
{
	ASSERT( _isInitialized );

	switch ( techType )
	{
	case TechniqueType::FORWARD_BASIC:
		return NewTechnique( Memory::Alloc<ForwardTechnique>() );
	}

	ASSERT( false ); //! should never get here
	return -1;
}


UInt RenderManagerImpl::NewTechnique( Technique* technique )
{
	ASSERT( _isInitialized );
	ASSERT( technique != NULL );

	return _techniques.Add( technique );
}


UInt RenderManagerImpl::NewCanvas( GLContext* glContext )
{
	ASSERT( _isInitialized );
	ASSERT( glContext != NULL );

	_Canvas canvas;
	canvas.glContext = glContext;
	canvas.glContext->SetSharedContext( _canvases[0].glContext );
	canvas.glContext->Create();
	canvas.renderer = NULL;

	return _canvases.Add( canvas );
}


void RenderManagerImpl::ChangeRenderer( UInt canvasID, RendererType type )
{
	ASSERT( _isInitialized );

	switch ( type )
	{
	case RendererType::GL_FORWARD:
		ChangeRenderer( canvasID, Memory::Alloc<ForwardRenderer>() );
		break;
	}
}


void RenderManagerImpl::ChangeRenderer( UInt canvasID, Renderer* renderer )
{
	ASSERT( _isInitialized );
	ASSERT( canvasID < _canvases.Size() );
	ASSERT( renderer != NULL );

	_Canvas& canvas = _canvases[canvasID];
	Memory::Free( canvas.renderer );
	canvas.renderer = renderer;
	canvas.renderer->Initialize( _gl, canvas.glContext );
}


void RenderManagerImpl::ChangeTechnique( UInt canvasID, UInt techID )
{
	ASSERT( _isInitialized );
	ASSERT( canvasID < _canvases.Size() );
	ASSERT( techID < _techniques.Size() );

	_Canvas& canvas = _canvases[canvasID];

	ASSERT( canvas.renderer != NULL );

	canvas.renderer->SetTechnique( _techniques[ techID ] );
}


void RenderManagerImpl::Render( UInt canvasID, Node* root, GameObject* cameraGO )
{
	ASSERT( _isInitialized );
	ASSERT( canvasID < _canvases.Size() );

	_Canvas& canvas = _canvases[canvasID];

	ASSERT( canvas.glContext != NULL );
	ASSERT( canvas.renderer != NULL );

	canvas.glContext->MakeCurrent();

	canvas.renderer->Render( root, cameraGO );
	canvas.renderer->DisplayOnScreen();

	canvas.glContext->SwapBuffers();
}


void RenderManagerImpl::OnResize( UInt canvasID )
{
	ASSERT( _isInitialized );
	ASSERT( canvasID < _canvases.Size() );

	_Canvas& canvas = _canvases[ canvasID ];

	ASSERT( canvas.glContext != NULL );
	canvas.glContext->OnResize();

	if ( canvas.renderer != NULL )
	{
		canvas.renderer->OnResize();
	}
}


void RenderManagerImpl::CreateMeshBuffers( OUT UInt& vbo, OUT UInt& ibo, 
										   DataArray<Vertex>& vertices, DataArray<UInt>& indices )
{
	ASSERT( _isInitialized );

	if ( vertices.Size() == 0 || indices.Size() == 0 )
	{
		return;
	}

	if ( vbo == EMPTY_BUFFER )
	{
		_gl->GenBuffers( 1, &vbo );
	}
  	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, vbo );
	_gl->BufferData( GLBuffer::ARRAY_BUFFER, sizeof(Vertex) * vertices.Size(), vertices.Data(), 
					 GLDrawType::STATIC_DRAW );

	if ( ibo == EMPTY_BUFFER )
	{
		_gl->GenBuffers( 1, &ibo );
	}
    _gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER, ibo );
	_gl->BufferData( GLBuffer::ELEMENT_ARRAY_BUFFER, sizeof(UInt) * indices.Size(), indices.Data(), 
					 GLDrawType::STATIC_DRAW );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			EMPTY_BUFFER );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	EMPTY_BUFFER );
}


void RenderManagerImpl::CreateMorphBuffers( OUT UInt& vbo, OUT UInt& ibo, 
										    DataArray<MorphVertex>& vertices, DataArray<UInt>& indices )
{
	ASSERT( _isInitialized );

	if ( vertices.Size() == 0 || indices.Size() == 0 )
	{
		return;
	}

	if ( vbo == EMPTY_BUFFER )
	{
		_gl->GenBuffers( 1, &vbo );
	}
  	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, vbo );
	_gl->BufferData( GLBuffer::ARRAY_BUFFER, sizeof(MorphVertex) * vertices.Size(), vertices.Data(), 
					 GLDrawType::STATIC_DRAW );

	if ( ibo == EMPTY_BUFFER )
	{
		_gl->GenBuffers( 1, &ibo );
	}
    _gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER, ibo );
	_gl->BufferData( GLBuffer::ELEMENT_ARRAY_BUFFER, sizeof(UInt) * indices.Size(), indices.Data(), 
					 GLDrawType::STATIC_DRAW );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			EMPTY_BUFFER );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	EMPTY_BUFFER );
}


void RenderManagerImpl::DeleteBuffers( UInt vbo, UInt ibo )
{
	_gl->DeleteBuffers( 1, &vbo );
	_gl->DeleteBuffers( 1, &ibo );
}


void RenderManagerImpl::CreateParticleBuffers( OUT UInt& vbo, OUT UInt& ibo, 
											   DataArray<ParticleVertex>& vertices, 
											   DataArray<UInt>& indices )
{
	ASSERT( _isInitialized );

	if ( vertices.Size() == 0 || indices.Size() == 0 )
	{
		return;
	}

	if ( vbo == EMPTY_BUFFER )
	{
		_gl->GenBuffers( 1, &vbo );
	}
  	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, vbo );
	_gl->BufferData( GLBuffer::ARRAY_BUFFER, sizeof(ParticleVertex) * vertices.Size(), vertices.Data(), 
					 GLDrawType::STATIC_DRAW );

	if ( ibo == EMPTY_BUFFER )
	{
		_gl->GenBuffers( 1, &ibo );
	}
    _gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER, ibo );
	_gl->BufferData( GLBuffer::ELEMENT_ARRAY_BUFFER, sizeof(UInt) * indices.Size(), indices.Data(), 
					 GLDrawType::STATIC_DRAW );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			EMPTY_BUFFER );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	EMPTY_BUFFER );
}


UInt RenderManagerImpl::CreateShaderProgram( const Char* vertexCode, 
											 const Char* geometryCode, 
											 const Char* fragmentCode )
{
	ASSERT( _isInitialized );
	ASSERT( vertexCode != NULL );
	ASSERT( geometryCode != NULL );
	ASSERT( fragmentCode != NULL );

    UInt programID = _gl->CreateProgram();
	Bool isOk = TRUE;
	UInt vertexShader	= _gl->CreateShader( GLShader::VERTEX_SHADER );
	UInt geometryShader = _gl->CreateShader( GLShader::GEOMETRY_SHADER );
	UInt fragmentShader = _gl->CreateShader( GLShader::FRAGMENT_SHADER );

	{
		_gl->ShaderSource( vertexShader, 1, &vertexCode, NULL );
		_gl->CompileShader( vertexShader );

		if ( !_IsShaderCompiled( vertexShader ) )
		{
			isOk = FALSE;
		}
	}

	if ( isOk )
	{
		_gl->ShaderSource( geometryShader, 1, &geometryCode, NULL );
		_gl->CompileShader( geometryShader );

		if ( !_IsShaderCompiled( geometryShader ) )
		{
			isOk = FALSE;
		}
	}

	if ( isOk )
	{
		_gl->ShaderSource( fragmentShader, 1, &fragmentCode, NULL );
		_gl->CompileShader( fragmentShader );

		if ( !_IsShaderCompiled( fragmentShader ) )
		{
			isOk = FALSE;
		}
	}

	if ( isOk )
	{
		_gl->AttachShader( programID, vertexShader );
		_gl->AttachShader( programID, geometryShader );
		_gl->AttachShader( programID, fragmentShader );

		_gl->LinkProgram( programID );
		if ( !_IsProgramLinked( programID ) )
		{
			isOk = FALSE;
		}

		// cleanup
		_gl->DetachShader( programID, vertexShader );
		_gl->DetachShader( programID, geometryShader );
		_gl->DetachShader( programID, fragmentShader );
	}

	// cleanup
	_gl->DeleteShader( vertexShader );
	_gl->DeleteShader( geometryShader );
	_gl->DeleteShader( fragmentShader );

	if ( isOk )
	{
		return programID;
	}
	else
	{
		_gl->DeleteProgram( programID );
		return INVALID_SHADER;
	}
}


void RenderManagerImpl::DeleteShaderProgram( UInt programID )
{
	_gl->DeleteProgram( programID );
}


UInt RenderManagerImpl::GetUniformLocation( UInt programID, const Char * uniform )
{
	ASSERT( _isInitialized );
	return _gl->GetUniformLocation( programID, uniform );
}


UInt RenderManagerImpl::GetUniformsCount( UInt programID )
{
	ASSERT( _isInitialized );

	Int total = 0;
	_gl->GetProgramiv( programID, GLProgramInfo::ACTIVE_UNIFORMS, &total ); 

	return total;
}


void RenderManagerImpl::GetUniformInfo( UInt programID, UInt index, Int buffSize,
										OUT Int* length, OUT Char* name )
{
	ASSERT( _isInitialized );

	Int size = -1;
	GLUniform type;

	_gl->GetActiveUniform( programID, index, buffSize, length, &size, type, name );
}


void RenderManagerImpl::CreateTexture2D( OUT UInt& textureID, UInt width, UInt height, 
										 UInt channels, Bool hasMipmap, 
										 UChar* imageBuffer )
{
	ASSERT( _isInitialized );

	if ( width == 0 || height == 0 || channels == 0 ) {
		return;
	}

	if ( textureID == INVALID_TEXTURE )	{
		_gl->GenTextures( 1, &textureID );
	}
	_gl->BindTexture( GLTexture::TEXTURE_2D, textureID );

	_gl->PixelStorei( GLAlignType::UNPACK_ALIGNMENT, GLAlignValue::VALUE_1 );

	if ( channels == 1 ) {
		_gl->TexImage2D( GLTextureImage::TEXTURE_2D, 0, GLTexInternal::LUMINANCE, width, height, 0, 
						 GLTexFormat::LUMINANCE, GLVarType::UNSIGNED_BYTE, imageBuffer );
	}

	if ( channels == 3 ) {
		_gl->TexImage2D( GLTextureImage::TEXTURE_2D, 0, GLTexInternal::RGB, width, height, 0, 
						 GLTexFormat::RGB, GLVarType::UNSIGNED_BYTE, imageBuffer );
	}

	if ( channels == 4 ) {
		_gl->TexImage2D( GLTextureImage::TEXTURE_2D, 0, GLTexInternal::RGBA, width, height, 0, 
						 GLTexFormat::RGBA, GLVarType::UNSIGNED_BYTE, imageBuffer );
	}

	_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::WRAP_S, GLTexParamValue::REPEAT );
	_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::WRAP_T, GLTexParamValue::REPEAT );

	if ( hasMipmap ) {
		_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::MAG_FILTER, GLTexParamValue::LINEAR_MIPMAP_LINEAR );
		_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::MIN_FILTER, GLTexParamValue::LINEAR_MIPMAP_LINEAR );

		_gl->GenerateMipmap( GLTexture::TEXTURE_2D );
	}
	else {
		_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::MAG_FILTER, GLTexParamValue::LINEAR );
		_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::MIN_FILTER, GLTexParamValue::LINEAR );
	}
}


void RenderManagerImpl::CreateCubeMapTexture( OUT UInt& textureID, UInt width, UInt height, 
											  UInt channels, Bool hasMipmap,
											  UChar* imageBuffer_PosX, UChar* imageBuffer_NegX, 
											  UChar* imageBuffer_PosY, UChar* imageBuffer_NegY, 
											  UChar* imageBuffer_PosZ, UChar* imageBuffer_NegZ )
{
	ASSERT( _isInitialized );

	if ( width == 0 || height == 0 || channels == 0 )
	{
		return;
	}

	if ( textureID == INVALID_TEXTURE )
	{
		_gl->GenTextures( 1, &textureID );
	}
	_gl->BindTexture( GLTexture::CUBE_MAP, textureID );

	_gl->PixelStorei( GLAlignType::UNPACK_ALIGNMENT, GLAlignValue::VALUE_1 );

	if ( channels == 3 ) 
	{
		_gl->TexImage2D( GLTextureImage::CUBE_MAP_POSITIVE_X, 0, GLTexInternal::RGB, width, height, 0, 
						 GLTexFormat::RGB, GLVarType::UNSIGNED_BYTE, imageBuffer_PosX );
		_gl->TexImage2D( GLTextureImage::CUBE_MAP_NEGATIVE_X, 0, GLTexInternal::RGB, width, height, 0, 
						 GLTexFormat::RGB, GLVarType::UNSIGNED_BYTE, imageBuffer_NegX );
		
		_gl->TexImage2D( GLTextureImage::CUBE_MAP_POSITIVE_Y, 0, GLTexInternal::RGB, width, height, 0, 
						 GLTexFormat::RGB, GLVarType::UNSIGNED_BYTE, imageBuffer_PosY );
		_gl->TexImage2D( GLTextureImage::CUBE_MAP_NEGATIVE_Y, 0, GLTexInternal::RGB, width, height, 0, 
						 GLTexFormat::RGB, GLVarType::UNSIGNED_BYTE, imageBuffer_NegY );
		
		_gl->TexImage2D( GLTextureImage::CUBE_MAP_POSITIVE_Z, 0, GLTexInternal::RGB, width, height, 0, 
						 GLTexFormat::RGB, GLVarType::UNSIGNED_BYTE, imageBuffer_PosZ );
		_gl->TexImage2D( GLTextureImage::CUBE_MAP_NEGATIVE_Z, 0, GLTexInternal::RGB, width, height, 0, 
						 GLTexFormat::RGB, GLVarType::UNSIGNED_BYTE, imageBuffer_NegZ );
	}

	if ( channels == 4 ) 
	{
		_gl->TexImage2D( GLTextureImage::CUBE_MAP_POSITIVE_X, 0, GLTexInternal::RGBA, width, height, 0, 
						 GLTexFormat::RGBA, GLVarType::UNSIGNED_BYTE, imageBuffer_PosX );
		_gl->TexImage2D( GLTextureImage::CUBE_MAP_NEGATIVE_X, 0, GLTexInternal::RGBA, width, height, 0, 
						 GLTexFormat::RGBA, GLVarType::UNSIGNED_BYTE, imageBuffer_NegX );
					
		_gl->TexImage2D( GLTextureImage::CUBE_MAP_POSITIVE_Y, 0, GLTexInternal::RGBA, width, height, 0, 
						 GLTexFormat::RGBA, GLVarType::UNSIGNED_BYTE, imageBuffer_PosY );
		_gl->TexImage2D( GLTextureImage::CUBE_MAP_NEGATIVE_Y, 0, GLTexInternal::RGBA, width, height, 0, 
						 GLTexFormat::RGBA, GLVarType::UNSIGNED_BYTE, imageBuffer_NegY );
						
		_gl->TexImage2D( GLTextureImage::CUBE_MAP_POSITIVE_Z, 0, GLTexInternal::RGBA, width, height, 0, 
						 GLTexFormat::RGBA, GLVarType::UNSIGNED_BYTE, imageBuffer_PosZ );
		_gl->TexImage2D( GLTextureImage::CUBE_MAP_NEGATIVE_Z, 0, GLTexInternal::RGBA, width, height, 0, 
						 GLTexFormat::RGBA, GLVarType::UNSIGNED_BYTE, imageBuffer_NegZ );
	}

    _gl->TexParameteri( GLTexture::CUBE_MAP, GLTexParamType::WRAP_S, GLTexParamValue::CLAMP_TO_EDGE );
    _gl->TexParameteri( GLTexture::CUBE_MAP, GLTexParamType::WRAP_T, GLTexParamValue::CLAMP_TO_EDGE );
    _gl->TexParameteri( GLTexture::CUBE_MAP, GLTexParamType::WRAP_R, GLTexParamValue::CLAMP_TO_EDGE );

	if ( hasMipmap )
	{
		_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::MAG_FILTER, GLTexParamValue::LINEAR_MIPMAP_LINEAR );
		_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::MIN_FILTER, GLTexParamValue::LINEAR_MIPMAP_LINEAR );

		_gl->GenerateMipmap( GLTexture::CUBE_MAP );
	}
	else
	{
		_gl->TexParameteri( GLTexture::CUBE_MAP, GLTexParamType::MAG_FILTER, GLTexParamValue::LINEAR );
		_gl->TexParameteri( GLTexture::CUBE_MAP, GLTexParamType::MIN_FILTER, GLTexParamValue::LINEAR );
	}
}


void RenderManagerImpl::DeleteTexture( UInt textureID )
{
	_gl->DeleteTextures( 1, &textureID );
}

