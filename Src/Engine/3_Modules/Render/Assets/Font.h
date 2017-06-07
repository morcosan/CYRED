// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/Asset.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Data/DataMap.h"

struct FT_FaceRec_;
typedef struct FT_FaceRec_* FT_Face;

namespace CYRED
{
	struct FontChar
	{
		uint	textureID;
		int		width;
		int		height;
		int		bearingX;
		int		bearingY;
		int		advance;
	};
}


namespace CYRED
{
	class DLL Font : public Asset
	{
	public:
		Font();
		virtual ~Font();


	public:
		void	LoadUniqueID	() override;
		void	LoadFullFile	() override;
		void	ClearAsset		() override;
		Asset*	Clone			() override;
		cchar*	GetExtension	() override;


	public:
		void		BindToGPU		();

		cchar*		GetExternalPath	() const;
		FontChar*	GetFontChar		( char c ) const;

		void		SetExternalPath	( cchar* filePath );


	private:
		String						_externalPath;
		FT_Face						_freetypeFace;
		DataMap<char, FontChar*>	_fontChars;
	};
}