// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"

#include "SceneManager.h"
#include "../../2_BuildingBlocks/Data/DataArray.h"


namespace CYRED
{
	class Node;
	class GameObject;
}


namespace CYRED
{
	namespace NotAPI
	{
		class SceneManagerImpl : public SceneManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( SceneManagerImpl )


		public:
			const Char* const NEW_SCENE			= "NewScene";
			const Char* const EMPTY_GAMEOBJECT	= "EmptyGameObject";


		public:
			void		Initialize		()	override;
			void		Finalize		()	override;

			Scene*		OpenScene		( const Char* sceneUID )		override;
			Scene*		OpenNewScene	()								override;
			Scene*		LoadScene		( const Char* sceneUID )		override;
			Scene*		LoadNewScene	()								override;
			void		SaveScene		( const Char* sceneUID )		override;
			Scene*		SaveSceneAs		( const Char* sceneUID, 
										  const Char* newSceneName,
										  const Char* dirPath )			override;
			void		SaveAllScenes	()								override;
			void		CloseScene		( const Char* sceneUID )		override;
			void		CloseAllScenes	()								override;

			void		StoreScenes		()								override;
			void		RestoreScenes	()								override;
			
			GameObject*	NewGameObject	( UInt sceneIndex = 0 )			override;
			GameObject*	NewGameObject	( const Char* sceneUID )		override;
			GameObject*	Instantiate		( const Prefab* prefab,
										  UInt sceneIndex = 0  )		override;
			GameObject*	Duplicate		( const GameObject* object )	override;
			GameObject*	Search			( const Char* objectName, 
										  const Char* sceneUID = NULL )	override;
			void		Destroy			( GameObject* object )			override;
			
			Int			GetSceneIndex		( const Char* sceneUID )	override;
			Scene*		GetScene			( const Char* sceneUID )	override;
			Scene*		GetScene			( UInt sceneIndex = 0 )		override;
			UInt		CountLoadedScenes	()							override;

			UInt		NextGameObjectUID	()							override;

			void		SetMainCamera		( GameObject* cameraGO )	override;
			GameObject*	GetMainCamera		()							override;


		public:
			void		FindClosestLights( GameObject* target, OUT DataArray<GameObject*>& lightsGO );


		protected:
			struct StoredScene
			{
				Scene* scene;
				String data;
			};

			DataArray<Scene*>		_currScenes;
			DataArray<StoredScene>	_storedScenes;

			UInt					_generatedUID;
			GameObject*				_mainCameraGO;


		protected:
			GameObject* _RecFindActiveCamera( Node* parent );
		};
	}
}