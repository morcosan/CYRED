// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AttributePanel.h"
#include "../AttrViewers/AttrViewer.h"
#include "CyredModule_Render.h"
#include "CyredModule_Asset.h"
#include "../Settings/EditorSkin.h"

#include "..\AttrViewers\AttrViewer_GameObject.h"
#include "..\AttrViewers\AttrViewer_Transform.h"
#include "..\AttrViewers\AttrViewer_Camera.h"
#include "..\AttrViewers\AttrViewer_ParticleEmitter.h"
#include "..\AttrViewers\AttrViewer_MeshRendering.h"
#include "..\AttrViewers\AttrViewer_MorphRendering.h"
#include "..\AttrViewers\AttrViewer_Material.h"
#include "..\AttrViewers\AttrViewer_Mesh.h"
#include "..\AttrViewers\AttrViewer_Morph.h"
#include "..\AttrViewers\AttrViewer_Shader.h"
#include "..\AttrViewers\AttrViewer_Texture.h"
#include "..\AttrViewers\AttrViewer_Scene.h"
#include "..\AttrViewers\AttrViewer_CyredProj.h"

#include "QtWidgets\qboxlayout.h"
#include "QtWidgets\qtreewidget.h"
#include "QtWidgets\qpushbutton.h"


using namespace CYRED;



AttributePanel::AttributePanel()
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

	// TODO
	_qtCompButton = Memory::Alloc<QPushButton>( this );
	_qtCompButton->setMinimumSize( 160, 25 );
	_qtCompButton->setMaximumSize( 160, 25 );
	_qtCompButton->setText( ADD_COMPONENT_BUTTON );
	_qtCompButton->hide();
	_qtCompButton->setObjectName( EditorSkin::ATTR_BUTTON );
	//QObject::connect( _qtCompButton, &QPushButton::clicked, this, &AttributePanel::A_AddComponent );

	QVBoxLayout* vLayout = Memory::Alloc<QVBoxLayout>();
	vLayout->setSpacing( 15 );
	vLayout->setContentsMargins( 0, 0, 0, 10 );
	vLayout->addWidget( _qtTree );
	vLayout->addWidget( _qtCompButton, 0, Qt::AlignHCenter );

	QWidget* layoutWidget = Memory::Alloc<QWidget>();
	layoutWidget->setLayout( vLayout );
	layoutWidget->setObjectName( EditorSkin::PANEL_LAYOUT );

	this->setWidget( layoutWidget );

	EventManager::Singleton()->RegisterListener( EventType::ALL, this );
}


AttributePanel::~AttributePanel()
{
	Iterator<String, AttrViewer*> iter = _attrViewers.GetIterator();

	while ( iter.HasNext() )
	{
		Memory::Free( iter.GetValue() );
		iter.Next();
	}
}


void AttributePanel::Initialize()
{
	SetAttrViewer( ATTR_GAMEOBJECT,			Memory::Alloc<AttrViewer_GameObject>() );
	SetAttrViewer( ATTR_TRANSFORM,			Memory::Alloc<AttrViewer_Transform>() );
	SetAttrViewer( ATTR_CAMERA,				Memory::Alloc<AttrViewer_Camera>() );
	SetAttrViewer( ATTR_PARTICLES_EMITTER,	Memory::Alloc<AttrViewer_ParticleEmitter>() );
	SetAttrViewer( ATTR_MESH_RENDERING,		Memory::Alloc<AttrViewer_MeshRendering>() );
	SetAttrViewer( ATTR_MORPH_RENDERING,	Memory::Alloc<AttrViewer_MorphRendering>() );
	SetAttrViewer( ATTR_MATERIAL,			Memory::Alloc<AttrViewer_Material>() );
	SetAttrViewer( ATTR_MESH,				Memory::Alloc<AttrViewer_Mesh>() );
	SetAttrViewer( ATTR_MORPH,				Memory::Alloc<AttrViewer_Morph>() );
	SetAttrViewer( ATTR_SHADER,				Memory::Alloc<AttrViewer_Shader>() );
	SetAttrViewer( ATTR_TEXTURE,			Memory::Alloc<AttrViewer_Texture>() );
	SetAttrViewer( ATTR_SCENE,				Memory::Alloc<AttrViewer_Scene>() );
	SetAttrViewer( ATTR_CYRED_PROJ,			Memory::Alloc<AttrViewer_CyredProj>() );

	_Clear();
}


