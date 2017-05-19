// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Math/Math.h"


namespace CYRED
{
	struct Vertex
	{
		Vector3 position;
		Vector4 color;
		Vector3 normal;
		Vector2 uv;
		Vector3 tangent;
		Vector3 bitangent;

		Vertex( Vector3 _pos )
			: position( _pos )
		{}

		Vertex( Vector3 _pos, Vector4 _color )
			: position( _pos )
			, color( _color )
		{}

		Vertex( Vector3 _pos, Vector3 _normal )
			: position( _pos )
			, normal( _normal )
		{}

		Vertex( Vector3 _pos, Vector2 _uv )
			: position( _pos )
			, uv( _uv )
		{}

		Vertex( Vector3 _pos, Vector3 _norm, Vector2 _uv )
			: position( _pos )
			, normal( _norm )
			, uv( _uv )
		{}

		Vertex( Vector3 _pos, Vector4 _color, Vector3 _norm, Vector2 _uv )
			: position( _pos )
			, color( _color )
			, normal( _norm )
			, uv( _uv )
		{}

		Vertex( Vector3 _pos, Vector4 _color, Vector3 _norm, Vector2 _uv, Vector3 _tan, Vector3 _bitan )
			: position( _pos )
			, color( _color )
			, normal( _norm )
			, uv( _uv )
			, tangent( _tan )
			, bitangent( _bitan )
		{}
	};
}