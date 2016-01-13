// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "CyredRequired.h"
#include "CyredOpenGL_3_0.h"


class QOpenGLFunctions_3_0;


namespace CYRED
{
	namespace NotAPI
	{
		class GLImpl_3_0 : public GL_3_0
		{
		public:
			GLImpl_3_0() {}
			virtual ~GLImpl_3_0() {};

			void Initialize() override;
				   
			void Viewport	( Int x, Int y, UInt width, UInt height )	override;
			void ClearColor	( Float r, Float g, Float b, Float a )		override;
			void Clear		( UInt bitMask )							override;


		protected:
			QOpenGLFunctions_3_0*  _qtGL;
		};
	}
}