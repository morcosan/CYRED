// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Data/DataUnion.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"
#include "../../../2_BuildingBlocks/Asset.h"


namespace CYRED
{
	class Shader;
	class Texture;
}


namespace CYRED
{
	namespace Enum_FaceCulling
	{
		enum Enum
		{
			CULL_BACK
			, CULL_FRONT
			, CULL_NONE
		};
	}
	typedef Enum_FaceCulling::Enum FaceCulling;
}


namespace CYRED
{
	class DLL Material : public Asset
	{
	public:
		Material();
		virtual ~Material() {}


	public:
		void	LoadUniqueID	() override;
		void	LoadFullFile	() override;
		void	ClearAsset		() override;
		Asset*	Clone			() override;


	public:
		Shader*		GetShader		() const;
		bool		IsWireframe		() const;
		float		GetLineWidth	() const;
		FaceCulling	GetFaceCulling	() const;

		void		SetShader		( Shader* shader );		
		void		SetWireframe	( bool value );		
		void		SetLineWidth	( float value );		
		void		SetFaceCulling	( FaceCulling value );

		void		SetProperty		( const Char* name, Int value );
		void		SetProperty		( const Char* name, Float value );
		void		SetProperty		( const Char* name, const Vector2& value );
		void		SetProperty		( const Char* name, const Vector3& value );
		void		SetProperty		( const Char* name, const Vector4& value );
		void		SetProperty		( const Char* name, Texture* value );

		UInt		GetPropertiesCount	();
		DataUnion&	GetPropertyDataAt	( UInt index );
		const Char*	GetPropertyNameAt	( UInt index );
		void		ClearProperties		();


	protected:
		struct _Property
		{
			String		name;
			DataUnion	data;
		};

		Shader*			_shader;
		bool			_isWireframe;
		float			_lineWidth;
		FaceCulling		_faceCulling;

		DataArray<_Property> _properties;
	};
}
