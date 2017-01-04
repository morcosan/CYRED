// Copyright (c) 2015 Morco (www.morco.ro)
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
		const Char*	GetExtension	() override;


	public:
		//! must be called in order for mesh to work
		void			BindToGPU		();

		UInt			GetVBO			()				const;		
		UInt			GetIBO			()				const;			
		UInt			GetNumIndices	()				const;	
		UInt			GetTotalStates	()				const;
		UInt			GetActiveStates	()				const;
		const Char*		GetFilePath		( UInt index )	const;

		void SetTotalStates	( UInt value );
		void SetFilePath	( UInt index, const Char* filePath );


	protected:
		UInt					_vbo;
		UInt					_ibo;
		UInt					_numIndices;
		
		UInt					_totalStates;
		UInt					_activeStates;
		String					_filePaths[MORPH_LIMIT];
	};
}