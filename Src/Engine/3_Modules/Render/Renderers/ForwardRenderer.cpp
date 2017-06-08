// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "ForwardRenderer.h"

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


/*****
* @desc: clear the previous frame
*/
void ForwardRenderer::ClearScreen( float r, float g, float b )
{
	ASSERT( _gl != NULL );

	_gl->Viewport( 0, 0, _glContext->GetWidth(), _glContext->GetHeight() );

	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, EMPTY_BUFFER );
	_gl->ClearColor( r, g, b, 1 );
	_gl->DepthMask( TRUE );
	_gl->Clear( GLBufferBit::COLOR_BUFFER_BIT | GLBufferBit::DEPTH_BUFFER_BIT );
}


/*****
* @desc: clear the depth buffer; new rendering goes over anything before
*/
void ForwardRenderer::ResetDepth()
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
void ForwardRenderer::Render( ComponentType compType, Node* target, GameObject* cameraGO, 
							  DataArray<GameObject*>& lightsGO )
{
	// sanity check
	if ( target == NULL || cameraGO == NULL ) { 
		return;
	}

	// store data
	_currCameraTran		= cameraGO->GetComponent<Transform>();
	_currCameraCam		= cameraGO->GetComponent<Camera>();

	// prepare for rendering
	switch ( compType ) {
		case ComponentType::MESH_RENDERING:
		case ComponentType::MORPH_RENDERING:
		{
			_gl->Enable( GLCapability::BLEND );
			_gl->BlendEquation( GLBlendMode::FUNC_ADD );
			_gl->BlendFunc( GLBlendFactor::SRC_ALPHA, GLBlendFactor::ONE_MINUS_SRC_ALPHA );
			_gl->Enable( GLCapability::DEPTH_TEST );
			_gl->DepthFunc( GLDepthFunc::LEQUAL );
			_gl->DepthMask( TRUE );
			break;
		}

		case ComponentType::PARTICLE_EMITTER:
		{
			_gl->Enable( GLCapability::BLEND );
			_gl->BlendFunc( GLBlendFactor::SRC_ALPHA, GLBlendFactor::ONE );
			_gl->DepthMask( FALSE );
			break;
		}
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
Vector4 ForwardRenderer::ReadPixel( int x, int y )
{
	return Vector4();
}


void ForwardRenderer::OnResize()
{
	// resize buffers here
}


void ForwardRenderer::DisplayOnScreen()
{
}


void ForwardRenderer::_OnInitialize()
{
}


void ForwardRenderer::_RecRenderMesh( GameObject* gameObject, DataArray<GameObject*>& lightsGO )
{
	if ( !gameObject->IsEnabled() ) {
		return;
	}

	// render children
	for ( int i = 0; i < gameObject->GetChildNodeCount(); ++i ) {
		_RecRenderMesh( CAST_S(GameObject*, gameObject->GetChildNodeAt(i)), lightsGO );
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

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			mesh->GetVBO() );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	mesh->GetIBO() );


	// bind vertex data
	_gl->EnableVertexAttribArray( 0 );
	_gl->VertexAttribPointer( 0, 3, GLVarType::FLOAT, FALSE, sizeof(Vertex), 
									(const void*) (offsetof(Vertex, position)) );
	_gl->EnableVertexAttribArray( 1 );
	_gl->VertexAttribPointer( 1, 4, GLVarType::FLOAT, FALSE, sizeof(Vertex), 
									(const void*) (offsetof(Vertex, color)) );
	_gl->EnableVertexAttribArray( 2 );
	_gl->VertexAttribPointer( 2, 3, GLVarType::FLOAT, FALSE, sizeof(Vertex), 
									(const void*) (offsetof(Vertex, normal)) );
	_gl->EnableVertexAttribArray( 3 );
	_gl->VertexAttribPointer( 3, 2, GLVarType::FLOAT, FALSE, sizeof(Vertex), 
									(const void*) (offsetof(Vertex, uv)) );
	_gl->EnableVertexAttribArray( 4 );
	_gl->VertexAttribPointer( 4, 3, GLVarType::FLOAT, FALSE, sizeof(Vertex), 
									(const void*) (offsetof(Vertex, tangent)) );
	_gl->EnableVertexAttribArray( 5 );
	_gl->VertexAttribPointer( 5, 3, GLVarType::FLOAT, FALSE, sizeof(Vertex), 
									(const void*) (offsetof(Vertex, bitangent)) );


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


	// bind camera
	int cameraPosWorldUniform = shader->GetUniformLocation( Uniform::CAMERA_POS_WORLD );
	_gl->Uniform3fv( cameraPosWorldUniform,	1, _currCameraTran->GetPositionWorld().Ptr() );


	// bind material
	_BindMaterial( material );


	// bind lights if needed
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

				if ( light != NULL && transform != NULL ) {
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
	}


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


void ForwardRenderer::_RecRenderMorph( GameObject* gameObject, DataArray<GameObject*>& lightsGO )
{
	if ( !gameObject->IsEnabled() ) {
		return;
	}

	// render child nodes
	for ( int i = 0; i < gameObject->GetChildNodeCount(); ++i ) {
		_RecRenderMorph( CAST_S(GameObject*, gameObject->GetChildNodeAt(i)), lightsGO );
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

	// bind morph rendering curstom uniforms
	int stateRatioUniform = shader->GetUniformLocation( UNIFORM_STATE_RATIO );
	_gl->Uniform1f( stateRatioUniform, morphRender->GetStateRatio() );

	// bind material
	_BindMaterial( material );


	// bind lights if needed
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

				if ( light != NULL && transform != NULL ) {
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
					Vector3 direction = transform->GetRotationWorld().ApplyRotation( Vector3( 0, 0, -1 ) );
					_gl->Uniform3fv( uniform, 1, direction.Ptr() );
				}
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


void ForwardRenderer::_RecRenderParticles( GameObject* gameObject )
{
	if ( !gameObject->IsEnabled() ) {
		return;
	}

	// render child nodes
	for ( int i = 0; i < gameObject->GetChildNodeCount(); ++i ) {
		_RecRenderParticles( CAST_S(GameObject*, gameObject->GetChildNodeAt(i)) );
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

	// bind material
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


void ForwardRenderer::_RecRenderText3D( GameObject* gameObject )
{
	if ( !gameObject->IsEnabled() ) {
		return;
	}

	// render children
	for ( int i = 0; i < gameObject->GetChildNodeCount(); ++i ) {
		_RecRenderText3D( CAST_S(GameObject*, gameObject->GetChildNodeAt(i)) );
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

	// bind material
	_BindMaterial( material );

	// bind text uniforms
	{
		int uniform = shader->GetUniformLocation( UNIFORM_TEXT_COLOR );
		_gl->Uniform4fv( uniform, 1, text3D->GetTextColor().Ptr() );
	}

	// draw font chars
	for ( uchar c = 0; c < 10; c++ ) {
		// get font char
		FontChar* fontChar = font->GetFontChar( c );

		// bind texture
		int uniform = shader->GetUniformLocation( UNIFORM_TEXT_TEXTURE );
		_gl->ActiveTexture( 0 );
		_gl->BindTexture( GLTexture::TEXTURE_2D, fontChar->textureID );
		_gl->Uniform1i( uniform, 0 );

		// calculate texture size and posistion
		float w = fontChar->width;
		float h = fontChar->height;
		float x = fontChar->bearingX;
		float y = fontChar->bearingY - fontChar->height;
		
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
	}

	// draw
	//_gl->DrawElements( GLDrawMode::TRIANGLES, mesh->GetNumIndices(), GLVarType::UNSIGNED_INT, 0 );

	// unbind all
	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			EMPTY_BUFFER );
	_gl->UseProgram( EMPTY_SHADER );
}


void ForwardRenderer::_BindMaterial( Material* material )
{
	Shader* shader = material->GetShader();

	ASSERT( shader != NULL );

	//! set uniforms from material
	int nextActiveTexture = 0;
	int totalProperties = material->GetPropertiesCount();

	for ( int i = 0; i < totalProperties; ++i ) {
		cchar* uniformName = material->GetPropertyNameAt( i );
		int uniform = shader->GetUniformLocation( uniformName );
		DataUnion& data = material->GetPropertyDataAt( i );

		switch ( data.GetValueType() ) {
			case DataUnion::INT:
				_gl->Uniform1i( uniform, data.GetInt() );
				break;

			case DataUnion::FLOAT:
				_gl->Uniform1f( uniform, data.GetFloat() );
				break;

			case DataUnion::VECTOR2:
				_gl->Uniform2fv( uniform, 1, data.GetVector2().Ptr() );
				break;

			case DataUnion::VECTOR3:
				_gl->Uniform3fv( uniform, 1, data.GetVector3().Ptr() );
				break;

			case DataUnion::VECTOR4:
				_gl->Uniform4fv( uniform, 1, data.GetVector4().Ptr() );
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

				_gl->Uniform1i( uniform, nextActiveTexture );

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
