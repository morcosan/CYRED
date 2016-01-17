// Copyright (c) 2015 Morco (www.morco.ro)
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
		void LoadUniqueID	() override;
		void LoadFullFile	() override;
		void ClearAsset		() override;


	public:
		void OnStart		( Bool isRuntime = TRUE );
		void OnUpdate		( Bool isRuntime = TRUE );

		void	SetMainCamera	( const Char* objectName );
		Node*	GetRoot			();


	protected:
		GameObject*	_mainCameraGO;
		Node*		_root;
	};

}