// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "Panel_Attributes.h"
#include "../AttrViewers/AttrViewer.h"
#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Script.h"
#include "../Settings/EditorSkin.h"

#include "..\AttrViewers\AttrViewer_GameObject.h"
#include "..\AttrViewers\AttrViewer_Transform.h"
#include "..\AttrViewers\AttrViewer_Camera.h"
#include "..\AttrViewers\AttrViewer_Light.h"
#include "..\AttrViewers\AttrViewer_ParticleEmitter.h"
#include "..\AttrViewers\AttrViewer_MeshRendering.h"
#include "..\AttrViewers\AttrViewer_MorphRendering.h"
#include "..\AttrViewers\AttrViewer_Scripter.h"
#include "..\AttrViewers\AttrViewer_Material.h"
#include "..\AttrViewers\AttrViewer_Mesh.h"
#include "..\AttrViewers\AttrViewer_Morph.h"
#include "..\AttrViewers\AttrViewer_Shader.h"
#include "..\AttrViewers\AttrViewer_Texture.h"
#include "..\AttrViewers\AttrViewer_Scene.h"
#include "..\AttrViewers\AttrViewer_Script.h"
#include "..\AttrViewers\AttrViewer_Prefab.h"
#include "..\AttrViewers\AttrViewer_CyredProj.h"

#include "QtWidgets\qboxlayout.h"
#include "QtWidgets\qtreewidget.h"
#include "QtWidgets\qpushbutton.h"


using namespace CYRED;



Panel_Attributes::Panel_Attributes()
	: _needsRefresh( FALSE )
{
	this->setWindowTitle( PANEL_TITLE );
	this->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
	this->setAllowedAreas( Qt::DockWidgetArea::AllDockWidgetAreas );
	this->setMinimumSize( MIN_SIZE.x, MIN_SIZE.y );

	_qtTree = Memory::Alloc<QTreeWidget>( this );
	_qtTree->setHeaderHidden( true );
	_qtTree->setDragEnabled( false );
	_qtTree->setObjectName( EditorSkin::ATTR_COMP_TREE );

	QVBoxLayout* vLayout = Memory::Alloc<QVBoxLayout>();
	vLayout->setSpacing( 15 );
	vLayout->setContentsMargins( 0, 0, 0, 10 );
	vLayout->addWidget( _qtTree );

	QWidget* layoutWidget = Memory::Alloc<QWidget>();
	layoutWidget->setLayout( vLayout );
	layoutWidget->setObjectName( EditorSkin::PANEL_LAYOUT );

	this->setWidget( layoutWidget );
}


Panel_Attributes::~Panel_Attributes()
{
	Iterator<String, _AttrViewer*> iter = _attrViewers.GetIterator();
	while ( iter.HasNext() ) {
		Memory::Free( iter.GetValue()->viewer );
		iter.Next();
	}
}


void Panel_Attributes::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	SetAttrViewer( ATTR_MATERIAL,			Memory::Alloc<AttrViewer_Material>() );
	SetAttrViewer( ATTR_MESH,				Memory::Alloc<AttrViewer_Mesh>() );
	SetAttrViewer( ATTR_MORPH,				Memory::Alloc<AttrViewer_Morph>() );
	SetAttrViewer( ATTR_SHADER,				Memory::Alloc<AttrViewer_Shader>() );
	SetAttrViewer( ATTR_TEXTURE,			Memory::Alloc<AttrViewer_Texture>() );
	SetAttrViewer( ATTR_SCENE,				Memory::Alloc<AttrViewer_Scene>() );
	SetAttrViewer( ATTR_SCRIPT,				Memory::Alloc<AttrViewer_Script>() );
	SetAttrViewer( ATTR_PREFAB,				Memory::Alloc<AttrViewer_Prefab>() );

	SetAttrViewer( ATTR_GAMEOBJECT,			Memory::Alloc<AttrViewer_GameObject>() );

	SetAttrViewer( ATTR_TRANSFORM,			Memory::Alloc<AttrViewer_Transform>() );
	SetAttrViewer( ATTR_CAMERA,				Memory::Alloc<AttrViewer_Camera>() );
	SetAttrViewer( ATTR_LIGHT,				Memory::Alloc<AttrViewer_Light>() );
	SetAttrViewer( ATTR_PARTICLES_EMITTER,	Memory::Alloc<AttrViewer_ParticleEmitter>() );
	SetAttrViewer( ATTR_MESH_RENDERING,		Memory::Alloc<AttrViewer_MeshRendering>() );
	SetAttrViewer( ATTR_MORPH_RENDERING,	Memory::Alloc<AttrViewer_MorphRendering>() );
	SetAttrViewer( ATTR_SCRIPTER,			Memory::Alloc<AttrViewer_Scripter>() );

	SetAttrViewer( ATTR_CYRED_PROJ,			Memory::Alloc<AttrViewer_CyredProj>() );

	_ClearPanel();

	// register events
	EventManager::Singleton()->RegisterListener( EventType::ALL, this );
}


