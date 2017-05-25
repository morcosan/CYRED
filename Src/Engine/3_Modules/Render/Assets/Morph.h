// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Data/DataArray.h"
#include "../../../2_BuildingBlocks/Asset.h"

#include "Shader.h"
#include "../OpenGL/MorphVertex.h"



namespace CYRED
{
	class DLL Morph : public Asset
	{
	public:
		Morph();
		virtual ~Morph();


	public:
		void		LoadUniqueID	() override;
		void		LoadFullFile	() override;
		void		ClearAsset		() override;
		Asset*		Clone			() override;
		cchar*	GetExtension	() override;


	public:
		//! must be called in order for mesh to work
		void		BindToGPU		();

		int			GetVBO			()				const;		
		int			GetIBO			()				const;			
		int			GetNumIndices	()				const;	
		int			GetTotalStates	()				const;
		int			GetActiveStates	()				const;
		cchar*	GetFilePath		( int index )	const;

		void SetTotalStates	( int value );
		void SetFilePath	( int index, cchar* filePath );


	private:
		uint	_vbo;
		uint	_ibo;
		int		_numIndices;
		
		int		_totalStates;
		int		_activeStates;
		String	_filePaths[MORPH_LIMIT];
	};
}