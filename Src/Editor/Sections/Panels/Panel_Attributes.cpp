// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Panel_Attributes.h"

#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Script.h"
#include "CyredModule_Physics.h"

#include "../Settings/EditorSkin.h"
#include "../../Utils/EditorEvents.h"

#include "../AttrViewers/AttrViewer.h"
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
#include "..\AttrViewers\AttrViewer_RigidBody.h"
#include "..\AttrViewers\AttrViewer_Font.h"
#include "..\AttrViewers\AttrViewer_Text3D.h"

#include "QtWidgets\qboxlayout.h"
#include "QtWidgets\qtreewidget.h"
#include "QtWidgets\qpushbutton.h"


using namespace CYRED;



Panel_Attributes::Panel_Attributes()
	: _needsRefresh( FALSE )
{
	this->setWindowTitle( PANEL_TITLE );
	this->setMinimumSize( MIN_SIZE.x, MIN_SIZE.y );

	_qtTree = new QTreeWidget( this );
	_qtTree->setHeaderHidden( true );
	_qtTree->setDragEnabled( false );
	_qtTree->setObjectName( EditorSkin::ATTR_COMP_TREE );

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->setSpacing( 15 );
	vLayout->setContentsMargins( 0, 0, 0, 0 );
	vLayout->addWidget( _qtTree );

	QWidget* layoutWidget = new QWidget();
	layoutWidget->setLayout( vLayout );
	layoutWidget->setObjectName( EditorSkin::PANEL_LAYOUT );

	this->setWidget( layoutWidget );
}


Panel_Attributes::~Panel_Attributes()
{
	Iterator<String, _AttrViewer*> iter = _attrViewers.GetIterator();
	while ( iter.HasNext() ) {
		PTR_FREE( iter.GetValue()->viewer );
		iter.Next();
	}
}


void Panel_Attributes::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	SetAttrViewer( ATTR_MATERIAL,			new AttrViewer_Material() );
	SetAttrViewer( ATTR_MESH,				new AttrViewer_Mesh() );
	SetAttrViewer( ATTR_MORPH,				new AttrViewer_Morph() );
	SetAttrViewer( ATTR_SHADER,				new AttrViewer_Shader() );
	SetAttrViewer( ATTR_TEXTURE,			new AttrViewer_Texture() );
	SetAttrViewer( ATTR_SCENE,				new AttrViewer_Scene() );
	SetAttrViewer( ATTR_SCRIPT,				new AttrViewer_Script() );
	SetAttrViewer( ATTR_PREFAB,				new AttrViewer_Prefab() );
	SetAttrViewer( ATTR_FONT,				new AttrViewer_Font() );

	SetAttrViewer( ATTR_GAMEOBJECT,			new AttrViewer_GameObject() );

	SetAttrViewer( ATTR_TRANSFORM,			new AttrViewer_Transform() );
	SetAttrViewer( ATTR_CAMERA,				new AttrViewer_Camera() );
	SetAttrViewer( ATTR_LIGHT,				new AttrViewer_Light() );
	SetAttrViewer( ATTR_PARTICLES_EMITTER,	new AttrViewer_ParticleEmitter() );
	SetAttrViewer( ATTR_MESH_RENDERING,		new AttrViewer_MeshRendering() );
	SetAttrViewer( ATTR_MORPH_RENDERING,	new AttrViewer_MorphRendering() );
	SetAttrViewer( ATTR_SCRIPTER,			new AttrViewer_Scripter() );
	SetAttrViewer( ATTR_RIGID_BODY,			new AttrViewer_RigidBody() );
	SetAttrViewer( ATTR_TEXT_3D,			new AttrViewer_Text3D() );

	SetAttrViewer( ATTR_CYRED_PROJ,			new AttrViewer_CyredProj() );

	_ClearPanel();

	// register events
	EventManager::Singleton()->RegisterListener( this, EventType::ALL );
}


void Panel_Attributes::Finalize()
{
	ASSERT( _isInitialized );

	// unregister events
	EventManager::Singleton()->UnregisterListener( this, EventType::ALL );
}


