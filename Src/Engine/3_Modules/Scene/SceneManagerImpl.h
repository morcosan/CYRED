// Copyright (c) 2015-2017 Morco (www.morco.ro)
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
	namespace NonAPI
	{
		class SceneManagerImpl : public SceneManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( SceneManagerImpl )


		public:
			cchar* const NEW_SCENE			= "NewScene";
			cchar* const EMPTY_GAMEOBJECT	= "EmptyGameObject";


		public:
			void		Initialize			()							override;
			void		Finalize			()							override;
	
			Scene*		OpenScene			( cchar* sceneUID )			override;
			Scene*		OpenNewScene		()							override;
			Scene*		LoadScene			( cchar* sceneUID )			override;
			Scene*		LoadNewScene		()							override;
			void		SaveScene			( cchar* sceneUID )			override;
			Scene*		SaveSceneAs			( cchar* sceneUID, cchar* newSceneName,
											  cchar* dirPath )			override;
			void		SaveAllScenes		()							override;
			void		CloseScene			( cchar* sceneUID )			override;
			void		CloseAllScenes		()							override;

			void		StoreScenes			()							override;
			void		RestoreScenes		()							override;
			
			GameObject*	NewGameObject		( int sceneIndex = 0 )		override;
			GameObject*	NewGameObject		( cchar* sceneUID )			override;
			GameObject*	Instantiate			( const Prefab* prefab,
											  int sceneIndex = 0  )		override;
			GameObject*	Duplicate			( const GameObject* object )override;
			GameObject*	Search				( cchar* objectName, 
											  cchar* sceneUID = NULL )	override;
			GameObject*	FindGameObject		( cchar* objectName )		override;
			void		Destroy				( GameObject* object )		override;
			
			int			GetSceneIndex		( cchar* sceneUID )			override;
			Scene*		GetScene			( cchar* sceneUID )			override;
			Scene*		GetScene			( int sceneIndex = 0 )		override;
			int			CountLoadedScenes	()							override;

			int			NextGameObjectUID	()							override;

			void		SetMainCamera		( GameObject* cameraGO )	override;
			GameObject*	GetMainCamera		()							override;


		public:
			void		FindClosestLights( GameObject* target, OUT DataArray<GameObject*>& lightsGO );


		private:
			struct StoredScene
			{
				Scene* scene;
				String data;
			};

			DataArray<Scene*>		_currScenes;
			DataArray<StoredScene>	_storedScenes;

			int						_generatedUID;
			GameObject*				_mainCameraGO;


		private:
			GameObject* _RecFindActiveCamera( Node* parent );
		};
	}
}