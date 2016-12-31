// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "ForwardTechnique.h"

#include "../../Scene/Sections/Scene.h"

#include "../../../2_BuildingBlocks/Components/Transform.h"
#include "../../../2_BuildingBlocks/GameObject.h"

#include "../Components/Camera.h"
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

	if ( scene == NULL || cameraGO == NULL )
	{ 
		return;
	}

	_cameraTran = cameraGO->GetComponent<Transform>();
	_cameraComp = cameraGO->GetComponent<Camera>();

	Node* sceneRoot = scene->GetRoot();

	_gl->Enable( GLCapability::BLEND );
	_gl->BlendEquation( GLBlendMode::FUNC_ADD );
	_gl->BlendFunc( GLBlendFactor::SRC_ALPHA, GLBlendFactor::ONE_MINUS_SRC_ALPHA );
	_gl->Enable( GLCapability::DEPTH_TEST );
	_gl->DepthFunc( GLDepthFunc::LEQUAL );
	_gl->DepthMask( TRUE );

	for ( UInt i = 0; i < sceneRoot->GetChildNodeCount(); ++i )
	{
		_RenderMesh( CAST_S( GameObject*, sceneRoot->GetChildNodeAt( i ) ) );
	}
	for ( UInt i = 0; i < sceneRoot->GetChildNodeCount(); ++i )
	{
		_RenderMorph( CAST_S( GameObject*, sceneRoot->GetChildNodeAt( i ) ) );
	}

	_gl->Enable( GLCapability::BLEND );
	_gl->BlendFunc( GLBlendFactor::SRC_ALPHA, GLBlendFactor::ONE );
	_gl->DepthMask( FALSE );

	for ( UInt i = 0; i < sceneRoot->GetChildNodeCount(); ++i )
	{
		_RenderParticles( CAST_S( GameObject*, sceneRoot->GetChildNodeAt( i ) ) );
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


void ForwardTechnique::_RenderMesh( GameObject* gameObject )
{
	if ( !gameObject->IsEnabled() )
	{
		return;
	}

	MeshRendering*	meshRender	= gameObject->GetComponent<MeshRendering>();
	Transform*		objTran		= gameObject->GetComponent<Transform>();
	if ( meshRender == NULL || objTran == NULL ||
		!meshRender->IsEnabled() || !objTran->IsEnabled() )
	{
		return;
	}

	Material*	material	= meshRender->GetMaterial();
	Mesh*		mesh		= meshRender->GetMesh();
	if ( material == NULL || mesh == NULL )
	{
		return;
	}

	Shader* shader = material->GetShader();
	if ( shader == NULL )
	{
		return;
	}

	UInt shaderProgram = shader->GetProgramID();
	if ( shaderProgram == INVALID_SHADER )
	{
		return;
	}

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

	Int worldLoc			= shader->GetUniformLocation( Uniform::WORLD		);
    Int viewLoc				= shader->GetUniformLocation( Uniform::VIEW			);
    Int projectionLoc		= shader->GetUniformLocation( Uniform::PROJECTION	);

	Matrix4 worldMatrix			= objTran->GetWorldMatrix();
	Matrix4 viewMatrix			= _cameraTran->GetViewMatrix();
	Matrix4 projectionMatrix	= _cameraComp->GetProjectionMatrix();

	_gl->UniformMatrix4fv( worldLoc,		1, FALSE, worldMatrix.Ptr()		 );
	_gl->UniformMatrix4fv( viewLoc,			1, FALSE, viewMatrix.Ptr()		 );
	_gl->UniformMatrix4fv( projectionLoc,	1, FALSE, projectionMatrix.Ptr() );

    Int cameraPosWorldLoc = shader->GetUniformLocation( Uniform::CAMERA_POS_WORLD );
	_gl->Uniform3fv( cameraPosWorldLoc,	1, _cameraTran->GetPositionWorld().Ptr() );

	_BindMaterial( material );

	switch ( mesh->GetMeshType() )
	{
		case MeshType::POLYGON:
			_gl->DrawElements( GLDrawMode::TRIANGLES, mesh->GetNumIndices(), GLVarType::UNSIGNED_INT, 0 );
			break;

		case MeshType::LINE:
			_gl->DrawElements( GLDrawMode::LINES, mesh->GetNumIndices(), GLVarType::UNSIGNED_INT, 0 );
			break;
	}

	//! unbind all
	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			EMPTY_BUFFER );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	EMPTY_BUFFER );
	_gl->UseProgram( EMPTY_SHADER );
}


