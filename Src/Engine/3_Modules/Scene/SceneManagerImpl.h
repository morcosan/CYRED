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
	namespace NotAPI
	{
		class SceneManagerImpl : public SceneManager
		{
			DECLARE_LOCAL_SINGLETON_IMPL( SceneManagerImpl )


		public:
			const char* const NEW_SCENE			= "NewScene";
			const char* const EMPTY_GAMEOBJECT	= "EmptyGameObject";


		public:
			void		Initialize		()	override;
			void		Finalize		()	override;

			Scene*		OpenScene		( const char* sceneUID )		override;
			Scene*		OpenNewScene	()								override;
			Scene*		LoadScene		( const char* sceneUID )		override;
			Scene*		LoadNewScene	()								override;
			void		SaveScene		( const char* sceneUID )		override;
			Scene*		SaveSceneAs		( const char* sceneUID, 
										  const char* newSceneName,
										  const char* dirPath )			override;
			void		SaveAllScenes	()								override;
			void		CloseScene		( const char* sceneUID )		override;
			void		CloseAllScenes	()								override;

			void		StoreScenes		()								override;
			void		RestoreScenes	()								override;
			
			GameObject*	NewGameObject	( int sceneIndex = 0 )			override;
			GameObject*	NewGameObject	( const char* sceneUID )		override;
			GameObject*	Instantiate		( const Prefab* prefab,
										  int sceneIndex = 0  )		override;
			GameObject*	Duplicate		( const GameObject* object )	override;
			GameObject*	Search			( const char* objectName, 
										  const char* sceneUID = NULL )	override;
			void		Destroy			( GameObject* object )			override;
			
			int			GetSceneIndex		( const char* sceneUID )	override;
			Scene*		GetScene			( const char* sceneUID )	override;
			Scene*		GetScene			( int sceneIndex = 0 )		override;
			int		CountLoadedScenes	()							override;

			int		NextGameObjectUID	()							override;

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

			int					_generatedUID;
			GameObject*				_mainCameraGO;


		protected:
			GameObject* _RecFindActiveCamera( Node* parent );
		};
	}
}