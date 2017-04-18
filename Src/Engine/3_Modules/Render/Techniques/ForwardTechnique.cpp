// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "ForwardTechnique.h"

#include "../../Scene/Sections/Scene.h"
#include "../../Scene/SceneManagerImpl.h"

#include "../../../2_BuildingBlocks/Components/Transform.h"
#include "../../../2_BuildingBlocks/GameObject.h"
#include "../../../2_BuildingBlocks/String/FiniteString.h"

#include "../Components/Camera.h"
#include "../Components/Light.h"
#include "../Components/MeshRendering.h"
#include "../Components/MorphRendering.h"
#include "../Components/ParticleEmitter.h"

#include "../Assets/Material.h"
#include "../Assets/Mesh.h"
#include "../Assets/Morph.h"
#include "../Assets/Shader.h"
#include "../Assets/Texture.h"

#include "../OpenGL/GL.h"
#include "../OpenGL/Uniform.h"

#include "../../Time/TimeManager.h"


using namespace CYRED;
using namespace NotAPI;



void ForwardTechnique::Render( UInt* buffers, Scene* scene, GameObject* cameraGO )
{
	ASSERT( _gl != NULL );

	_mainFramebufferID = buffers[ 0 ];

	_ClearScreen();

	if ( scene == NULL || cameraGO == NULL ) { 
		return;
	}

	// get camera
	_cameraTran = cameraGO->GetComponent<Transform>();
	_cameraComp = cameraGO->GetComponent<Camera>();

	// get scene root
	Node* sceneRoot = scene->GetRoot();

	_gl->Enable( GLCapability::BLEND );
	_gl->BlendEquation( GLBlendMode::FUNC_ADD );
	_gl->BlendFunc( GLBlendFactor::SRC_ALPHA, GLBlendFactor::ONE_MINUS_SRC_ALPHA );
	_gl->Enable( GLCapability::DEPTH_TEST );
	_gl->DepthFunc( GLDepthFunc::LEQUAL );
	_gl->DepthMask( TRUE );

	// render mesh
	for ( UInt i = 0; i < sceneRoot->GetChildNodeCount(); ++i ) {
		_RecRenderMesh( CAST_S( GameObject*, sceneRoot->GetChildNodeAt( i ) ) );
	}
	// render morph
	for ( UInt i = 0; i < sceneRoot->GetChildNodeCount(); ++i ) {
		_RecRenderMorph( CAST_S( GameObject*, sceneRoot->GetChildNodeAt( i ) ) );
	}

	_gl->Enable( GLCapability::BLEND );
	_gl->BlendFunc( GLBlendFactor::SRC_ALPHA, GLBlendFactor::ONE );
	_gl->DepthMask( FALSE );

	// render particles
	for ( UInt i = 0; i < sceneRoot->GetChildNodeCount(); ++i )	{
		_RecRenderParticles( CAST_S( GameObject*, sceneRoot->GetChildNodeAt( i ) ) );
	}
}


void ForwardTechnique::_ClearScreen()
{
	ASSERT( _gl != NULL );

	_gl->Viewport( 0, 0, _viewportWidth, _viewportHeight );

	_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, EMPTY_BUFFER );
	_gl->ClearColor( 0.3f, 0.3f, 0.3f, 1 );
	_gl->DepthMask( TRUE );
    _gl->Clear( GLBufferBit::COLOR_BUFFER_BIT | GLBufferBit::DEPTH_BUFFER_BIT );

	//_gl->BindFramebuffer( GLFrameBuffer::FRAMEBUFFER, _mainFramebufferID );
	//_gl->ClearColor( 0, 0, 0, 0 );
 //   _gl->Clear( GLFlag::COLOR_BUFFER_BIT | GLFlag::DEPTH_BUFFER_BIT );
}


