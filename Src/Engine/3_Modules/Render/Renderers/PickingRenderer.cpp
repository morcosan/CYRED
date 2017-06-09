// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "PickingRenderer.h"

#include "../../../2_BuildingBlocks/Components/Transform.h"
#include "../../../2_BuildingBlocks/GameObject.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"

#include "../Components/Camera.h"
#include "../Components/Light.h"
#include "../Components/MeshRendering.h"
#include "../Components/MorphRendering.h"
#include "../Components/ParticleEmitter.h"
#include "../Components/Text3D.h"

#include "../Assets/Material.h"
#include "../Assets/Mesh.h"
#include "../Assets/Morph.h"
#include "../Assets/Shader.h"
#include "../Assets/Texture.h"
#include "../Assets/Font.h"

#include "../OpenGL/GL.h"
#include "../OpenGL/Uniform.h"
#include "../OpenGL/GLContext.h"
#include "../OpenGL/Vertex.h"

#include "../RenderManager.h"
#include "../../Time/TimeManager.h"


using namespace CYRED;
using namespace NonAPI;



PickingRenderer::~PickingRenderer()
{
	_gl->DeleteFramebuffers	( 1, &_frameBufferID );
	_gl->DeleteTextures		( 1, &_colorBufferID );
	_gl->DeleteTextures		( 1, &_depthBufferID );

	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, EMPTY_BUFFER );
}


/*****
* @desc: clear the previous frame
*/
void PickingRenderer::ClearScreen( float r, float g, float b )
{
	ASSERT( _gl != NULL );

	_gl->Viewport( 0, 0, _glContext->GetWidth(), _glContext->GetHeight() );

	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, _frameBufferID );
	_gl->ClearColor( r, g, b, 1 );
	_gl->DepthMask( TRUE );
	_gl->Clear( GLBufferBit::COLOR_BUFFER_BIT | GLBufferBit::DEPTH_BUFFER_BIT );
}


/*****
* @desc: clear the depth buffer; new rendering goes over anything before
*/
void PickingRenderer::ResetDepth()
{
	ASSERT( _gl != NULL );

	// clear depth buffer
	_gl->DepthMask( TRUE );
	_gl->Clear( GLBufferBit::DEPTH_BUFFER_BIT );
}


/*****
* @desc: render the given component from given gameobject and its children
* @params: 
* 		compType	- the component to render
* 		target		- the target gameobject
* 		cameraGO	- camera
* 		lightsGO	- the list of lights to be used
*/
void PickingRenderer::Render( int layer, ComponentType compType, Node* target, GameObject* cameraGO, 
							  DataArray<GameObject*>& lightsGO )
{
	// sanity check
	if ( target == NULL || cameraGO == NULL ) { 
		return;
	}

	// prepare buffer
	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, _frameBufferID );

	// store data
	_currCameraTran	= cameraGO->GetComponent<Transform>();
	_currCameraCam	= cameraGO->GetComponent<Camera>();
	_currLayer		= layer;

	// prepare for rendering
	switch ( compType ) {
		case ComponentType::MESH_RENDERING:
		case ComponentType::MORPH_RENDERING:
		case ComponentType::TEXT_3D:
			_gl->Disable( GLCapability::BLEND );
			_gl->Enable( GLCapability::DEPTH_TEST );
			_gl->DepthFunc( GLDepthFunc::LEQUAL );
			_gl->DepthMask( TRUE );
			break;

		case ComponentType::PARTICLE_EMITTER:
			_gl->Disable( GLCapability::BLEND );
			_gl->DepthMask( FALSE );
			break;
	}

	// check if target is gameobject
	GameObject* targetGO = CAST_D( GameObject*, target );
	if ( targetGO != NULL ) {
		switch ( compType ) {
			case ComponentType::MESH_RENDERING:		_RecRenderMesh( targetGO, lightsGO );	break;
			case ComponentType::MORPH_RENDERING:	_RecRenderMorph( targetGO, lightsGO );	break;
			case ComponentType::PARTICLE_EMITTER:	_RecRenderParticles( targetGO );		break;
			case ComponentType::TEXT_3D:			_RecRenderText3D( targetGO );			break;
		}
	}
	else {
		// render children
		for ( int i = 0; i < target->GetChildNodeCount(); ++i ) {
			GameObject* gameObject = CAST_S(GameObject*, target->GetChildNodeAt(i));
			switch ( compType ) {
				case ComponentType::MESH_RENDERING:		_RecRenderMesh( gameObject, lightsGO );		break;
				case ComponentType::MORPH_RENDERING:	_RecRenderMorph( gameObject, lightsGO );	break;
				case ComponentType::PARTICLE_EMITTER:	_RecRenderParticles( gameObject );			break;
				case ComponentType::TEXT_3D:			_RecRenderText3D( gameObject );				break;
			}
		}
	}
}


