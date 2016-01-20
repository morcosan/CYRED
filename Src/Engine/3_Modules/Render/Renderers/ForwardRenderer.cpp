// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "ForwardRenderer.h"

#include "../Techniques/Technique.h"

#include "../OpenGL/GLContext.h"
#include "../OpenGL/GL.h"
#include "../OpenGL/Vertex.h"

#include "../Assets/Shader.h"
#include "../Assets/Texture.h"

#include "../RenderManager.h"


using namespace CYRED;
using namespace NotAPI;



ForwardRenderer::~ForwardRenderer()
{
	_gl->DeleteFramebuffers	( 1, &_mainFramebufferID );
	_gl->DeleteTextures		( 1, &_mainColorbufferID );
	_gl->DeleteTextures		( 1, &_mainDepthbufferID );

	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, EMPTY_BUFFER );
}


void ForwardRenderer::Render( Scene* scene, GameObject* cameraGO )
{
	ASSERT( _technique != NULL );

	UInt buffers[ 1 ] =
	{
		_mainFramebufferID
	};

	_technique->SetViewportSize( _glContext->GetWidth(), _glContext->GetHeight() );
	_technique->Render( buffers, scene, cameraGO );
}


void ForwardRenderer::OnResize()
{
	// resize buffers here
}


void ForwardRenderer::DisplayOnScreen()
{
	return;

	_gl->Enable( GLCapability::BLEND );
	_gl->BlendEquation( GLBlendMode::FUNC_ADD );
	_gl->BlendFunc( GLBlendFactor::SRC_ALPHA, GLBlendFactor::ONE_MINUS_SRC_ALPHA );
	_gl->Disable( GLCapability::DEPTH_TEST );

	_gl->PolygonMode( GLPolygonFace::FRONT_AND_BACK, GLPolygonMode::FILL );
	_gl->Enable( GLCapability::CULL_FACE );
	_gl->CullFace( GLCullFace::BACK );

	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, EMPTY_BUFFER );

	_RenderScreenQuad( NULL, _screenQuadShader );
}


void ForwardRenderer::_CreateMainBuffers( UInt width, UInt height )
{
	_gl->GenFramebuffers( 1, &_mainFramebufferID );
	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, _mainFramebufferID );


	_gl->GenTextures( 1, &_mainColorbufferID );
	_gl->BindTexture( GLTexture::TEXTURE_2D, _mainColorbufferID );
	_gl->TexImage2D( GLTextureImage::TEXTURE_2D, 0, GLTexInternal::RGBA, width, height, 0, 
					 GLTexFormat::RGBA, GLVarType::UNSIGNED_BYTE, NULL );
	_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::WRAP_S,		GLTexParamValue::CLAMP_TO_EDGE );
	_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::WRAP_T,		GLTexParamValue::CLAMP_TO_EDGE );
	_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::MAG_FILTER,	GLTexParamValue::NEAREST );
	_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::MIN_FILTER,	GLTexParamValue::NEAREST );
	_gl->FramebufferTexture2D( GLFrameBuffer::FRAMEBUFFER, GLBufferAttachment::COLOR_ATTACHMENT0, 
							   GLTexture::TEXTURE_2D, _mainColorbufferID, 0 );
	
	_gl->GenTextures( 1, &_mainDepthbufferID );
    _gl->BindTexture( GLTexture::TEXTURE_2D, _mainDepthbufferID );
    _gl->TexImage2D( GLTextureImage::TEXTURE_2D, 0, GLTexInternal::DEPTH_COMPONENT16, width, height, 0, 
					 GLTexFormat::DEPTH_COMPONENT, GLVarType::UNSIGNED_SHORT, NULL );
    _gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::WRAP_S, GLTexParamValue::CLAMP_TO_EDGE );
	_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::WRAP_T, GLTexParamValue::CLAMP_TO_EDGE );
	_gl->FramebufferTexture2D( GLFrameBuffer::FRAMEBUFFER, GLBufferAttachment::DEPTH_ATTACHMENT, 
							   GLTexture::TEXTURE_2D, _mainDepthbufferID, 0 ); 


	GLBufferStatus status = _gl->CheckFramebufferStatus( GLFrameBuffer::FRAMEBUFFER );
	// TODO
	//ASSERT( status == GLFlag::FRAMEBUFFER_COMPLETE );


	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, EMPTY_BUFFER );
	_gl->BindTexture( GLTexture::TEXTURE_2D, EMPTY_BUFFER );
}