void Panel_Attributes::Finalize()
{
	ASSERT( _isInitialized );

	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::ALL, this );
}


void Panel_Attributes::Update()
{
	ASSERT( _isInitialized );

	if ( _needsRefresh )
	{
		_needsRefresh = FALSE;

		// show active viewers
		Iterator<String, _AttrViewer*> iter = _attrViewers.GetIterator();
		while ( iter.HasNext() ) {
			if ( iter.GetValue()->needsRefresh ) {
				iter.GetValue()->viewer->ShowViewer();
				iter.GetValue()->needsRefresh = FALSE;
			}
			iter.Next();
		}

		// WARNING: this will cause huge lag in editor
		_qtTree->setStyleSheet( _qtTree->styleSheet() );
	}
}


void Panel_Attributes::OnEvent( EventType eType, void* eData )
{
	switch ( eType ) {
		case EventType::SELECT_GAMEOBJECT:
		{
			_target = eData;

			// clear panel
			_ClearPanel();

			// display game object
			Node* node = CAST_S( Node*, _target );
			GameObject* gameObject = CAST_D( GameObject*, node );
			_DisplayGameObject( gameObject );

			break;
		}

		case EventType::CHANGE_GAMEOBJECT:
		{
			if ( _target != NULL ) {
				if ( _target == eData ) {
					// display game object
					Node* node = CAST_S( Node*, _target );
					GameObject* gameObject = CAST_D( GameObject*, node );
					_DisplayGameObject( gameObject );
				}
				else {
					// check if target is prefab
					Asset* asset = CAST_S( Asset*, _target );
					if ( asset != NULL && asset->GetAssetType() == AssetType::PREFAB ) {
						// send asset change event
						EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
					}
				}
			}
			break;
		}

		case EventType::RENAME_GAMEOBJECT:
		{
			if ( _target != NULL ) {
				if ( _target == eData ) {
					ASSERT( _attrViewers.Has( ATTR_GAMEOBJECT ) );
					AttrViewer* gameObjectViewer = _attrViewers.Get( ATTR_GAMEOBJECT )->viewer;
					gameObjectViewer->UpdateGUI();
				}
				else {
					// check if target is prefab
					Asset* asset = CAST_S( Asset*, _target );
					if ( asset != NULL && asset->GetAssetType() == AssetType::PREFAB ) {
						// send asset change event
						EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
					}
				}
			}
			break;
		}

		case EventType::CHANGE_TRANSFORM:
		{
			Component* comp = CAST_S( Component*, eData );
			if ( _target != NULL ) {
				if ( _target == comp->GetGameObject() ) {
					ASSERT( _attrViewers.Has( ATTR_TRANSFORM ) );
					AttrViewer* viewer = _attrViewers.Get( ATTR_TRANSFORM )->viewer;
					viewer->UpdateGUI();
				}
				else {
					// check if target is prefab
					Asset* asset = CAST_S( Asset*, _target );
					if ( asset != NULL && asset->GetAssetType() == AssetType::PREFAB ) {
						// send asset change event
						EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
					}
				}
			}
			break;
		}
			
		case EventType::CHANGE_CAMERA:
		{
			Component* comp = CAST_S( Component*, eData );
			if ( _target != NULL ) {
				if ( _target == comp->GetGameObject() ) {
					ASSERT( _attrViewers.Has( ATTR_CAMERA ) );
					AttrViewer* viewer = _attrViewers.Get( ATTR_CAMERA )->viewer;
					viewer->UpdateGUI();
				}
			}
			break;
		}

		case EventType::CHANGE_MESH_RENDERING:
		{
			Component* comp = CAST_S( Component*, eData );
			if ( _target != NULL ) {
				if ( _target == comp->GetGameObject() ) {
					ASSERT( _attrViewers.Has( ATTR_MESH_RENDERING ) );
					AttrViewer* viewer = _attrViewers.Get( ATTR_MESH_RENDERING )->viewer;
					viewer->UpdateGUI();
				}
				else {
					// check if target is prefab
					Asset* asset = CAST_S( Asset*, _target );
					if ( asset != NULL && asset->GetAssetType() == AssetType::PREFAB ) {
						// send asset change event
						EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
					}
				}
			}
			break;
		}

		case EventType::CHANGE_MORPH_RENDERING:
		{
			Component* comp = CAST_S( Component*, eData );
			if ( _target != NULL ) {
				if ( _target == comp->GetGameObject() ) {
					ASSERT( _attrViewers.Has( ATTR_MORPH_RENDERING ) );
					AttrViewer* viewer = _attrViewers.Get( ATTR_MORPH_RENDERING )->viewer;
					viewer->UpdateGUI();
				}
				else {
					// check if target is prefab
					Asset* asset = CAST_S( Asset*, _target );
					if ( asset != NULL && asset->GetAssetType() == AssetType::PREFAB ) {
						// send asset change event
						EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
					}
				}
			}
			break;
		}

		case EventType::CHANGE_PARTICLE_EMITTER:
		{
			Component* comp = CAST_S( Component*, eData );
			if ( _target != NULL ) {
				if ( _target == comp->GetGameObject() ) {
					ASSERT( _attrViewers.Has( ATTR_PARTICLES_EMITTER ) );
					AttrViewer* viewer = _attrViewers.Get( ATTR_PARTICLES_EMITTER )->viewer;
					viewer->UpdateGUI();
				}
				else {
					// check if target is prefab
					Asset* asset = CAST_S( Asset*, _target );
					if ( asset != NULL && asset->GetAssetType() == AssetType::PREFAB ) {
						// send asset change event
						EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
					}
				}
			}
			break;
		}

		case EventType::CHANGE_SCRIPTER:
		{
			Component* comp = CAST_S( Component*, eData );
			if ( _target != NULL ) {
				if ( _target == comp->GetGameObject() ) {
					ASSERT( _attrViewers.Has( ATTR_SCRIPTER ) );
					AttrViewer* viewer = _attrViewers.Get( ATTR_SCRIPTER )->viewer;
					viewer->UpdateGUI();
				}
				else {
					// check if target is prefab
					Asset* asset = CAST_S( Asset*, _target );
					if ( asset != NULL && asset->GetAssetType() == AssetType::PREFAB ) {
						// send asset change event
						EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
					}
				}
			}
			break;
		}

		case EventType::CHANGE_LIGHT:
		{
			Component* comp = CAST_S( Component*, eData );
			if ( _target != NULL ) {
				if ( _target == comp->GetGameObject() ) {
					ASSERT( _attrViewers.Has( ATTR_LIGHT ) );
					AttrViewer* viewer = _attrViewers.Get( ATTR_LIGHT )->viewer;
					viewer->UpdateGUI();
				}
				else {
					// check if target is prefab
					Asset* asset = CAST_S( Asset*, _target );
					if ( asset != NULL && asset->GetAssetType() == AssetType::PREFAB ) {
						// send asset change event
						EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
					}
				}
			}
			break;
		}
		
		case EventType::CHANGE_ASSET:
		{
			if ( _target == eData )	{
				Asset* asset = CAST_S( Asset*, eData );
				ASSERT( asset != NULL );

				const Char* attrViewerType = NULL;

				switch ( asset->GetAssetType() ) {
					case AssetType::MATERIAL:
						attrViewerType = ATTR_MATERIAL;
						break;

					case AssetType::MESH:
						attrViewerType = ATTR_MESH;
						break;

					case AssetType::MORPH:
						attrViewerType = ATTR_MORPH;
						break;

					case AssetType::TEXTURE:
						attrViewerType = ATTR_TEXTURE;
						break;

					case AssetType::SHADER:
						attrViewerType = ATTR_SHADER;
						break;

					case AssetType::SCENE:
						attrViewerType = ATTR_SCENE;
						break;

					case AssetType::SCRIPT:
						attrViewerType = ATTR_SCRIPT;
						break;

					case AssetType::PREFAB:
						attrViewerType = ATTR_PREFAB;
						break;
				}

				ASSERT( _attrViewers.Has( attrViewerType ) );
				AttrViewer* attrViewer = _attrViewers.Get( attrViewerType )->viewer;
				attrViewer->UpdateGUI();
			}
			break;
		}

		case EventType::SELECT_SCENE:
		case EventType::SELECT_PREFAB:
		case EventType::SELECT_ASSET:
		{
			_target = eData;
			_ClearPanel();

			if ( eData == NULL ) {
				break;
			}

			Asset* asset = CAST_S( Asset*, eData );
			ASSERT( asset != NULL );

			const Char* attrViewerType = NULL;

			switch ( asset->GetAssetType() ) {
				case AssetType::MATERIAL:
					attrViewerType = ATTR_MATERIAL;
					break;

				case AssetType::MESH:
					attrViewerType = ATTR_MESH;
					break;

				case AssetType::MORPH:
					attrViewerType = ATTR_MORPH;
					break;

				case AssetType::TEXTURE:
					attrViewerType = ATTR_TEXTURE;
					break;

				case AssetType::SHADER:
					attrViewerType = ATTR_SHADER;
					break;

				case AssetType::SCENE:
					attrViewerType = ATTR_SCENE;
					break;

				case AssetType::SCRIPT:
					attrViewerType = ATTR_SCRIPT;
					break;

				case AssetType::PREFAB:
					attrViewerType = ATTR_PREFAB;
					break;
			}

			ASSERT( _attrViewers.Has( attrViewerType ) );
			_AttrViewer* atttrViewer = _attrViewers.Get( attrViewerType );
			atttrViewer->needsRefresh = TRUE;
			AttrViewer* viewer = atttrViewer->viewer;
			viewer->ChangeTarget( asset );
			viewer->UpdateGUI();

			break;
		}

		case EventType::EDITOR_PROJ_SETTINGS:
		{
			ASSERT( _attrViewers.Has( ATTR_CYRED_PROJ ) );
			_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_CYRED_PROJ );
			atttrViewer->needsRefresh = TRUE;
			AttrViewer* viewer = atttrViewer->viewer;
			viewer->ChangeTarget( NULL );
			viewer->UpdateGUI();
			break;
		}
	}
}