/*****
* @desc: read the pixel from renderer at given location
* @params: 
* 		x - location on x axis
* 		y - location on y axis
* @assert: canvas and renderer are set
*/
Vector4 PickingRenderer::ReadPixel( int x, int y )
{
	ASSERT( _gl != NULL );

	// prepare buffer
	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, _frameBufferID );
	// swap x
	y = _glContext->GetHeight() - 1 - y;
	// read pixel
	Vector4 pixel;
	_gl->ReadPixels( x, y, 1, 1, GLPixelFormat::RGBA, GLVarType::FLOAT, &pixel );

	return pixel;
}


void PickingRenderer::OnResize()
{
	// resize buffers
	_ResizeBuffers( _glContext->GetWidth(), _glContext->GetHeight() );
}


void PickingRenderer::DisplayOnScreen()
{
	//_gl->Enable( GLCapability::BLEND );
	//_gl->BlendEquation( GLBlendMode::FUNC_ADD );
	//_gl->BlendFunc( GLBlendFactor::SRC_ALPHA, GLBlendFactor::ONE_MINUS_SRC_ALPHA );
	//_gl->Disable( GLCapability::DEPTH_TEST );

	//_gl->PolygonMode( GLPolygonFace::FRONT_AND_BACK, GLPolygonMode::FILL );
	//_gl->Enable( GLCapability::CULL_FACE );
	//_gl->CullFace( GLCullFace::BACK );

	//_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, EMPTY_BUFFER );

	//_RenderScreenQuad( NULL, _screenQuadShader );
}


void PickingRenderer::_OnInitialize()
{
	_CreateBuffers( _glContext->GetWidth(), _glContext->GetHeight() );
	//_GenerateScreenQuad();
}


void PickingRenderer::_CreateBuffers( int width, int height )
{
	_gl->GenFramebuffers( 1, &_frameBufferID );
	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, _frameBufferID );

	_gl->GenTextures( 1, &_colorBufferID );
	_gl->BindTexture( GLTexture::TEXTURE_2D, _colorBufferID );
	_gl->TexImage2D( GLTextureImage::TEXTURE_2D, 0, GLTexInternal::RGBA32F, width, height, 0, 
					 GLTexFormat::RGBA, GLVarType::FLOAT, NULL );
	_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::WRAP_S,		GLTexParamValue::CLAMP_TO_EDGE );
	_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::WRAP_T,		GLTexParamValue::CLAMP_TO_EDGE );
	_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::MAG_FILTER,	GLTexParamValue::NEAREST );
	_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::MIN_FILTER,	GLTexParamValue::NEAREST );
	_gl->FramebufferTexture2D( GLFrameBuffer::FRAMEBUFFER, GLBufferAttachment::COLOR_ATTACHMENT0, 
							   GLTexture::TEXTURE_2D, _colorBufferID, 0 );

	_gl->GenTextures( 1, &_depthBufferID );
	_gl->BindTexture( GLTexture::TEXTURE_2D, _depthBufferID );
	_gl->TexImage2D( GLTextureImage::TEXTURE_2D, 0, GLTexInternal::DEPTH_COMPONENT16, width, height, 0, 
					 GLTexFormat::DEPTH_COMPONENT, GLVarType::UNSIGNED_SHORT, NULL );
	_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::WRAP_S, GLTexParamValue::CLAMP_TO_EDGE );
	_gl->TexParameteri( GLTexture::TEXTURE_2D, GLTexParamType::WRAP_T, GLTexParamValue::CLAMP_TO_EDGE );
	_gl->FramebufferTexture2D( GLFrameBuffer::FRAMEBUFFER, GLBufferAttachment::DEPTH_ATTACHMENT, 
							   GLTexture::TEXTURE_2D, _depthBufferID, 0 ); 


	GLBufferStatus status = _gl->CheckFramebufferStatus( GLFrameBuffer::FRAMEBUFFER );

	// unbind
	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, EMPTY_BUFFER );
	_gl->BindTexture( GLTexture::TEXTURE_2D, EMPTY_BUFFER );
}


