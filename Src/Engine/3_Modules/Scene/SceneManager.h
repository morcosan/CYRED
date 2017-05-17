// Copyright (c) 2015 Morco (www.morco.ro)
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
		virtual void Initialize	()	PURE_VIRTUAL;
		virtual void Finalize	()	PURE_VIRTUAL;

		virtual Scene*	OpenScene		( const char* sceneUID )	PURE_VIRTUAL;
		virtual Scene*	OpenNewScene	()							PURE_VIRTUAL;
		virtual Scene*	LoadScene		( const char* sceneUID )	PURE_VIRTUAL;
		virtual Scene*	LoadNewScene	()							PURE_VIRTUAL;
		virtual void	SaveScene		( const char* sceneUID )	PURE_VIRTUAL;
		virtual Scene*	SaveSceneAs		( const char* sceneUID, 
										  const char* newSceneName,
										  const char* dirPath )		PURE_VIRTUAL;
		virtual void	SaveAllScenes	()							PURE_VIRTUAL;
		virtual void	CloseScene		( const char* sceneUID )	PURE_VIRTUAL;
		virtual void	CloseAllScenes	()							PURE_VIRTUAL;

		virtual void	StoreScenes		()							PURE_VIRTUAL;
		virtual void	RestoreScenes	()							PURE_VIRTUAL;

		virtual GameObject*	NewGameObject	( int sceneIndex = 0 )			PURE_VIRTUAL;
		virtual GameObject*	NewGameObject	( const char* sceneUID )		PURE_VIRTUAL;
		virtual GameObject*	Instantiate		( const Prefab* prefab,
											  int sceneIndex = 0 )			PURE_VIRTUAL;
		virtual GameObject*	Duplicate		( const GameObject* object )	PURE_VIRTUAL;
		virtual GameObject*	Search			( const char* objectName, 
											  const char* sceneUID = NULL )	PURE_VIRTUAL;
		virtual void		Destroy			( GameObject* object )			PURE_VIRTUAL;

		virtual int			GetSceneIndex		( const char* sceneUID )	PURE_VIRTUAL;
		virtual Scene*		GetScene			( const char* sceneUID )	PURE_VIRTUAL;
		virtual Scene*		GetScene			( int sceneIndex = 0 )		PURE_VIRTUAL;
		virtual int		CountLoadedScenes	()							PURE_VIRTUAL;

		virtual	int		NextGameObjectUID	()							PURE_VIRTUAL;
	
		virtual void		SetMainCamera	( GameObject* cameraGO )		PURE_VIRTUAL;
		virtual GameObject*	GetMainCamera	()								PURE_VIRTUAL;
	};
}