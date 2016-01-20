// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "../../2_BuildingBlocks/String/String.h"


namespace CYRED
{
	class GameObject;
	class Scene;
}


namespace CYRED
{
	ABSTRACT class DLL SceneManager
	{
		DECLARE_LOCAL_SINGLETON( SceneManager )


	public:
		virtual void Initialize	()	PURE_VIRTUAL;
		virtual void Finalize	()	PURE_VIRTUAL;

		virtual Scene*	OpenScene		( const Char* sceneUID )	PURE_VIRTUAL;
		virtual Scene*	OpenNewScene	()							PURE_VIRTUAL;
		virtual Scene*	LoadScene		( const Char* sceneUID )	PURE_VIRTUAL;
		virtual Scene*	LoadNewScene	()							PURE_VIRTUAL;
		virtual void	SaveScene		( const Char* sceneUID )	PURE_VIRTUAL;
		virtual Scene*	SaveSceneAs		( const Char* sceneUID, const Char* newSceneName,
										  const Char* dirPath )		PURE_VIRTUAL;
		virtual void	SaveAllScenes	()							PURE_VIRTUAL;
		virtual void	CloseScene		( const Char* sceneUID )	PURE_VIRTUAL;
		virtual void	CloseAllScenes	()							PURE_VIRTUAL;

		virtual GameObject*	NewGameObject	( UInt sceneIndex = 0 )			PURE_VIRTUAL;
		virtual GameObject*	NewGameObject	( const Char* sceneUID )		PURE_VIRTUAL;
		virtual GameObject*	Duplicate		( const GameObject* object )	PURE_VIRTUAL;
		virtual GameObject*	Search			( const Char* objectName, 
											  UInt sceneIndex = 0 )			PURE_VIRTUAL;
		virtual GameObject*	Search			( const Char* objectName, 
											  const Char* sceneUID )		PURE_VIRTUAL;
		virtual void		Destroy			( GameObject* object )			PURE_VIRTUAL;

		virtual Int			GetSceneIndex		( const Char* sceneUID )	PURE_VIRTUAL;
		virtual Scene*		GetScene			( const Char* sceneUID )	PURE_VIRTUAL;
		virtual Scene*		GetScene			( UInt sceneIndex = 0 )		PURE_VIRTUAL;
		virtual UInt		CountLoadedScenes	()							PURE_VIRTUAL;

		virtual	UInt		NextGameObjectUID	()							PURE_VIRTUAL;
	
		virtual void		SetMainCamera	( GameObject* cameraGO )		PURE_VIRTUAL;
		virtual GameObject*	GetMainCamera	()								PURE_VIRTUAL;
	};
}