void PickingRenderer::_ResizeBuffers( int width, int height )
{
	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, _frameBufferID );

	_gl->BindTexture( GLTexture::TEXTURE_2D, _colorBufferID );
	_gl->TexImage2D( GLTextureImage::TEXTURE_2D, 0, GLTexInternal::RGBA32F, width, height, 0, 
					 GLTexFormat::RGBA, GLVarType::FLOAT, NULL );

	_gl->BindTexture( GLTexture::TEXTURE_2D, _depthBufferID );
	_gl->TexImage2D( GLTextureImage::TEXTURE_2D, 0, GLTexInternal::DEPTH_COMPONENT16, width, height, 0, 
					 GLTexFormat::DEPTH_COMPONENT, GLVarType::UNSIGNED_SHORT, NULL );

	// unbind
	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, EMPTY_BUFFER );
	_gl->BindTexture( GLTexture::TEXTURE_2D, EMPTY_BUFFER );
}


//void PickingRenderer::_RenderScreenQuad( Texture* texture, Shader* shader )
//{
//	ASSERT( shader != NULL );
//
//	int programID = shader->GetProgramID();
//	_gl->UseProgram( programID );
//
//	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, _screenQuadID );
//
//	//! set shader variables
//	_gl->EnableVertexAttribArray( 0 );
//	_gl->VertexAttribPointer( 0, 3, GLVarType::FLOAT, FALSE, sizeof(Vertex), (const void*) (offsetof(Vertex, position)) );
//	_gl->EnableVertexAttribArray( 3 );
//	_gl->VertexAttribPointer( 3, 2, GLVarType::FLOAT, FALSE, sizeof(Vertex), (const void*) (offsetof(Vertex, uv)) );
//
//	//int screenSizeLoc = shader->GetUniformLocation( "ME3D_screenSize" );
//	//_gl->Uniform2fv( screenSizeLoc, 1, _contextSize.Ptr() );
//
//
//	_gl->PolygonMode( GLPolygonFace::FRONT_AND_BACK, GLPolygonMode::FILL );
//	_gl->Enable( GLCapability::CULL_FACE );
//	_gl->CullFace( GLCullFace::BACK );
//
//	// TODO
//	_gl->Disable( GLCapability::CULL_FACE );
//
//
//
//	_gl->DrawArrays( GLDrawMode::TRIANGLES, 0, 6 );
//
//	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, EMPTY_BUFFER );
//	_gl->UseProgram( EMPTY_SHADER );
//}


//void PickingRenderer::_GenerateScreenQuad()
//{
//	DataArray<Vertex> vertices;
//
//	vertices.Add( Vertex( Vector3(-1, -1, 0), Vector2(0, 0) ) );             
//	vertices.Add( Vertex( Vector3( 1, -1, 0), Vector2(1, 0) ) ); 
//	vertices.Add( Vertex( Vector3( 1,  1, 0), Vector2(1, 1) ) ); 
//
//	vertices.Add( Vertex( Vector3(-1, -1, 0), Vector2(0, 0) ) );  
//	vertices.Add( Vertex( Vector3( 1,  1, 0), Vector2(1, 1) ) ); 
//	vertices.Add( Vertex( Vector3(-1,  1, 0), Vector2(0, 1) ) );
//
//	_gl->GenBuffers( 1, &_screenQuadID );
//	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, _screenQuadID );
//	_gl->BufferData( GLBuffer::ARRAY_BUFFER, sizeof(Vertex) * vertices.Size(), 
//					 vertices.Data(), GLDrawType::STATIC_DRAW );
//	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, EMPTY_BUFFER );
//}


