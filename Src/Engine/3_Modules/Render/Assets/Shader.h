// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Data/DataMap.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"
#include "../../../2_BuildingBlocks/Asset.h"


namespace CYRED
{
	class DLL Shader : public Asset
	{
	public:
		Shader();
		virtual ~Shader();


	public:
		void		LoadUniqueID	() override;
		void		LoadFullFile	() override;
		void		ClearAsset		() override;
		Asset*		Clone			() override;
		const Char*	GetExtension	() override;


	public:
		void SetShaderFiles		( const Char* rendererType, const Char* vertexPath,
								  const Char* geometryPath, const Char* fragmentPath );

		void GetShaderFiles		( const Char* rendererType, OUT const Char** vertexPath,
								  OUT const Char** geometryPath, OUT const Char** fragmentPath );

		//! must be called before using the shader
		//! setting the same renderer, does a reload of shaders
		void ChangeRenderer		( const Char* rendererType );
		//! allows you to add the code directly
		void ChangeRenderer		( const Char* vertexShader, const Char* geometryShader, 
								  const Char* fragmentShader );

		// getters
		Int	GetProgramID		()						const;
		Int	GetUniformLocation	( const Char* uniform )	const;


	protected:
		struct _FilesPaths
		{
			String vertex;
			String geometry;
			String fragment;
		};

		DataMap<String, _FilesPaths*>	_shaderFiles;
		Int								_programID;
		DataMap<String, Int>			_uniforms;
	};
}
