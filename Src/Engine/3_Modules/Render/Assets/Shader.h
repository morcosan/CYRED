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
		cchar*	GetExtension	() override;


	public:
		void SetShaderFiles		( cchar* vertexPath, cchar* geometryPath, cchar* fragmentPath );
		void GetShaderFiles		( OUT cchar** vertexPath, OUT cchar** geometryPath, 
								  OUT cchar** fragmentPath ) const;
		void LoadShaderProgram	();
		int	 GetProgramID		()						const;
		int	 GetUniformLocation	( cchar* uniform )	const;


	private:
		String					_vertexFilePath;
		String					_geometryFilePath;
		String					_fragmentFilePath;
		int						_programID;
		DataMap<String, int>	_uniforms;
	};
}