void PickingRenderer::_RecRenderMesh( GameObject* gameObject, DataArray<GameObject*>& lightsGO )
{
	if ( !gameObject->IsEnabled() ) {
		return;
	}

	// render children
	for ( int i = 0; i < gameObject->GetChildNodeCount(); ++i ) {
		_RecRenderMesh( CAST_S(GameObject*, gameObject->GetChildNodeAt(i)), lightsGO );
	}

	if ( gameObject->GetLayer() != _currLayer ) {
		return;
	}


	MeshRendering*	meshRender	= gameObject->GetComponent<MeshRendering>();
	Transform*		objTran		= gameObject->GetComponent<Transform>();
	if ( meshRender == NULL || objTran == NULL || !meshRender->IsEnabled() || !objTran->IsEnabled() ) {
		return;
	}

	Material*	material	= meshRender->GetMaterial();
	Mesh*		mesh		= meshRender->GetMesh();
	if ( material == NULL || mesh == NULL ) {
		return;
	}

	Material* pickingProxy = material->GetPickingProxy();
	if ( pickingProxy == NULL ) {
		return;
	}

	Shader* shader = pickingProxy->GetShader();
	if ( shader == NULL ) {
		return;
	}

	int shaderProgram = shader->GetProgramID();
	if ( shaderProgram == INVALID_SHADER ) {
		return;
	}

	// add shader
	_gl->UseProgram( shaderProgram );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			mesh->GetVBO() );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	mesh->GetIBO() );

	// bind vertex data
	_gl->EnableVertexAttribArray( 0 );
	_gl->VertexAttribPointer( 0, 3, GLVarType::FLOAT, FALSE, sizeof(Vertex), 
									(const void*) (offsetof(Vertex, position)) );


	// bind main 3 matrix
	int worldUniform			= shader->GetUniformLocation( Uniform::WORLD		);
	int viewUniform				= shader->GetUniformLocation( Uniform::VIEW			);
	int projectionUniform		= shader->GetUniformLocation( Uniform::PROJECTION	);

	Matrix4 worldMatrix			= objTran->GetWorldMatrix();
	Matrix4 viewMatrix			= _currCameraTran->GetViewMatrix();
	Matrix4 projectionMatrix	= _currCameraCam->GetProjectionMatrix();

	_gl->UniformMatrix4fv( worldUniform,		1, FALSE, worldMatrix.Ptr()		 );
	_gl->UniformMatrix4fv( viewUniform,			1, FALSE, viewMatrix.Ptr()		 );
	_gl->UniformMatrix4fv( projectionUniform,	1, FALSE, projectionMatrix.Ptr() );


	// bind unique id uniform
	int uidUniform = shader->GetUniformLocation( Uniform::UID );
	_gl->Uniform1i( uidUniform, gameObject->GetUniqueID() );


	// bind material
	_BindMaterial( pickingProxy );


	// draw
	switch ( mesh->GetMeshType() ) {
		case MeshType::POLYGON:
			_gl->DrawElements( GLDrawMode::TRIANGLES, mesh->GetNumIndices(), GLVarType::UNSIGNED_INT, 0 );
			break;

		case MeshType::LINE:
			_gl->DrawElements( GLDrawMode::LINES, mesh->GetNumIndices(), GLVarType::UNSIGNED_INT, 0 );
			break;
	}

	// unbind all
	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			EMPTY_BUFFER );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	EMPTY_BUFFER );
	_gl->UseProgram( EMPTY_SHADER );
}


