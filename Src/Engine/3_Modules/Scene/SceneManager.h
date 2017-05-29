// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "../../2_BuildingBlocks/String/String.h"


namespace CYRED
{
	class GameObject;
	class Scene;
	class Prefab;
}


namespace CYRED
{
	ABSTRACT class DLL SceneManager
	{
		DECLARE_LOCAL_SINGLETON( SceneManager )


	public:
		virtual void		Initialize			()								PURE_VIRTUAL;
		virtual void		Finalize			()								PURE_VIRTUAL;

		virtual Scene*		OpenScene			( cchar* sceneUID )				PURE_VIRTUAL;
		virtual Scene*		OpenNewScene		()								PURE_VIRTUAL;
		virtual Scene*		LoadScene			( cchar* sceneUID )				PURE_VIRTUAL;
		virtual Scene*		LoadNewScene		()								PURE_VIRTUAL;
		virtual void		SaveScene			( cchar* sceneUID )				PURE_VIRTUAL;
		virtual Scene*		SaveSceneAs			( cchar* sceneUID, cchar* newSceneName,
												  cchar* dirPath )				PURE_VIRTUAL;
		virtual void		SaveAllScenes		()								PURE_VIRTUAL;
		virtual void		CloseScene			( cchar* sceneUID )				PURE_VIRTUAL;
		virtual void		CloseAllScenes		()								PURE_VIRTUAL;
			
		virtual void		StoreScenes			()								PURE_VIRTUAL;
		virtual void		RestoreScenes		()								PURE_VIRTUAL;

		virtual GameObject*	NewGameObject		( int sceneIndex = 0 )			PURE_VIRTUAL;
		virtual GameObject*	NewGameObject		( cchar* sceneUID )				PURE_VIRTUAL;
		virtual GameObject*	Instantiate			( const Prefab* prefab,
												  int sceneIndex = 0 )			PURE_VIRTUAL;
		virtual GameObject*	Duplicate			( const GameObject* object )	PURE_VIRTUAL;
		virtual GameObject*	Search				( cchar* objectName, 
												  cchar* sceneUID = NULL )		PURE_VIRTUAL;
		virtual GameObject*	FindGameObject		( cchar* objectName )			PURE_VIRTUAL;

		virtual void		Destroy				( GameObject* object )			PURE_VIRTUAL;

		virtual int			GetSceneIndex		( cchar* sceneUID )				PURE_VIRTUAL;
		virtual Scene*		GetScene			( cchar* sceneUID )				PURE_VIRTUAL;
		virtual Scene*		GetScene			( int sceneIndex = 0 )			PURE_VIRTUAL;
		virtual int			CountLoadedScenes	()								PURE_VIRTUAL;
	
		virtual	int			NextGameObjectUID	()								PURE_VIRTUAL;
	
		virtual void		SetMainCamera		( GameObject* cameraGO )		PURE_VIRTUAL;
		virtual GameObject*	GetMainCamera		()								PURE_VIRTUAL;
	};
}