void Panel_Attributes::Update( bool isRuntime )
{
	ASSERT( _isInitialized );

	if ( _needsRefresh ) {
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


void Panel_Attributes::OnEvent( int eventType, void* eventData )
{
	switch ( eventType ) {
		case EditorEventType::GAMEOBJECT_SELECT:
		case EditorEventType::ISOLATE_SELECT:
		{
			_target = eventData;

			// display game object
			Node* node = CAST_S( Node*, _target );
			GameObject* gameObject = CAST_D( GameObject*, node );
			_DisplayGameObject( gameObject );

			break;
		}

		case EventType::COMPONENT_ADD:
		case EventType::COMPONENT_REMOVE:
			if ( _target != NULL ) {
				if ( _target == CAST_S( Component*, eventData )->GetGameObject() ) {
					// refresh
					_DisplayGameObject( CAST_S( GameObject*, _target ) );
				}
			}
			break;

		case EventType::GAMEOBJECT_UPDATE:
			if ( _target != NULL ) {
				if ( _target == eventData ) {
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
						EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, asset );
					}
				}
			}
			break;

		case EventType::GAMEOBJECT_RENAME:
			if ( _target != NULL ) {
				if ( _target == eventData ) {
					AttrViewer* gameObjectViewer = _attrViewers.Get( ATTR_GAMEOBJECT )->viewer;
					gameObjectViewer->UpdateGUI();
				}
				else {
					// check if target is prefab
					Asset* asset = CAST_S( Asset*, _target );
					if ( asset != NULL && asset->GetAssetType() == AssetType::PREFAB ) {
						// send asset change event
						EventManager::Singleton()->EmitEvent( EventType::ASSET_RENAME, asset );
					}
				}
			}
			break;

		case EventType::COMPONENT_UPDATE:
		{
			Component* comp = CAST_S( Component*, eventData );
			if ( _target != NULL ) {
				if ( _target == comp->GetGameObject() ) {
					if ( comp->GetComponentType() == ComponentType::TRANSFORM ) {
						AttrViewer* viewer = _attrViewers.Get( ATTR_TRANSFORM )->viewer;
						viewer->UpdateGUI();
					}
					else if ( comp->GetComponentType() == ComponentType::CAMERA ) {
						AttrViewer* viewer = _attrViewers.Get( ATTR_CAMERA )->viewer;
						viewer->UpdateGUI();
					}
					else if ( comp->GetComponentType() == ComponentType::MESH_RENDERING ) {
						AttrViewer* viewer = _attrViewers.Get( ATTR_MESH_RENDERING )->viewer;
						viewer->UpdateGUI();
					}
					else if ( comp->GetComponentType() == ComponentType::MORPH_RENDERING ) {
						AttrViewer* viewer = _attrViewers.Get( ATTR_MORPH_RENDERING )->viewer;
						viewer->UpdateGUI();
					}
					else if ( comp->GetComponentType() == ComponentType::PARTICLE_EMITTER ) {
						AttrViewer* viewer = _attrViewers.Get( ATTR_PARTICLES_EMITTER )->viewer;
						viewer->UpdateGUI();
					}
					else if ( comp->GetComponentType() == ComponentType::SCRIPTER ) {
						AttrViewer* viewer = _attrViewers.Get( ATTR_SCRIPTER )->viewer;
						viewer->UpdateGUI();
					}
					else if ( comp->GetComponentType() == ComponentType::LIGHT ) {
						AttrViewer* viewer = _attrViewers.Get( ATTR_LIGHT )->viewer;
						viewer->UpdateGUI();
					}
					else if ( comp->GetComponentType() == ComponentType::RIGID_BODY ) {
						AttrViewer* viewer = _attrViewers.Get( ATTR_RIGID_BODY )->viewer;
						viewer->UpdateGUI();
					}
					else if ( comp->GetComponentType() == ComponentType::TEXT_3D ) {
						AttrViewer* viewer = _attrViewers.Get( ATTR_TEXT_3D )->viewer;
						viewer->UpdateGUI();
					}
				}
				else {
					// check if target is prefab
					Asset* asset = CAST_S( Asset*, _target );
					if ( asset != NULL && asset->GetAssetType() == AssetType::PREFAB ) {
						// send asset change event
						EventManager::Singleton()->EmitEvent( EventType::ASSET_UPDATE, asset );
					}
				}
			}
			break;
		}
			
		case EventType::ASSET_UPDATE:
			if ( _target == eventData )	{
				Asset* asset = CAST_S( Asset*, eventData );
				ASSERT( asset != NULL );

				cchar* attrViewerType = NULL;
				switch ( asset->GetAssetType() ) {
					case AssetType::MATERIAL:	attrViewerType = ATTR_MATERIAL;		break;
					case AssetType::MESH:		attrViewerType = ATTR_MESH;			break;
					case AssetType::MORPH:		attrViewerType = ATTR_MORPH;		break;
					case AssetType::TEXTURE:	attrViewerType = ATTR_TEXTURE;		break;
					case AssetType::SHADER:		attrViewerType = ATTR_SHADER;		break;
					case AssetType::SCENE:		attrViewerType = ATTR_SCENE;		break;
					case AssetType::SCRIPT:		attrViewerType = ATTR_SCRIPT;		break;
					case AssetType::PREFAB:		attrViewerType = ATTR_PREFAB;		break;
					case AssetType::FONT:		attrViewerType = ATTR_FONT;			break;
				}

				AttrViewer* attrViewer = _attrViewers.Get( attrViewerType )->viewer;
				attrViewer->UpdateGUI();
			}
			break;

		case EditorEventType::SCENE_SELECT:
		case EditorEventType::PREFAB_SELECT:
		case EditorEventType::ASSET_SELECT:
		{
			_target = eventData;
			_ClearPanel();

			if ( eventData == NULL ) {
				break;
			}

			Asset* asset = CAST_S( Asset*, eventData );
			ASSERT( asset != NULL );

			cchar* attrViewerType = NULL;
			switch ( asset->GetAssetType() ) {
				case AssetType::MATERIAL:	attrViewerType = ATTR_MATERIAL;		break;
				case AssetType::MESH:		attrViewerType = ATTR_MESH;			break;
				case AssetType::MORPH:		attrViewerType = ATTR_MORPH;		break;
				case AssetType::TEXTURE:	attrViewerType = ATTR_TEXTURE;		break;
				case AssetType::SHADER:		attrViewerType = ATTR_SHADER;		break;
				case AssetType::SCENE:		attrViewerType = ATTR_SCENE;		break;
				case AssetType::SCRIPT:		attrViewerType = ATTR_SCRIPT;		break;
				case AssetType::PREFAB:		attrViewerType = ATTR_PREFAB;		break;	
				case AssetType::FONT:		attrViewerType = ATTR_FONT;			break;	
			}

			_AttrViewer* atttrViewer = _attrViewers.Get( attrViewerType );
			atttrViewer->needsRefresh = TRUE;
			AttrViewer* viewer = atttrViewer->viewer;
			viewer->ChangeTarget( asset );
			viewer->UpdateGUI();

			break;
		}

		case EditorEventType::EDITOR_PROJ_SETTINGS:
		{
			_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_CYRED_PROJ );
			atttrViewer->needsRefresh = TRUE;
			AttrViewer* viewer = atttrViewer->viewer;
			viewer->ChangeTarget( NULL );
			viewer->UpdateGUI();
			break;
		}
	}
}