void PickingRenderer::_RecRenderMorph( GameObject* gameObject, DataArray<GameObject*>& lightsGO )
{
	if ( !gameObject->IsEnabled() ) {
		return;
	}

	// render child nodes
	for ( int i = 0; i < gameObject->GetChildNodeCount(); ++i ) {
		_RecRenderMorph( CAST_S(GameObject*, gameObject->GetChildNodeAt(i)), lightsGO );
	}

	if ( gameObject->GetLayer() != _currLayer ) {
		return;
	}


	MorphRendering*	morphRender	= gameObject->GetComponent<MorphRendering>();
	Transform*		objTran		= gameObject->GetComponent<Transform>();
	if ( morphRender == NULL || objTran == NULL || !morphRender->IsEnabled() || !objTran->IsEnabled() ) {
		return;
	}

	Material*	material	= morphRender->GetMaterial();
	Morph*		morph		= morphRender->GetMorph();
	if ( material == NULL || morph == NULL || morph->GetActiveStates() == 0 ) {
		return;
	}

	Shader* shader = material->GetShader();
	if ( shader == NULL ) {
		return;
	}

	int shaderProgram = shader->GetProgramID();
	if ( shaderProgram == INVALID_SHADER ) {
		return;
	}

	_gl->UseProgram( shaderProgram );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			morph->GetVBO() );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	morph->GetIBO() );


	int firstState = morphRender->GetFirstState();
	int secondState = morphRender->GetSecondState();

	_gl->EnableVertexAttribArray( 0 );
	_gl->VertexAttribPointer( 0, 3, GLVarType::FLOAT, FALSE, sizeof(MorphVertex), 
		(const void*) (offsetof(MorphVertex, position[firstState])) );
	_gl->EnableVertexAttribArray( 1 );
	_gl->VertexAttribPointer( 1, 3, GLVarType::FLOAT, FALSE, sizeof(MorphVertex), 
		(const void*) (offsetof(MorphVertex, position[secondState])) );
	_gl->EnableVertexAttribArray( 2 );
	_gl->VertexAttribPointer( 2, 4, GLVarType::FLOAT, FALSE, sizeof(MorphVertex), 
		(const void*) (offsetof(MorphVertex, color)) );
	_gl->EnableVertexAttribArray( 3 );
	_gl->VertexAttribPointer( 3, 3, GLVarType::FLOAT, FALSE, sizeof(MorphVertex), 
		(const void*) (offsetof(MorphVertex, normal[firstState])) );
	_gl->EnableVertexAttribArray( 4 );
	_gl->VertexAttribPointer( 4, 3, GLVarType::FLOAT, FALSE, sizeof(MorphVertex), 
		(const void*) (offsetof(MorphVertex, normal[secondState])) );
	_gl->EnableVertexAttribArray( 5 );
	_gl->VertexAttribPointer( 5, 2, GLVarType::FLOAT, FALSE, sizeof(MorphVertex), 
		(const void*) (offsetof(MorphVertex, uv)) );
	_gl->EnableVertexAttribArray( 6 );
	_gl->VertexAttribPointer( 6, 3, GLVarType::FLOAT, FALSE, sizeof(MorphVertex), 
		(const void*) (offsetof(MorphVertex, tangent)) );
	_gl->EnableVertexAttribArray( 7 );
	_gl->VertexAttribPointer( 7, 3, GLVarType::FLOAT, FALSE, sizeof(MorphVertex), 
		(const void*) (offsetof(MorphVertex, bitangent)) );

	int worldUniform			= shader->GetUniformLocation( Uniform::WORLD		);
	int viewUniform				= shader->GetUniformLocation( Uniform::VIEW			);
	int projectionUniform		= shader->GetUniformLocation( Uniform::PROJECTION	);

	Matrix4 worldMatrix			= objTran->GetWorldMatrix();
	Matrix4 viewMatrix			= _currCameraTran->GetViewMatrix();
	Matrix4 projectionMatrix	= _currCameraCam->GetProjectionMatrix();

	_gl->UniformMatrix4fv( worldUniform,		1, FALSE, worldMatrix.Ptr()			);
	_gl->UniformMatrix4fv( viewUniform,			1, FALSE, viewMatrix.Ptr()			);
	_gl->UniformMatrix4fv( projectionUniform,	1, FALSE, projectionMatrix.Ptr()	);

	// morph rendering curstom uniforms
	int stateRatioUniform = shader->GetUniformLocation( UNIFORM_STATE_RATIO );
	_gl->Uniform1f( stateRatioUniform, morphRender->GetStateRatio() );

	// add material
	_BindMaterial( material );


	// add lights if needed
	if ( lightsGO.Size() > 0 )
	{
		// check if shader contains light uniforms
		int lightsUniform		= shader->GetUniformLocation( Uniform::LIGHTS );
		int lightsCountUniform	= shader->GetUniformLocation( Uniform::LIGHTS_COUNT );
		int ambientColorUniform	= shader->GetUniformLocation( Uniform::AMBIENT_COLOR );

		// check if lights needed
		if ( lightsUniform != -1 || lightsCountUniform != -1 ) {
			// add lights count
			_gl->Uniform1i( lightsCountUniform, lightsGO.Size() );

			// bind each light
			for ( int i = 0; i < lightsGO.Size() && i < Uniform::MAX_LIGHTS; i++ ) {
				Light* light = lightsGO[i]->GetComponent<Light>();
				Transform* transform = lightsGO[i]->GetComponent<Transform>();
				FiniteString uniformName;
				int uniform;

				uniformName.Set( "DEFAULT_lights[%d].type", i );
				uniform = shader->GetUniformLocation( uniformName.GetChar() );
				_gl->Uniform1i( uniform, light->GetLightType() );

				uniformName.Set( "DEFAULT_lights[%d].color", i );
				uniform = shader->GetUniformLocation( uniformName.GetChar() );
				_gl->Uniform3fv( uniform, 1, light->GetColor().Ptr() );

				uniformName.Set( "DEFAULT_lights[%d].intensity", i );
				uniform = shader->GetUniformLocation( uniformName.GetChar() );
				_gl->Uniform1f( uniform, light->GetIntensity() );

				uniformName.Set( "DEFAULT_lights[%d].spotAngle", i );
				uniform = shader->GetUniformLocation( uniformName.GetChar() );
				_gl->Uniform1f( uniform, Math::ToRadians( light->GetSpotAngle() ) );

				uniformName.Set( "DEFAULT_lights[%d].range", i );
				uniform = shader->GetUniformLocation( uniformName.GetChar() );
				_gl->Uniform1f( uniform, light->GetRange() );

				uniformName.Set( "DEFAULT_lights[%d].positionWorld", i );
				uniform = shader->GetUniformLocation( uniformName.GetChar() );
				_gl->Uniform3fv( uniform, 1, transform->GetPositionWorld().Ptr() );

				uniformName.Set( "DEFAULT_lights[%d].directionWorld", i );
				uniform = shader->GetUniformLocation( uniformName.GetChar() );
				Vector3 direction = transform->GetRotationWorld().ApplyRotation( Vector3(0, 0, -1) );
				_gl->Uniform3fv( uniform, 1, direction.Ptr() );
			}
		}
	}


	// draw
	_gl->DrawElements( GLDrawMode::TRIANGLES, morph->GetNumIndices(), GLVarType::UNSIGNED_INT, 0 );

	// unbind all
	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			EMPTY_BUFFER );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	EMPTY_BUFFER );
	_gl->UseProgram( EMPTY_SHADER );
}


