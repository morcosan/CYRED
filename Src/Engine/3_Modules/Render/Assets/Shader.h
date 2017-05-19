// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
		const char*	GetExtension	() override;


	public:
		void SetShaderFiles		( const char* rendererType, const char* vertexPath,
								  const char* geometryPath, const char* fragmentPath );

		void GetShaderFiles		( const char* rendererType, OUT const char** vertexPath,
								  OUT const char** geometryPath, 
								  OUT const char** fragmentPath ) const;

		//! must be called before using the shader
		//! setting the same renderer, does a reload of shaders
		void ChangeRenderer		( const char* rendererType );
		//! allows you to add the code directly
		void ChangeRenderer		( const char* vertexShader, const char* geometryShader, 
								  const char* fragmentShader );

		// getters
		int	GetProgramID		()						const;
		int	GetUniformLocation	( const char* uniform )	const;


	private:
		struct _FilesPaths
		{
			String vertex;
			String geometry;
			String fragment;
		};

		DataMap<String, _FilesPaths*>	_shaderFiles;
		int								_programID;
		DataMap<String, int>			_uniforms;
	};
}
