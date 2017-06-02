// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "GLContextImpl.h"

#include "QtGui\QOpenGLContext"
#include "QtGui\qwindow.h"


using namespace CYRED;



GLContextImpl::GLContextImpl( QWindow* qtWindow )
	: _qtWindow( qtWindow )
	, _qtContext( new QOpenGLContext() ) 
{
}


void GLContextImpl::Create()
{
	_qtContext->create();
}


void GLContextImpl::MakeCurrent()
{
	_qtContext->makeCurrent( _qtWindow );
}


void GLContextImpl::SwapBuffers()
{
	_qtContext->swapBuffers( _qtWindow );
}


void GLContextImpl::OnResize()
{
	_width = _qtWindow->width();
	_height = _qtWindow->height();
}


void GLContextImpl::SetSharedContext( GLContext* other )
{
	GLContextImpl* otherContext = CAST_S( GLContextImpl*, other );

	_qtContext->setShareContext( otherContext->_qtContext );
}