void PickingRenderer::_RecRenderParticles( GameObject* gameObject )
{
	if ( !gameObject->IsEnabled() ) {
		return;
	}

	// render child nodes
	for ( int i = 0; i < gameObject->GetChildNodeCount(); ++i ) {
		_RecRenderParticles( CAST_S(GameObject*, gameObject->GetChildNodeAt(i)) );
	}

	if ( gameObject->GetLayer() != _currLayer ) {
		return;
	}


	ParticleEmitter*	emitter	= gameObject->GetComponent<ParticleEmitter>();
	Transform*			objTran	= gameObject->GetComponent<Transform>();
	if ( emitter == NULL || objTran == NULL || !emitter->IsEnabled() || !objTran->IsEnabled() ) {
		return;
	}

	Material* material = emitter->GetMaterial();
	if ( material == NULL ) {
		return;
	}

	Shader* shader = material->GetShader();
	if ( shader == NULL ) {
		return;
	}

	int shaderProgram = shader->GetProgramID();
	if ( shaderProgram == INVALID_SHADER ) {
		return;
	}

	// add shader
	_gl->UseProgram( shaderProgram );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			emitter->GetVBO() );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	emitter->GetIBO() );

	_gl->EnableVertexAttribArray( 0 );
	_gl->VertexAttribPointer( 0, 3, GLVarType::FLOAT, FALSE, sizeof(ParticleVertex), 
		(const void*) (offsetof(ParticleVertex, position)) );
	_gl->EnableVertexAttribArray( 1 );
	_gl->VertexAttribPointer( 1, 3, GLVarType::FLOAT, FALSE, sizeof(ParticleVertex), 
		(const void*) (offsetof(ParticleVertex, velocity)) );
	_gl->EnableVertexAttribArray( 2 );
	_gl->VertexAttribPointer( 2, 3, GLVarType::FLOAT, FALSE, sizeof(ParticleVertex), 
		(const void*) (offsetof(ParticleVertex, acceleration)) );
	_gl->EnableVertexAttribArray( 3 );
	_gl->VertexAttribPointer( 3, 2, GLVarType::FLOAT, FALSE, sizeof(ParticleVertex), 
		(const void*) (offsetof(ParticleVertex, age_sizeS_sizeE)) );

	int worldUniform			= shader->GetUniformLocation( Uniform::WORLD		);
	int viewUniform				= shader->GetUniformLocation( Uniform::VIEW			);
	int projectionUniform		= shader->GetUniformLocation( Uniform::PROJECTION	);

	Matrix4 worldMatrix			= objTran->GetWorldMatrix();
	Matrix4 viewMatrix			= _currCameraTran->GetViewMatrix();
	Matrix4 projectionMatrix	= _currCameraCam->GetProjectionMatrix();

	_gl->UniformMatrix4fv( worldUniform,		1, FALSE, worldMatrix.Ptr()			);
	_gl->UniformMatrix4fv( viewUniform,			1, FALSE, viewMatrix.Ptr()			);
	_gl->UniformMatrix4fv( projectionUniform,	1, FALSE, projectionMatrix.Ptr()	);

	// particle emitter curstom uniforms
	{
		int uniform = shader->GetUniformLocation( UNIFORM_IS_LOOPING );
		_gl->Uniform1i( uniform, (emitter->IsLooping() ? 1 : 0) );
	}
	{
		int uniform = shader->GetUniformLocation( UNIFORM_LIFETIME );
		_gl->Uniform1f( uniform, emitter->GetParticleLifetime() );
	}
	{
		int uniform = shader->GetUniformLocation( UNIFORM_DELTA_TIME );
		_gl->Uniform1f( uniform, TimeManager::Singleton()->GetRenderDeltaTime() );
	}

	// add material
	_BindMaterial( material );

	_gl->BindBufferBase( GLBaseBuffer::SHADER_STORAGE_BUFFER, 0, emitter->GetVBO() );

	// draw
	_gl->DrawElements( GLDrawMode::POINTS, emitter->GetNumIndices(), GLVarType::UNSIGNED_INT, 0 );

	// unbind all
	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			EMPTY_BUFFER );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	EMPTY_BUFFER );
	_gl->BindBufferBase( GLBaseBuffer::SHADER_STORAGE_BUFFER, 0, EMPTY_BUFFER );
	_gl->UseProgram( EMPTY_SHADER );
}