void AttributePanel::Update()
{
	if ( _needsRefresh )
	{
		_needsRefresh = FALSE;

		// WARNING: this will cause huge lag in editor
		_qtTree->setStyleSheet( _qtTree->styleSheet() );
	}
}


void AttributePanel::OnEvent( EventType eType, EventName eName, void* eSource )
{
	switch ( eType )
	{
		case EventType::SCENE:
		{
			switch ( eName )
			{
				case EventName::GAMEOBJECT_SELECTED:
				{
					_target = eSource;

					// clear panel
					_Clear();

					// prepare to add new 
					Node* node = CAST_S( Node*, eSource );
					GameObject* selectedGO = CAST_D( GameObject*, node );

					if ( selectedGO != NULL )
					{
						ASSERT( _attrViewers.Has( ATTR_GAMEOBJECT ) );
						AttrViewer* gameObjectViewer = _attrViewers.Get( ATTR_GAMEOBJECT );
						gameObjectViewer->ChangeTarget( selectedGO );
						gameObjectViewer->UpdateGUI();

						{
							COMP::Component* comp = selectedGO->GetComponent<COMP::Transform>();
							if ( comp != NULL )
							{
								ASSERT( _attrViewers.Has( ATTR_TRANSFORM ) );
								AttrViewer* viewer = _attrViewers.Get( ATTR_TRANSFORM );
								viewer->ChangeTarget( comp );
								viewer->UpdateGUI();
							}
						}
						{
							COMP::Component* comp = selectedGO->GetComponent<COMP::Camera>();
							if ( comp != NULL )
							{
								ASSERT( _attrViewers.Has( ATTR_CAMERA ) );
								AttrViewer* viewer = _attrViewers.Get( ATTR_CAMERA );
								viewer->ChangeTarget( comp );
								viewer->UpdateGUI();
							}
						}
						{
							COMP::Component* comp = selectedGO->GetComponent<COMP::ParticleEmitter>();
							if ( comp != NULL )
							{
								ASSERT( _attrViewers.Has( ATTR_PARTICLES_EMITTER ) );
								AttrViewer* viewer = _attrViewers.Get( ATTR_PARTICLES_EMITTER );
								viewer->ChangeTarget( comp );
								viewer->UpdateGUI();
							}
						}
						{
							COMP::Component* comp = selectedGO->GetComponent<COMP::MeshRendering>();
							if ( comp != NULL )
							{
								ASSERT( _attrViewers.Has( ATTR_MESH_RENDERING ) );
								AttrViewer* viewer = _attrViewers.Get( ATTR_MESH_RENDERING );
								viewer->ChangeTarget( comp );
								viewer->UpdateGUI();
							}
						}
						{
							COMP::Component* comp = selectedGO->GetComponent<COMP::MorphRendering>();
							if ( comp != NULL )
							{
								ASSERT( _attrViewers.Has( ATTR_MORPH_RENDERING ) );
								AttrViewer* viewer = _attrViewers.Get( ATTR_MORPH_RENDERING );
								viewer->ChangeTarget( comp );
								viewer->UpdateGUI();
							}
						}

						//_qtCompButton->show();
					}
					break;
				}

				case EventName::SCENE_SELECTED:
				{
					// clear panel
					_Clear();

					_target = NULL;

					break;
				}

				case EventName::GAMEOBJECT_RENAMED:
				{
					if ( _target != NULL && _target == eSource )
					{
						Node* node = CAST_S( Node*, eSource );
						GameObject* selectedGO = CAST_D( GameObject*, node );

						if ( selectedGO != NULL )
						{
							ASSERT( _attrViewers.Has( ATTR_GAMEOBJECT ) );
							AttrViewer* gameObjectViewer = _attrViewers.Get( ATTR_GAMEOBJECT );
							gameObjectViewer->UpdateGUI();
						}
					}
					break;
				}
			}

			break;
		}

		case EventType::COMPONENT:
		{
			if ( _target != NULL )
			{
				COMP::Component* comp = CAST_S( COMP::Component*, eSource );
				if ( _target == comp->GetGameObject() )
				{
					if ( eName == EventName::TRANSFORM_CHANGED )
					{
						ASSERT( _attrViewers.Has( ATTR_TRANSFORM ) );
						AttrViewer* viewer = _attrViewers.Get( ATTR_TRANSFORM );
						viewer->UpdateGUI();
					}
					if ( eName == EventName::CAMERA_CHANGED )
					{
						ASSERT( _attrViewers.Has( ATTR_CAMERA ) );
						AttrViewer* viewer = _attrViewers.Get( ATTR_CAMERA );
						viewer->UpdateGUI();
					}
					if ( eName == EventName::MESH_RENDERING_CHANGED )
					{
						ASSERT( _attrViewers.Has( ATTR_MESH_RENDERING ) );
						AttrViewer* viewer = _attrViewers.Get( ATTR_MESH_RENDERING );
						viewer->UpdateGUI();
					}
					if ( eName == EventName::MORPH_RENDERING_CHANGED )
					{
						ASSERT( _attrViewers.Has( ATTR_MORPH_RENDERING ) );
						AttrViewer* viewer = _attrViewers.Get( ATTR_MORPH_RENDERING );
						viewer->UpdateGUI();
					}
					if ( eName == EventName::PARTICLE_EMITTER_CHANGED )
					{
						ASSERT( _attrViewers.Has( ATTR_PARTICLES_EMITTER ) );
						AttrViewer* viewer = _attrViewers.Get( ATTR_PARTICLES_EMITTER );
						viewer->UpdateGUI();
					}
				}
			}
			break;
		}

		case EventType::ASSET:
		{
			switch ( eName )
			{
				case EventName::ASSET_CHANGED:
				{
					if ( _target == eSource )
					{
						Asset* asset = CAST_S( Asset*, eSource );
						ASSERT( asset != NULL );

						const Char* attrViewerType = NULL;

						switch ( asset->GetAssetType() )
						{
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
						}

						ASSERT( _attrViewers.Has( attrViewerType ) );
						AttrViewer* attrViewer = _attrViewers.Get( attrViewerType );
						attrViewer->UpdateGUI();
					}
					break;
				}

				case EventName::ASSET_SELECTED:
				{
					_target = eSource;
					_Clear();

					if ( eSource == NULL )
					{
						break;
					}

					Asset* asset = CAST_S( Asset*, eSource );
					ASSERT( asset != NULL );

					const Char* attrViewerType = NULL;

					switch ( asset->GetAssetType() )
					{
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
					}

					ASSERT( _attrViewers.Has( attrViewerType ) );
					AttrViewer* attrViewer = _attrViewers.Get( attrViewerType );
					attrViewer->ChangeTarget( asset );
					attrViewer->UpdateGUI();

					break;
				}
			}
			break;
		}

		case EventType::CUSTOM:
		{
			if ( eName == EventName::EDITOR_PROJ_SETTINGS )
			{
				ASSERT( _attrViewers.Has( ATTR_CYRED_PROJ ) );
				AttrViewer* attrViewer = _attrViewers.Get( ATTR_CYRED_PROJ );
				attrViewer->ChangeTarget( NULL );
				attrViewer->UpdateGUI();
			}
			break;
		}
	}
}


void AttributePanel::SetAttrViewer( const Char* typeName, AttrViewer* viewer )
{
	_attrViewers.Set( typeName, viewer ); 
	viewer->Initialize( this, _qtTree );
}


void AttributePanel::RefreshPanel()
{
	_needsRefresh = TRUE;
}


void AttributePanel::_Clear()
{
	for ( Int i = 0; i < _qtTree->topLevelItemCount(); ++i )
	{
		QTreeWidgetItem* item = _qtTree->topLevelItem( i );
		item->setHidden( TRUE );
	}

	_qtCompButton->hide();
}

