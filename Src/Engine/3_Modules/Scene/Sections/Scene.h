// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../../1_Required/Required.h"
#include "../../../2_BuildingBlocks/String/String.h"
#include "../../../2_BuildingBlocks/Asset.h"


namespace CYRED
{
	class GameObject;
	class Node;
}


namespace CYRED
{
	class DLL Scene : public Asset
	{
	public:
		Scene();
		virtual ~Scene();


	public:
		void		LoadUniqueID	() override;
		void		LoadFullFile	() override;
		void		ClearAsset		() override;
		Asset*		Clone			() override;
		cchar*	GetExtension	() override;


	public:
		void	OnUpdate	( bool isRuntime = TRUE );

		Node*	GetRoot		() const;
		void	ClearRoot	();


	private:
		GameObject*	_mainCameraGO;
		Node*		_root;
	};

}