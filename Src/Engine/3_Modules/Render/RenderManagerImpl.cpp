// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "RenderManagerImpl.h"

#include "OpenGL\GL.h"
#include "OpenGL\GLContext.h"

#include "Renderers\ForwardRenderer.h"
#include "Renderers\PickingRenderer.h"

#include "../Debug/DebugManager.h"


using namespace CYRED;
using namespace NotAPI;


//! deferred definition of RenderManager
DEFINE_LOCAL_SINGLETON( RenderManager, RenderManagerImpl )
DEFINE_LOCAL_SINGLETON_IMPL( RenderManagerImpl )



/*****
* @desc: initialize manager by creating first canvas, with id 0
* @params: 
* 		glContext	- the context of the main window
* 		gl			- the OpenGL API
*/
void RenderManagerImpl::Initialize( GLContext* glContext, GL* gl )
{
	ASSERT( !_isInitialized );
	_isInitialized = true;

	// crate first canvas
	_Canvas canvas;
	canvas.glContext = glContext;
	canvas.glContext->Create();
	canvas.glContext->MakeCurrent();

	// add to list
	_canvases.Add( canvas );

	// 0 is main
	_currCanvas = 0;

	// store API
	_gl = gl;

	// before calling it, a context must be active and the window must be displayed
	_gl->Initialize();
}


/*****
* @desc: destroy the manager
* @assert: it was first initialized
*/
void RenderManagerImpl::Finalize()
{
	if ( !_isInitialized ) {
		return;
	}
}


/*****
* @desc: create a new canvas and activate it
* @params: 
* 		glContext - the context of the window
* @return: the index of the new canvas
*/
int RenderManagerImpl::CreateCanvas( GLContext* glContext )
{
	ASSERT( _isInitialized );
	ASSERT( glContext != NULL );

	_Canvas canvas;
	canvas.glContext = glContext;
	canvas.glContext->SetSharedContext( _canvases[0].glContext );
	canvas.glContext->Create();

	return _canvases.Add( canvas );
}


/*****
* @desc: change the current canvas
* @params: 
* 		canvasID - id of the new canvas
* @assert: canvas exists
*/
void RenderManagerImpl::SwitchCanvas( int canvasID )
{
	ASSERT( _isInitialized );
	ASSERT( canvasID >= 0 && canvasID < _canvases.Size() );

	_currCanvas = canvasID;
}


/*****
* @desc: create a new renderer for current canvas
* @params: 
* 		rendererType - type of the renderer
* @assert: canvas is set
*/
void RenderManagerImpl::CreateRenderer( RendererType rendererType )
{
	ASSERT( _isInitialized );

	// get canvas
	_Canvas& canvas = _canvases[_currCanvas];
	ASSERT( !canvas.renderers.Has( rendererType ) );

	// create new renderer
	Renderer* renderer = NULL;
	switch ( rendererType ) {
		case RendererType::GL_FORWARD:	renderer = Memory::Alloc<ForwardRenderer>();	break;
		case RendererType::GL_PICKING:	renderer = Memory::Alloc<PickingRenderer>();	break;
	}

	// set context
	canvas.glContext->MakeCurrent();
	// initialize renderer
	renderer->Initialize( _gl, canvas.glContext );
	// add to canvas
	canvas.renderers.Set( rendererType, renderer );
	// set as current
	_currRenderer = rendererType;
}


/*****
* @desc: change the current renderer
* @params: 
* 		rendererType - type of the renderer
* @assert: canvas is set
*/
void RenderManagerImpl::SwitchRenderer( RendererType rendererType )
{
	ASSERT( _isInitialized );

	// get canvas
	_Canvas& canvas = _canvases[_currCanvas];
	ASSERT( canvas.renderers.Has( rendererType ) );

	// set as current
	_currRenderer = rendererType;
	// set context
	canvas.glContext->MakeCurrent();
}


/*****
* @desc: clear the previous frame
*/
void RenderManagerImpl::ClearScreen( float r, float g, float b )
{
	ASSERT( _isInitialized );

	// get canvas
	_Canvas& canvas = _canvases[_currCanvas];
	// get renderer
	ASSERT( canvas.renderers.Has( _currRenderer ) );
	Renderer* renderer = canvas.renderers.Get( _currRenderer );

	// clear screen
	renderer->ClearScreen( r, g, b );
}


/*****
* @desc: clear the depth buffer; new rendering goes over anything before
*/
void RenderManagerImpl::ResetDepth()
{
	ASSERT( _isInitialized );

	// get canvas
	_Canvas& canvas = _canvases[_currCanvas];
	// get renderer
	ASSERT( canvas.renderers.Has( _currRenderer ) );
	Renderer* renderer = canvas.renderers.Get( _currRenderer );

	// clear screen
	renderer->ResetDepth();
}