void ForwardTechnique::_RecRenderMesh( GameObject* gameObject )
{
	if ( !gameObject->IsEnabled() ) {
		return;
	}

	// render child nodes
	for ( UInt i = 0; i < gameObject->GetChildNodeCount(); ++i ) {
		_RecRenderMesh( CAST_S( GameObject*, gameObject->GetChildNodeAt( i ) ) );
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

	UInt shaderProgram = shader->GetProgramID();
	if ( shaderProgram == INVALID_SHADER ) {
		return;
	}

	// add shader
	_gl->UseProgram( shaderProgram );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			mesh->GetVBO() );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	mesh->GetIBO() );


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

	Int worldUniform			= shader->GetUniformLocation( Uniform::WORLD		);
    Int viewUniform				= shader->GetUniformLocation( Uniform::VIEW			);
    Int projectionUniform		= shader->GetUniformLocation( Uniform::PROJECTION	);

	Matrix4 worldMatrix			= objTran->GetWorldMatrix();
	Matrix4 viewMatrix			= _cameraTran->GetViewMatrix();
	Matrix4 projectionMatrix	= _cameraComp->GetProjectionMatrix();

	_gl->UniformMatrix4fv( worldUniform,		1, FALSE, worldMatrix.Ptr()		 );
	_gl->UniformMatrix4fv( viewUniform,			1, FALSE, viewMatrix.Ptr()		 );
	_gl->UniformMatrix4fv( projectionUniform,	1, FALSE, projectionMatrix.Ptr() );

    Int cameraPosWorldUniform = shader->GetUniformLocation( Uniform::CAMERA_POS_WORLD );
	_gl->Uniform3fv( cameraPosWorldUniform,	1, _cameraTran->GetPositionWorld().Ptr() );

	// add material
	_BindMaterial( material );


	// add lights if needed
	{
		// check if shader contains light uniforms
		Int lightsUniform		= shader->GetUniformLocation( Uniform::LIGHTS );
		Int lightsCountUniform	= shader->GetUniformLocation( Uniform::LIGHTS_COUNT );
		Int ambientColorUniform	= shader->GetUniformLocation( Uniform::AMBIENT_COLOR );

		// check if lights needed
		if ( lightsUniform != -1 || lightsCountUniform != -1 ) {
			// get closest lights
			DataArray<GameObject*> lightsGO;
			SceneManagerImpl::Singleton()->FindClosestLights( gameObject, lightsGO );
			
			// add lights count
			_gl->Uniform1i( lightsCountUniform, lightsGO.Size() );

			// bind each light
			for ( UInt i = 0; i < lightsGO.Size() && i < Uniform::MAX_LIGHTS; i++ ) {
				Light* light = lightsGO[i]->GetComponent<Light>();
				Transform* transform = lightsGO[i]->GetComponent<Transform>();
				FiniteString uniformName;
				Int uniform;

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


void ForwardTechnique::_RecRenderMorph( GameObject* gameObject )
{
	if ( !gameObject->IsEnabled() ) {
		return;
	}

	// render child nodes
	for ( UInt i = 0; i < gameObject->GetChildNodeCount(); ++i ) {
		_RecRenderMorph( CAST_S( GameObject*, gameObject->GetChildNodeAt( i ) ) );
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

	UInt shaderProgram = shader->GetProgramID();
	if ( shaderProgram == INVALID_SHADER ) {
		return;
	}

	_gl->UseProgram( shaderProgram );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			morph->GetVBO() );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	morph->GetIBO() );


	UInt firstState = morphRender->GetFirstState();
	UInt secondState = morphRender->GetSecondState();

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

	Int worldUniform			= shader->GetUniformLocation( Uniform::WORLD		);
    Int viewUniform				= shader->GetUniformLocation( Uniform::VIEW			);
    Int projectionUniform		= shader->GetUniformLocation( Uniform::PROJECTION	);

	Matrix4 worldMatrix			= objTran->GetWorldMatrix();
	Matrix4 viewMatrix			= _cameraTran->GetViewMatrix();
	Matrix4 projectionMatrix	= _cameraComp->GetProjectionMatrix();

	_gl->UniformMatrix4fv( worldUniform,		1, FALSE, worldMatrix.Ptr()			);
	_gl->UniformMatrix4fv( viewUniform,			1, FALSE, viewMatrix.Ptr()			);
	_gl->UniformMatrix4fv( projectionUniform,	1, FALSE, projectionMatrix.Ptr()	);

	// morph rendering curstom uniforms
	Int stateRatioUniform = shader->GetUniformLocation( UNIFORM_STATE_RATIO );
	_gl->Uniform1f( stateRatioUniform, morphRender->GetStateRatio() );

	// add material
	_BindMaterial( material );


	// add lights if needed
	{
		// check if shader contains light uniforms
		Int lightsUniform		= shader->GetUniformLocation( Uniform::LIGHTS );
		Int lightsCountUniform	= shader->GetUniformLocation( Uniform::LIGHTS_COUNT );
		Int ambientColorUniform	= shader->GetUniformLocation( Uniform::AMBIENT_COLOR );

		// check if lights needed
		if ( lightsUniform != -1 || lightsCountUniform != -1 ) {
			// get closest lights
			DataArray<GameObject*> lightsGO;
			SceneManagerImpl::Singleton()->FindClosestLights( gameObject, lightsGO );

			// add lights count
			_gl->Uniform1i( lightsCountUniform, lightsGO.Size() );

			// bind each light
			for ( UInt i = 0; i < lightsGO.Size() && i < Uniform::MAX_LIGHTS; i++ ) {
				Light* light = lightsGO[i]->GetComponent<Light>();
				Transform* transform = lightsGO[i]->GetComponent<Transform>();
				FiniteString uniformName;
				Int uniform;

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


void ForwardTechnique::_RecRenderParticles( GameObject* gameObject )
{
	if ( !gameObject->IsEnabled() ) {
		return;
	}

	// render child nodes
	for ( UInt i = 0; i < gameObject->GetChildNodeCount(); ++i ) {
		_RecRenderParticles( CAST_S( GameObject*, gameObject->GetChildNodeAt( i ) ) );
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

	UInt shaderProgram = shader->GetProgramID();
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

	Int worldUniform			= shader->GetUniformLocation( Uniform::WORLD		);
	Int viewUniform				= shader->GetUniformLocation( Uniform::VIEW			);
	Int projectionUniform		= shader->GetUniformLocation( Uniform::PROJECTION	);

	Matrix4 worldMatrix			= objTran->GetWorldMatrix();
	Matrix4 viewMatrix			= _cameraTran->GetViewMatrix();
	Matrix4 projectionMatrix	= _cameraComp->GetProjectionMatrix();

	_gl->UniformMatrix4fv( worldUniform,		1, FALSE, worldMatrix.Ptr()			);
	_gl->UniformMatrix4fv( viewUniform,			1, FALSE, viewMatrix.Ptr()			);
	_gl->UniformMatrix4fv( projectionUniform,	1, FALSE, projectionMatrix.Ptr()	);

	// particle emitter curstom uniforms
	{
		Int uniform = shader->GetUniformLocation( UNIFORM_IS_LOOPING );
		_gl->Uniform1i( uniform, (emitter->IsLooping() ? 1 : 0) );
	}
	{
		Int uniform = shader->GetUniformLocation( UNIFORM_LIFETIME );
		_gl->Uniform1f( uniform, emitter->GetParticleLifetime() );
	}
	{
		Int uniform = shader->GetUniformLocation( UNIFORM_DELTA_TIME );
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


void ForwardTechnique::_BindMaterial( Material* material )
{
	Shader* shader = material->GetShader();

	ASSERT( shader != NULL );

	//! set uniforms from material
	UInt nextActiveTexture = 0;
	UInt totalProperties = material->GetPropertiesCount();

	for ( UInt i = 0; i < totalProperties; ++i ) {
		const Char* uniformName = material->GetPropertyNameAt( i );
		Int location = shader->GetUniformLocation( uniformName );
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
		//_gl->glLineWidth( material->GetLineWidth() );  TODO
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