void ForwardTechnique::_RenderMorph( GameObject* gameObject )
{
	if ( !gameObject->IsEnabled() )
	{
		return;
	}

	MorphRendering*	morphRender	= gameObject->GetComponent<MorphRendering>();
	Transform*		objTran		= gameObject->GetComponent<Transform>();
	if ( morphRender == NULL || objTran == NULL ||
		!morphRender->IsEnabled() || !objTran->IsEnabled() )
	{
		return;
	}

	Material*	material	= morphRender->GetMaterial();
	Morph*		morph		= morphRender->GetMorph();
	if ( material == NULL || morph == NULL || morph->GetActiveStates() == 0 )
	{
		return;
	}

	Shader* shader = material->GetShader();
	if ( shader == NULL )
	{
		return;
	}

	UInt shaderProgram = shader->GetProgramID();
	if ( shaderProgram == INVALID_SHADER )
	{
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

	Int worldLoc			= shader->GetUniformLocation( Uniform::WORLD		);
    Int viewLoc				= shader->GetUniformLocation( Uniform::VIEW			);
    Int projectionLoc		= shader->GetUniformLocation( Uniform::PROJECTION	);

	Matrix4 worldMatrix			= objTran->GetWorldMatrix();
	Matrix4 viewMatrix			= _cameraTran->GetViewMatrix();
	Matrix4 projectionMatrix	= _cameraComp->GetProjectionMatrix();

	_gl->UniformMatrix4fv( worldLoc,		1, FALSE, worldMatrix.Ptr()			);
	_gl->UniformMatrix4fv( viewLoc,			1, FALSE, viewMatrix.Ptr()			);
	_gl->UniformMatrix4fv( projectionLoc,	1, FALSE, projectionMatrix.Ptr()	);

	//! morph rendering curstom uniforms
	{
		Int location = shader->GetUniformLocation( UNIFORM_STATE_RATIO );
		_gl->Uniform1f( location, morphRender->GetStateRatio() );
	}


	_BindMaterial( material );

	_gl->DrawElements( GLDrawMode::TRIANGLES, morph->GetNumIndices(), GLVarType::UNSIGNED_INT, 0 );

	//! unbind all
	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			EMPTY_BUFFER );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	EMPTY_BUFFER );
	_gl->UseProgram( EMPTY_SHADER );
}


void ForwardTechnique::_RenderParticles( GameObject* gameObject )
{
	if ( !gameObject->IsEnabled() )
	{
		return;
	}

	ParticleEmitter*	emitter	= gameObject->GetComponent<ParticleEmitter>();
	Transform*		objTran	= gameObject->GetComponent<Transform>();
	if ( emitter == NULL || objTran == NULL || 
		!emitter->IsEnabled() || !objTran->IsEnabled() )
	{
		return;
	}

	Material* material = emitter->GetMaterial();
	if ( material == NULL )
	{
		return;
	}

	Shader* shader = material->GetShader();
	if ( shader == NULL )
	{
		return;
	}

	UInt shaderProgram = shader->GetProgramID();
	if ( shaderProgram == INVALID_SHADER )
	{
		return;
	}

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

	Int worldLoc			= shader->GetUniformLocation( Uniform::WORLD		);
    Int viewLoc				= shader->GetUniformLocation( Uniform::VIEW			);
    Int projectionLoc		= shader->GetUniformLocation( Uniform::PROJECTION	);

	Matrix4 worldMatrix			= objTran->GetWorldMatrix();
	Matrix4 viewMatrix			= _cameraTran->GetViewMatrix();
	Matrix4 projectionMatrix	= _cameraComp->GetProjectionMatrix();

	_gl->UniformMatrix4fv( worldLoc,		1, FALSE, worldMatrix.Ptr()			);
	_gl->UniformMatrix4fv( viewLoc,			1, FALSE, viewMatrix.Ptr()			);
	_gl->UniformMatrix4fv( projectionLoc,	1, FALSE, projectionMatrix.Ptr()	);

	//! particle emitter curstom uniforms
	{
		Int location = shader->GetUniformLocation( UNIFORM_IS_LOOPING );
		_gl->Uniform1i( location, (emitter->IsLooping() ? 1 : 0) );
	}
	{
		Int location = shader->GetUniformLocation( UNIFORM_LIFETIME );
		_gl->Uniform1f( location, emitter->GetParticleLifetime() );
	}
	{
		Int location = shader->GetUniformLocation( UNIFORM_DELTA_TIME );
		_gl->Uniform1f( location, TimeManager::Singleton()->GetRenderDeltaTime() );
	}

	_BindMaterial( material );

	_gl->BindBufferBase( GLBaseBuffer::SHADER_STORAGE_BUFFER, 0, emitter->GetVBO() );

	_gl->DrawElements( GLDrawMode::POINTS, emitter->GetNumIndices(), GLVarType::UNSIGNED_INT, 0 );

	//! unbind all
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

	for ( UInt i = 0; i < totalProperties; ++i )
	{
		const Char* uniformName = material->GetPropertyNameAt( i );
		Int location = shader->GetUniformLocation( uniformName );
		DataUnion& data = material->GetPropertyDataAt( i );

		switch ( data.GetValueType() )
		{
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
				
				if ( texture != NULL )
				{
					switch ( texture->GetTextureType() )
					{
						case TextureType::TEXTURE_2D:
							_gl->BindTexture( GLTexture::TEXTURE_2D, texture->GetTextureID() );
							break;

						case TextureType::CUBE_MAP:
							_gl->BindTexture( GLTexture::CUBE_MAP, texture->GetTextureID() );
							break;
					}
				}
				else
				{
					// TODO
					// bind a pure white texture
				}

				_gl->Uniform1i( location, nextActiveTexture );
		
				++ nextActiveTexture;
				break;
		}
	}


	if ( material->IsWireframe() )
	{
		_gl->PolygonMode( GLPolygonFace::FRONT_AND_BACK, GLPolygonMode::LINE );
		//_gl->glLineWidth( material->GetLineWidth() );  TODO
		_gl->Disable( GLCapability::CULL_FACE );
	}
	else
	{
		_gl->PolygonMode( GLPolygonFace::FRONT_AND_BACK, GLPolygonMode::FILL );

		switch ( material->GetFaceCulling() )
		{
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