void Panel_Attributes::SetAttrViewer( const Char* typeName, AttrViewer* viewer )
{
	ASSERT( _isInitialized );

	_AttrViewer* attrViewer = Memory::Alloc<_AttrViewer>();
	attrViewer->viewer = viewer;
	attrViewer->needsRefresh = FALSE;

	// add to list
	_attrViewers.Set( typeName, attrViewer );

	// initialize viewer
	viewer->Initialize( this, _qtTree );
}


void Panel_Attributes::RefreshPanel()
{
	ASSERT( _isInitialized );
	_needsRefresh = TRUE;
}


void Panel_Attributes::_ClearPanel()
{
	ASSERT( _isInitialized );

	// hide widgets
	for ( Int i = 0; i < _qtTree->topLevelItemCount(); ++i ) {
		QTreeWidgetItem* item = _qtTree->topLevelItem( i );
		item->setHidden( TRUE );
	}

	// disable viewers
	Iterator<String, _AttrViewer*> iter = _attrViewers.GetIterator();
	while ( iter.HasNext() ) {
		iter.GetValue()->needsRefresh = FALSE;
		iter.Next();
	}
}


void Panel_Attributes::_DisplayGameObject( GameObject* gameObject )
{
	if ( gameObject != NULL ) {
		{
			ASSERT( _attrViewers.Has( ATTR_GAMEOBJECT ) );
			_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_GAMEOBJECT );
			atttrViewer->needsRefresh = TRUE;
			AttrViewer* viewer = atttrViewer->viewer;
			viewer->ChangeTarget( gameObject );
			viewer->UpdateGUI();
		}
		{
			Component* comp = gameObject->GetComponent<Transform>();
			if ( comp != NULL )	{
				ASSERT( _attrViewers.Has( ATTR_TRANSFORM ) );
				_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_TRANSFORM );
				atttrViewer->needsRefresh = TRUE;
				AttrViewer* viewer = atttrViewer->viewer;
				viewer->ChangeTarget( comp );
				viewer->UpdateGUI();
			}
		}
		{
			Component* comp = gameObject->GetComponent<Camera>();
			if ( comp != NULL )	{
				ASSERT( _attrViewers.Has( ATTR_CAMERA ) );
				_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_CAMERA );
				atttrViewer->needsRefresh = TRUE;
				AttrViewer* viewer = atttrViewer->viewer;
				viewer->ChangeTarget( comp );
				viewer->UpdateGUI();
			}
		}
		{
			Component* comp = gameObject->GetComponent<Light>();
			if ( comp != NULL )	{
				ASSERT( _attrViewers.Has( ATTR_LIGHT ) );
				_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_LIGHT );
				atttrViewer->needsRefresh = TRUE;
				AttrViewer* viewer = atttrViewer->viewer;
				viewer->ChangeTarget( comp );
				viewer->UpdateGUI();
			}
		}
		{
			Component* comp = gameObject->GetComponent<ParticleEmitter>();
			if ( comp != NULL )	{
				ASSERT( _attrViewers.Has( ATTR_PARTICLES_EMITTER ) );
				_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_PARTICLES_EMITTER );
				atttrViewer->needsRefresh = TRUE;
				AttrViewer* viewer = atttrViewer->viewer;
				viewer->ChangeTarget( comp );
				viewer->UpdateGUI();
			}
		}
		{
			Component* comp = gameObject->GetComponent<MeshRendering>();
			if ( comp != NULL )	{
				ASSERT( _attrViewers.Has( ATTR_MESH_RENDERING ) );
				_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_MESH_RENDERING );
				atttrViewer->needsRefresh = TRUE;
				AttrViewer* viewer = atttrViewer->viewer;
				viewer->ChangeTarget( comp );
				viewer->UpdateGUI();
			}
		}
		{
			Component* comp = gameObject->GetComponent<MorphRendering>();
			if ( comp != NULL )	{
				ASSERT( _attrViewers.Has( ATTR_MORPH_RENDERING ) );
				_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_MORPH_RENDERING );
				atttrViewer->needsRefresh = TRUE;
				AttrViewer* viewer = atttrViewer->viewer;
				viewer->ChangeTarget( comp );
				viewer->UpdateGUI();
			}
		}
		{
			Component* comp = gameObject->GetComponent<Scripter>();
			if ( comp != NULL )
			{
				ASSERT( _attrViewers.Has( ATTR_SCRIPTER ) );
				_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_SCRIPTER );
				atttrViewer->needsRefresh = TRUE;
				AttrViewer* viewer = atttrViewer->viewer;
				viewer->ChangeTarget( comp );
				viewer->UpdateGUI();
			}
		}
	}
}