void Panel_Attributes::SetAttrViewer( cchar* typeName, AttrViewer* viewer )
{
	ASSERT( _isInitialized );

	_AttrViewer* attrViewer = new _AttrViewer();
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
	for ( int i = 0; i < _qtTree->topLevelItemCount(); ++i ) {
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
	// clear panel
	_ClearPanel();

	if ( gameObject != NULL ) {
		{
			_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_GAMEOBJECT );
			atttrViewer->needsRefresh = TRUE;
			AttrViewer* viewer = atttrViewer->viewer;
			viewer->ChangeTarget( gameObject );
			viewer->UpdateGUI();
		}
		{
			Component* comp = gameObject->GetComponent<Transform>();
			if ( comp != NULL )	{
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
				_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_MORPH_RENDERING );
				atttrViewer->needsRefresh = TRUE;
				AttrViewer* viewer = atttrViewer->viewer;
				viewer->ChangeTarget( comp );
				viewer->UpdateGUI();
			}
		}
		{
			Component* comp = gameObject->GetComponent<Scripter>();
			if ( comp != NULL )	{
				_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_SCRIPTER );
				atttrViewer->needsRefresh = TRUE;
				AttrViewer* viewer = atttrViewer->viewer;
				viewer->ChangeTarget( comp );
				viewer->UpdateGUI();
			}
		}
		{
			Component* comp = gameObject->GetComponent<RigidBody>();
			if ( comp != NULL )	{
				_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_RIGID_BODY );
				atttrViewer->needsRefresh = TRUE;
				AttrViewer* viewer = atttrViewer->viewer;
				viewer->ChangeTarget( comp );
				viewer->UpdateGUI();
			}
		}
		{
			Component* comp = gameObject->GetComponent<Text3D>();
			if ( comp != NULL )	{
				_AttrViewer* atttrViewer = _attrViewers.Get( ATTR_TEXT_3D );
				atttrViewer->needsRefresh = TRUE;
				AttrViewer* viewer = atttrViewer->viewer;
				viewer->ChangeTarget( comp );
				viewer->UpdateGUI();
			}
		}
	}
}