/*****
* @desc: render the given component from given gameobject and its children
* @params: 
* 		compType	- the component to render
* 		target		- the target gameobject
* 		cameraGO	- camera
* 		lightsGO	- the list of lights to be used
* @assert: canvas and renderer are set
*/
void RenderManagerImpl::Render( ComponentType compType, Node* target, GameObject* cameraGO, 
								DataArray<GameObject*>& lightsGO )
{
	ASSERT( _isInitialized );

	// get canvas
	_Canvas& canvas = _canvases[_currCanvas];
	// get renderer
	ASSERT( canvas.renderers.Has( _currRenderer ) );
	Renderer* renderer = canvas.renderers.Get( _currRenderer );

	// render
	renderer->Render( compType, target, cameraGO, lightsGO );
	renderer->DisplayOnScreen();
}


/*****
* @desc: display the rendering
*/
void RenderManagerImpl::SwapBuffers()
{
	ASSERT( _isInitialized );

	// get canvas
	_Canvas& canvas = _canvases[_currCanvas];
	// swap buffers
	canvas.glContext->SwapBuffers();
}


/*****
* @desc: force resize for given canvas
* @params: 
* 		canvasID - id of canvas
* @assert: canvas exists
*/
void RenderManagerImpl::OnResize( int canvasID )
{
	ASSERT( _isInitialized );
	ASSERT( canvasID < _canvases.Size() );

	// get canvas
	_Canvas& canvas = _canvases[ canvasID ];
	// call resize
	canvas.glContext->OnResize();

	// resize all renderers
	Iterator<RendererType, Renderer*> iter = canvas.renderers.GetIterator();
	while ( iter.HasNext() ) {
		iter.GetValue()->OnResize();
		iter.Next();
	}
}


/*****
* @desc: read the pixel from renderer at given location
* @params: 
* 		x - location on x axis
* 		y - location on y axis
* @assert: canvas and renderer are set
*/
Vector4 RenderManagerImpl::ReadPixel( int x, int y )
{
	ASSERT( _isInitialized );

	// get canvas
	_Canvas& canvas = _canvases[_currCanvas];
	// get renderer
	ASSERT( canvas.renderers.Has( _currRenderer ) );
	Renderer* renderer = canvas.renderers.Get( _currRenderer );

	// read pixel
	return renderer->ReadPixel( x, y );
}


bool RenderManagerImpl::_IsShaderCompiled( int shaderID ) const
{
	int comp;
	_gl->GetShaderiv( shaderID, GLShaderInfo::COMPILE_STATUS, &comp );

	if( comp == FALSE )
	{
		char description[512];
		_gl->GetShaderInfoLog( shaderID, sizeof(description), 0, &description[0] );

		DebugManager::Singleton()->Log( description );

		return FALSE;
	}

	return TRUE;
}


bool RenderManagerImpl::_IsProgramLinked( int programID ) const
{
	int status;
	{
		_gl->GetProgramiv( programID, GLProgramInfo::LINK_STATUS, &status );

		if( status == FALSE ) {
			char description[512];
			_gl->GetProgramInfoLog( programID, sizeof(description), 0, &description[0] );

			DebugManager::Singleton()->Log( description );
			return FALSE;
		}
	}
	{
		_gl->ValidateProgram( programID );
		_gl->GetProgramiv( programID, GLProgramInfo::VALIDATE_STATUS, &status );
  	
		if( status == FALSE ) {
			char description[256];
			_gl->GetProgramInfoLog( programID, sizeof(description), 0, &description[0] );

			DebugManager::Singleton()->Log( description );
			return FALSE;
		}
	}
	return TRUE;
}


void RenderManagerImpl::CreateMeshBuffers( OUT uint& vbo, OUT uint& ibo,
										   DataArray<Vertex>& vertices, DataArray<int>& indices )
{
	ASSERT( _isInitialized );

	if ( vertices.Size() == 0 || indices.Size() == 0 ) {
		return;
	}

	if ( vbo == EMPTY_BUFFER ) {
		_gl->GenBuffers( 1, &vbo );
	}
  	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, vbo );
	_gl->BufferData( GLBuffer::ARRAY_BUFFER, sizeof(Vertex) * vertices.Size(), vertices.Data(), 
					 GLDrawType::STATIC_DRAW );

	if ( ibo == EMPTY_BUFFER ) {
		_gl->GenBuffers( 1, &ibo );
	}
    _gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER, ibo );
	_gl->BufferData( GLBuffer::ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.Size(), indices.Data(), 
					 GLDrawType::STATIC_DRAW );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			EMPTY_BUFFER );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	EMPTY_BUFFER );
}


void RenderManagerImpl::CreateMorphBuffers( OUT uint& vbo, OUT uint& ibo, 
										    DataArray<MorphVertex>& vertices, DataArray<int>& indices )
{
	ASSERT( _isInitialized );

	if ( vertices.Size() == 0 || indices.Size() == 0 ) {
		return;
	}

	if ( vbo == EMPTY_BUFFER ) {
		_gl->GenBuffers( 1, &vbo );
	}
  	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, vbo );
	_gl->BufferData( GLBuffer::ARRAY_BUFFER, sizeof(MorphVertex) * vertices.Size(), vertices.Data(), 
					 GLDrawType::STATIC_DRAW );

	if ( ibo == EMPTY_BUFFER ) {
		_gl->GenBuffers( 1, &ibo );
	}
    _gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER, ibo );
	_gl->BufferData( GLBuffer::ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.Size(), indices.Data(), 
					 GLDrawType::STATIC_DRAW );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			EMPTY_BUFFER );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	EMPTY_BUFFER );
}


