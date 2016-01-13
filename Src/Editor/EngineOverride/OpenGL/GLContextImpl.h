// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredOpenGL.h"


class QWindow;
class QOpenGLContext;


namespace CYRED
{
	namespace NotAPI
	{
		class GLContextImpl : public GLContext
		{
		public:
			GLContextImpl( QWindow* qtWindow );
			virtual ~GLContextImpl() {};


			void MakeCurrent	() override;
			void SwapBuffers	() override;

			void OnResize	() override;

			virtual void SetSharedContext	( GLContext* other )	override;



		protected:
			QWindow*			_qtWindow;
			QOpenGLContext*		_qtContext;
		};
	}
}