void ForwardRenderer::_ResizeMainBuffers( UInt width, UInt height )
{
	_gl->BindTexture( GLTexture::TEXTURE_2D, _mainColorbufferID );
	_gl->TexImage2D( GLTextureImage::TEXTURE_2D, 0, GLTexInternal::RGBA, width, height, 0, 
					 GLTexFormat::RGBA, GLVarType::UNSIGNED_BYTE, NULL );

	_gl->BindTexture( GLTexture::TEXTURE_2D, _mainDepthbufferID );
    _gl->TexImage2D( GLTextureImage::TEXTURE_2D, 0, GLTexInternal::DEPTH_COMPONENT16, width, height, 0, 
					 GLTexFormat::DEPTH_COMPONENT, GLVarType::UNSIGNED_SHORT, NULL );
}


void ForwardRenderer::_RenderScreenQuad( Texture* texture, Shader* shader )
{
	ASSERT( shader != NULL );

	UInt programID = shader->GetProgramID();
	_gl->UseProgram( programID );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, _screenQuadID );

	//! set shader variables
	_gl->EnableVertexAttribArray( 0 );
    _gl->VertexAttribPointer( 0, 3, GLVarType::FLOAT, FALSE, sizeof(Vertex), (const void*) (offsetof(Vertex, position)) );
	_gl->EnableVertexAttribArray( 3 );
    _gl->VertexAttribPointer( 3, 2, GLVarType::FLOAT, FALSE, sizeof(Vertex), (const void*) (offsetof(Vertex, uv)) );

	//int screenSizeLoc = shader->GetUniformLocation( "ME3D_screenSize" );
	//_gl->Uniform2fv( screenSizeLoc, 1, _contextSize.GetValuePtr() );


	_gl->PolygonMode( GLPolygonFace::FRONT_AND_BACK, GLPolygonMode::FILL );
	_gl->Enable( GLCapability::CULL_FACE );
	_gl->CullFace( GLCullFace::BACK );

	// TODO
	_gl->Disable( GLCapability::CULL_FACE );



	_gl->DrawArrays( GLDrawMode::TRIANGLES, 0, 6 );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, EMPTY_BUFFER );
	_gl->UseProgram( EMPTY_SHADER );
}


void ForwardRenderer::_GenerateScreenQuad()
{
	DataArray<Vertex> vertices;

	vertices.Add( Vertex( Vector3(-1, -1, 0), Vector2(0, 0) ) );             
	vertices.Add( Vertex( Vector3( 1, -1, 0), Vector2(1, 0) ) ); 
	vertices.Add( Vertex( Vector3( 1,  1, 0), Vector2(1, 1) ) ); 

	vertices.Add( Vertex( Vector3(-1, -1, 0), Vector2(0, 0) ) );  
	vertices.Add( Vertex( Vector3( 1,  1, 0), Vector2(1, 1) ) ); 
	vertices.Add( Vertex( Vector3(-1,  1, 0), Vector2(0, 1) ) );

	_gl->GenBuffers( 1, &_screenQuadID );
  	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, _screenQuadID );
	_gl->BufferData( GLBuffer::ARRAY_BUFFER, sizeof(Vertex) * vertices.Size(), 
					 vertices.Data(), GLDrawType::STATIC_DRAW );
	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, EMPTY_BUFFER );
}


void ForwardRenderer::_OnInitialize()
{
	_CreateMainBuffers( _glContext->GetWidth(), _glContext->GetHeight() );

	_GenerateScreenQuad();
}


