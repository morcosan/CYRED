// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"

#include "../../../2_BuildingBlocks/Component.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Math/Vector4.h"

namespace CYRED
{
	class Font;
	class Shader;

	namespace Enum_VerticalAlign
	{
		enum Enum
		{
			TOP
			, MIDDLE
			, BOTTOM
		};
	}
	namespace Enum_HorizontalAlign
	{
		enum Enum
		{
			LEFT
			, MIDDLE
			, RIGHT
		};
	}
	typedef Enum_VerticalAlign::Enum	VerticalAlign;
	typedef Enum_HorizontalAlign::Enum	HorizontalAlign;
}


namespace CYRED
{
	class DLL Text3D : public Component
	{
	public:
		Text3D( GameObject* gameObject = NULL );
		virtual ~Text3D() {}


	public:
		void OnAdded	()							override {}
		void OnRemoved	()							override {}
		void Clone		( Component* clone ) const	override {}


	public:
		cchar*				GetText				()	const;
		Vector4				GetTextColor		()	const;
		Font*				GetFont				()	const;
		int					GetFontSize			()	const;
		VerticalAlign		GetVerticalAlign	()	const;
		HorizontalAlign		GetHorizontalAlign	()	const;
		Shader*				GetShader			()	const;

		void				SetText				( cchar* text );
		void				SetTextColor		( const Vector4& color );
		void				SetFont				( Font* font );
		void				SetFontSize			( int size );
		void				SetVerticalAlign	( VerticalAlign align );
		void				SetHorizontalAlign	( HorizontalAlign align );
		void				SetShader			( Shader* shader );


	private:
		void _OnEnable() override {}


	private:
		String				_text;
		Vector4				_textColor;
		VerticalAlign		_verticalAlign;
		HorizontalAlign		_horizontalAlign;
		Font*				_font;
		int					_fontSize;
		Shader*				_shader;
	};
}