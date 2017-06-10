// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "CyredRequired.h"
#include "CyredOpenGL.h"

class QWindow;
class QOpenGLContext;


namespace CYRED
{
	class GLContextImpl : public GLContext
	{
	public:
		GLContextImpl( QWindow* qtWindow );
		virtual ~GLContextImpl() {};


	public:
		void Create		() override;
		void MakeCurrent() override;
		void SwapBuffers() override;
		void OnResize	() override;

		virtual void SetSharedContext	( GLContext* other )	override;



	private:
		QWindow*			_qtWindow;
		QOpenGLContext*		_qtContext;
	};
}
