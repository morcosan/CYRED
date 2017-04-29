// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Asset.h"


namespace CYRED
{
	class Node;
}


namespace CYRED
{
	class DLL Prefab : public Asset
	{
	public:
		Prefab();
		virtual ~Prefab() {}


	public:
		void		LoadUniqueID	() override;
		void		LoadFullFile	() override;
		void		ClearAsset		() override;
		Asset*		Clone			() override;
		const Char*	GetExtension	() override;


	public:
		Node*	GetRoot		() const;
		void	CreateRoot	();


	protected:
		Node* _root;
	};
}