void PickingRenderer::_RecRenderText3D( GameObject* gameObject )
{
	if ( !gameObject->IsEnabled() ) {
		return;
	}

	// render children
	for ( int i = 0; i < gameObject->GetChildNodeCount(); ++i ) {
		_RecRenderText3D( CAST_S(GameObject*, gameObject->GetChildNodeAt(i)) );
	}

	if ( gameObject->GetLayer() != _currLayer ) {
		return;
	}


	Text3D*		text3D	= gameObject->GetComponent<Text3D>();
	Transform*	objTran	= gameObject->GetComponent<Transform>();
	if ( text3D == NULL || objTran == NULL || !text3D->IsEnabled() || !objTran->IsEnabled() ) {
		return;
	}

	Font*		font		= text3D->GetFont();
	Material*	material	= text3D->GetMaterial();
	if ( font == NULL || material == NULL ) {
		return;
	}

	Material* pickingProxy = material->GetPickingProxy();
	if ( pickingProxy == NULL ) {
		return;
	}

	Shader* shader = pickingProxy->GetShader();
	if ( shader == NULL ) {
		return;
	}

	int shaderProgram = shader->GetProgramID();
	if ( shaderProgram == INVALID_SHADER ) {
		return;
	}

	// add shader
	_gl->UseProgram( shaderProgram );

	// bind buffer
	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, _textVBO );

	// bind vertex data
	_gl->EnableVertexAttribArray( 0 );
	_gl->VertexAttribPointer( 0, 3, GLVarType::FLOAT, FALSE, sizeof(Vertex), 
		(const void*) (offsetof(Vertex, position)) );
	_gl->EnableVertexAttribArray( 3 );
	_gl->VertexAttribPointer( 3, 2, GLVarType::FLOAT, FALSE, sizeof(Vertex), 
		(const void*) (offsetof(Vertex, uv)) );


	// bind main 3 matrix
	int worldUniform			= shader->GetUniformLocation( Uniform::WORLD		);
	int viewUniform				= shader->GetUniformLocation( Uniform::VIEW			);
	int projectionUniform		= shader->GetUniformLocation( Uniform::PROJECTION	);

	Matrix4 worldMatrix			= objTran->GetWorldMatrix();
	Matrix4 viewMatrix			= _currCameraTran->GetViewMatrix();
	Matrix4 projectionMatrix	= _currCameraCam->GetProjectionMatrix();

	_gl->UniformMatrix4fv( worldUniform,		1, FALSE, worldMatrix.Ptr()		 );
	_gl->UniformMatrix4fv( viewUniform,			1, FALSE, viewMatrix.Ptr()		 );
	_gl->UniformMatrix4fv( projectionUniform,	1, FALSE, projectionMatrix.Ptr() );


	// bind unique id uniform
	int uidUniform = shader->GetUniformLocation( Uniform::UID );
	_gl->Uniform1i( uidUniform, gameObject->GetUniqueID() );


	// bind material
	_BindMaterial( pickingProxy );


	// prepare for drawing
	int offsetX = 0;
	cchar* text = text3D->GetText();
	int textLength = String::Length( text );
	float unitRatio = (1.0f / font->GetMaxSize()) * (text3D->GetTextSize() / Text3D::RENDER_UNIT_SIZE);

	// calculate text total with and height
	float totalWidth = 0;
	float totalHeight = 0;
	for ( int i = 0; i < textLength; i++ ) {
		// get font char
		FontChar* fontChar = font->GetFontChar( text[i] );

		// advance cursors for next glyph (note that advance is number of 1/64 pixels)
		// bitshift by 6 to get value in pixels (2^6 = 64)
		offsetX += (fontChar->advance >> 6);

		// update width
		if ( i == textLength - 1 ) {
			totalWidth = CAST_S( float, offsetX );
		}

		// update height
		if ( totalHeight < fontChar->height ) {
			totalHeight = CAST_S( float, fontChar->height );
		}
	}

	// draw font chars
	offsetX = 0;
	for ( int i = 0; i < textLength; i++ ) {
		// get font char
		FontChar* fontChar = font->GetFontChar( text[i] );

		// calculate texture size and posistion
		float w = unitRatio * CAST_S( float, fontChar->width );
		float h = unitRatio * CAST_S( float, fontChar->height );
		float x = unitRatio * CAST_S( float, offsetX + fontChar->bearingX );
		float y = unitRatio * CAST_S( float, fontChar->bearingY - fontChar->height );

		// apply horizontal alignment
		switch ( text3D->GetHorizontalAlign() ) {
			case HorizontalAlign::LEFT:											break;
			case HorizontalAlign::MIDDLE:	x -= unitRatio * totalWidth / 2;	break;
			case HorizontalAlign::RIGHT:	x -= unitRatio * totalWidth;		break;
		}

		// apply vertical alignment
		switch ( text3D->GetVerticalAlign() ) {
			case VerticalAlign::TOP:		y -= unitRatio * totalHeight;		break;
			case VerticalAlign::MIDDLE:		y -= unitRatio * totalHeight / 2;	break;
			case VerticalAlign::BOTTOM:											break;
		}

		// create vertices
		Vertex vertices[6] = {
			Vertex { Vector3( x,	 y + h, 0 ), Vector2( 0, 0 ) },
			Vertex { Vector3( x,	 y,		0 ), Vector2( 0, 1 ) },
			Vertex { Vector3( x + w, y,		0 ), Vector2( 1, 1 ) },

			Vertex { Vector3( x,	 y + h, 0 ), Vector2( 0, 0 ) },
			Vertex { Vector3( x + w, y,		0 ), Vector2( 1, 1 ) },
			Vertex { Vector3( x + w, y + h, 0 ), Vector2( 1, 0 ) }
		};

		// bind buffer data
		_gl->BufferSubData( GLBuffer::ARRAY_BUFFER, 0, sizeof(vertices), vertices ); 

		// render quad
		_gl->DrawArrays( GLDrawMode::TRIANGLES, 0, 6 );

		// advance cursors for next glyph (note that advance is number of 1/64 pixels)
		// bitshift by 6 to get value in pixels (2^6 = 64)
		offsetX += (fontChar->advance >> 6); 
	}

	// unbind all
	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, EMPTY_BUFFER );
	_gl->UseProgram( EMPTY_SHADER );
	_gl->BindTexture( GLTexture::TEXTURE_2D, INVALID_TEXTURE );
}