void RenderManagerImpl::DeleteBuffers( uint vbo, uint ibo )
{
	_gl->DeleteBuffers( 1, &vbo );
	_gl->DeleteBuffers( 1, &ibo );
}


void RenderManagerImpl::CreateParticleBuffers( OUT uint& vbo, OUT uint& ibo, 
											   DataArray<ParticleVertex>& vertices, 
											   DataArray<int>& indices )
{
	ASSERT( _isInitialized );

	if ( vertices.Size() == 0 || indices.Size() == 0 ) {
		return;
	}

	if ( vbo == EMPTY_BUFFER ) {
		_gl->GenBuffers( 1, &vbo );
	}
  	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER, vbo );
	_gl->BufferData( GLBuffer::ARRAY_BUFFER, sizeof(ParticleVertex) * vertices.Size(), vertices.Data(), 
					 GLDrawType::STATIC_DRAW );

	if ( ibo == EMPTY_BUFFER ) {
		_gl->GenBuffers( 1, &ibo );
	}
    _gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER, ibo );
	_gl->BufferData( GLBuffer::ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.Size(), indices.Data(), 
					 GLDrawType::STATIC_DRAW );

	_gl->BindBuffer( GLBuffer::ARRAY_BUFFER,			EMPTY_BUFFER );
	_gl->BindBuffer( GLBuffer::ELEMENT_ARRAY_BUFFER,	EMPTY_BUFFER );
}


int RenderManagerImpl::CreateShaderProgram( cchar* vertexCode, 
											 cchar* geometryCode, 
											 cchar* fragmentCode )
{
	ASSERT( _isInitialized );
	ASSERT( vertexCode != NULL );
	ASSERT( geometryCode != NULL );
	ASSERT( fragmentCode != NULL );

    int programID = _gl->CreateProgram();
	bool isOk = TRUE;
	int vertexShader	= _gl->CreateShader( GLShader::VERTEX_SHADER );
	int geometryShader = _gl->CreateShader( GLShader::GEOMETRY_SHADER );
	int fragmentShader = _gl->CreateShader( GLShader::FRAGMENT_SHADER );

	{
		_gl->ShaderSource( vertexShader, 1, &vertexCode, NULL );
		_gl->CompileShader( vertexShader );

		if ( !_IsShaderCompiled( vertexShader ) ) {
			isOk = FALSE;
		}
	}

	if ( isOk ) {
		_gl->ShaderSource( geometryShader, 1, &geometryCode, NULL );
		_gl->CompileShader( geometryShader );

		if ( !_IsShaderCompiled( geometryShader ) ) {
			isOk = FALSE;
		}
	}

	if ( isOk ) {
		_gl->ShaderSource( fragmentShader, 1, &fragmentCode, NULL );
		_gl->CompileShader( fragmentShader );

		if ( !_IsShaderCompiled( fragmentShader ) ) {
			isOk = FALSE;
		}
	}

	if ( isOk ) {
		_gl->AttachShader( programID, vertexShader );
		_gl->AttachShader( programID, geometryShader );
		_gl->AttachShader( programID, fragmentShader );

		_gl->LinkProgram( programID );
		if ( !_IsProgramLinked( programID ) ) {
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

	if ( isOk ) {
		return programID;
	}
	else {
		_gl->DeleteProgram( programID );
		return INVALID_SHADER;
	}
}


void RenderManagerImpl::DeleteShaderProgram( int programID )
{
	_gl->DeleteProgram( programID );
}


int RenderManagerImpl::GetUniformLocation( int programID, cchar * uniform )
{
	ASSERT( _isInitialized );
	return _gl->GetUniformLocation( programID, uniform );
}


int RenderManagerImpl::GetUniformsCount( int programID )
{
	ASSERT( _isInitialized );

	int total = 0;
	_gl->GetProgramiv( programID, GLProgramInfo::ACTIVE_UNIFORMS, &total ); 

	return total;
}


void RenderManagerImpl::GetUniformInfo( int programID, int index, int buffSize,
										OUT int* length, OUT char* name )
{
	ASSERT( _isInitialized );

	int size = -1;
	GLUniform type;

	_gl->GetActiveUniform( programID, index, buffSize, length, &size, type, name );
}


void RenderManagerImpl::CreateTexture2D( OUT uint& textureID, int width, int height, 
										 int channels, bool hasMipmap, 
										 uchar* imageBuffer )
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


void RenderManagerImpl::CreateCubeMapTexture( OUT uint& textureID, int width, int height, 
											  int channels, bool hasMipmap,
											  uchar* imageBuffer_PosX, uchar* imageBuffer_NegX, 
											  uchar* imageBuffer_PosY, uchar* imageBuffer_NegY, 
											  uchar* imageBuffer_PosZ, uchar* imageBuffer_NegZ )
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


void RenderManagerImpl::DeleteTexture( uint textureID )
{
	_gl->DeleteTextures( 1, &textureID );
}