void PickingRenderer::_BindMaterial( Material* material )
{
	Shader* shader = material->GetShader();

	ASSERT( shader != NULL );

	//! set uniforms from material
	int nextActiveTexture = 0;
	int totalProperties = material->GetPropertiesCount();

	for ( int i = 0; i < totalProperties; ++i ) {
		cchar* uniformName = material->GetPropertyNameAt( i );
		int location = shader->GetUniformLocation( uniformName );
		DataUnion& data = material->GetPropertyDataAt( i );

		switch ( data.GetValueType() ) {
			case DataUnion::INT:
				_gl->Uniform1i( location, data.GetInt() );
				break;

			case DataUnion::FLOAT:
				_gl->Uniform1f( location, data.GetFloat() );
				break;

			case DataUnion::VECTOR2:
				_gl->Uniform2fv( location, 1, data.GetVector2().Ptr() );
				break;

			case DataUnion::VECTOR3:
				_gl->Uniform3fv( location, 1, data.GetVector3().Ptr() );
				break;

			case DataUnion::VECTOR4:
				_gl->Uniform4fv( location, 1, data.GetVector4().Ptr() );
				break;

			case DataUnion::REFERENCE:
				_gl->ActiveTexture( nextActiveTexture );
				Texture* texture = CAST_S( Texture*, data.GetReference() );

				if ( texture != NULL ) {
					switch ( texture->GetTextureType() ) {
						case TextureType::TEXTURE_2D:
							_gl->BindTexture( GLTexture::TEXTURE_2D, texture->GetTextureID() );
							break;

						case TextureType::CUBE_MAP:
							_gl->BindTexture( GLTexture::CUBE_MAP, texture->GetTextureID() );
							break;
					}
				}
				else {
					// TODO
					// bind a pure white texture
				}

				_gl->Uniform1i( location, nextActiveTexture );

				++ nextActiveTexture;
				break;
		}
	}


	if ( material->IsWireframe() ) {
		_gl->PolygonMode( GLPolygonFace::FRONT_AND_BACK, GLPolygonMode::LINE );
		_gl->LineWidth( material->GetLineWidth() );
		_gl->Disable( GLCapability::CULL_FACE );
	}
	else {
		_gl->PolygonMode( GLPolygonFace::FRONT_AND_BACK, GLPolygonMode::FILL );

		switch ( material->GetFaceCulling() ) {
			case FaceCulling::CULL_NONE:
				_gl->Disable( GLCapability::CULL_FACE );
				break;

			case FaceCulling::CULL_BACK:
				_gl->Enable( GLCapability::CULL_FACE );
				_gl->CullFace( GLCullFace::BACK );
				break;

			case FaceCulling::CULL_FRONT:
				_gl->Enable( GLCapability::CULL_FACE );
				_gl->CullFace( GLCullFace::FRONT );
				break;
		}
	}
}
