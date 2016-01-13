// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AssetsPanel.h"
#include "CyredModule_File.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"
#include "CyredModule_Debug.h"
#include "CyredModule_Scene.h"
#include "../EditorSkin.h"
#include "../EditorSettings.h"

#include "QtWidgets\qtreewidget.h"
#include "QtCore\qdiriterator.h"
#include "QtCore\qfileinfo.h"
#include "QtWidgets\qmenu.h"
#include "QtGui\QDesktopServices"
#include "QtCore\qurl.h"
#include "QtWidgets\qlabel.h"


using namespace CYRED;


QTreeWidget* AssetsPanel::_firstTreeInstance = NULL;


class AssetsPanel::_QtTreeItem : public QTreeWidgetItem
{
public:
	Asset* asset;
};


AssetsPanel::AssetsPanel()
{
	this->setWindowTitle( PANEL_TITLE );
	this->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
	this->setAllowedAreas( Qt::DockWidgetArea::AllDockWidgetAreas );
	this->setMinimumSize( MIN_SIZE.x, MIN_SIZE.y );

	if ( _firstTreeInstance == NULL )
	{
		_qtTree = Memory::Alloc<QTreeWidget>();
		_qtTree->setHeaderHidden( true );
		_qtTree->setObjectName( EditorSkin::ASSET_TREE );
		_qtTree->setEditTriggers( QAbstractItemView::NoEditTriggers );

		_firstTreeInstance = _qtTree;
	}
	else
	{
		_qtTree = Memory::Alloc<QTreeWidget>( _firstTreeInstance );
	}

	this->setWidget( _qtTree );

	QObject::connect( _qtTree, &QTreeWidget::itemPressed,		this, &AssetsPanel::A_ItemClicked );
	QObject::connect( _qtTree, &QTreeWidget::itemDoubleClicked, this, &AssetsPanel::A_Item2xClicked );
	QObject::connect( _qtTree, &QTreeWidget::itemChanged,		this, &AssetsPanel::A_ItemRenamed );
}


void AssetsPanel::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	_LoadIcons();
	_CreateRightClickMenu();

	EventManager::Singleton()->RegisterListener( EventType::SCENE, this );
	EventManager::Singleton()->RegisterListener( EventType::ASSET, this );
}


void AssetsPanel::OnEvent( EventType eType, EventName eName, void* eSource )
{
	ASSERT( _isInitialized );

	switch ( eType )
	{
		case EventType::SCENE:
			_qtTree->setCurrentItem( NULL );
			break;

		case EventType::ASSET:
			switch ( eName )
			{
				case EventName::ASSET_CHANGED:
					{
						Asset* asset = CAST_S( Asset*, eSource );

						_QtTreeItem* treeItem = _FindTreeItem( asset );

						Bool isDuplicate = _IsFilePathDuplicate( asset );
						if ( isDuplicate )
						{
							DebugManager::Singleton()->Log( DEBUG_DUPLICATED_FILE_PATH );

							// change the name back
							asset->SetName( treeItem->text(0).toUtf8().constData() );
						}
						else
						{
							_SaveAssetToFile( asset, treeItem->text(0).toUtf8().constData() );
						
							if ( treeItem != NULL )
							{
								_qtTree->blockSignals( true );
								treeItem->setText( 0, asset->GetName() );
								_qtTree->blockSignals( false );
							}
						}
					}
					break;
			}
			break;
	}
}


void AssetsPanel::A_ItemClicked( QTreeWidgetItem* item, int column )
{
	Asset* asset = CAST_S( _QtTreeItem*, item )->asset;

	if ( asset != NULL )
	{
		EventManager::Singleton()->EmitEvent( EventType::ASSET,	
											  EventName::ASSET_SELECTED,
											  asset );
	}
	else
	{
		EventManager::Singleton()->EmitEvent( EventType::ASSET, 
											  EventName::UNKNOWN_SELECTED,
											  NULL );
	}
}


void AssetsPanel::A_Item2xClicked( QTreeWidgetItem* item, int column )
{
	Asset* asset = CAST_S( _QtTreeItem*, item )->asset;

	if ( asset != NULL && asset->GetAssetType() == AssetType::SCENE )
	{
		SceneManager::Singleton()->OpenScene( asset->GetUniqueID() );
	}
}


void AssetsPanel::A_ItemRenamed( QTreeWidgetItem* item, int column )
{
	Asset* asset = CAST_S( _QtTreeItem*, item )->asset;

	if ( asset != NULL )
	{
		String newName( item->text( 0 ).toUtf8().constData() );

		_qtTree->blockSignals( true );
		item->setText( 0, asset->GetName() );
		_qtTree->blockSignals( false );

		asset->SetName( newName.GetChar() );
	}
}


void AssetsPanel::A_RightClickMenu( const QPoint& pos )
{
	QTreeWidgetItem* item = _qtTree->itemAt( pos );

	if ( item != NULL )
	{
		_AddRightClickActions( item );

		_qtRightClickMenu->popup( _qtTree->mapToGlobal(pos) );
	}
}


void AssetsPanel::A_ReloadAsset()
{
	Asset* asset = CAST_S( _QtTreeItem*, _qtTree->currentItem() )->asset;

	ASSERT( asset != NULL );

	switch ( asset->GetAssetType() )
	{
		case AssetType::MATERIAL:
			{
				Material* material = CAST_S( Material*, asset );
				material->LoadFullFile();
			}
			break;

		case AssetType::MESH:
			{
				Mesh* mesh = CAST_S( Mesh*, asset );
				mesh->LoadFullFile();
			}
			break;

		case AssetType::TEXTURE:
			{
				Texture* texture = CAST_S( Texture*, asset );
				texture->LoadFullFile();
			}
			break;

		case AssetType::SHADER:
			{
				Shader* shader = CAST_S( Shader*, asset );
				shader->LoadFullFile();
			}
			break;
	}
}


void AssetsPanel::A_OpenOnDisk()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString filePath = item->whatsThis( 1 );
	QDesktopServices::openUrl( QUrl::fromLocalFile( filePath ) );
}


void AssetsPanel::A_ShowOnDisk()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString filePath = item->whatsThis( 1 );
	QFileInfo fileInfo( filePath );
	QDesktopServices::openUrl( QUrl::fromLocalFile( fileInfo.dir().path() ) );
}


void AssetsPanel::A_Rename()
{
	QTreeWidgetItem* item = _qtTree->currentItem();
	_qtTree->editItem( item );
}


void AssetsPanel::A_OpenScene()
{
	Asset* asset = CAST_S( _QtTreeItem*, _qtTree->currentItem() )->asset;

	ASSERT( asset->GetAssetType() == AssetType::SCENE );

	SceneManager::Singleton()->OpenScene( asset->GetUniqueID() );
}


void AssetsPanel::A_LoadScene()
{
	Asset* asset = CAST_S( _QtTreeItem*, _qtTree->currentItem() )->asset;

	ASSERT( asset->GetAssetType() == AssetType::SCENE );

	SceneManager::Singleton()->LoadScene( asset->GetUniqueID() );
}


void AssetsPanel::ReloadAllAssets()
{
	ASSERT( _isInitialized );

	// TODO
	_ParseDirectory( EditorSettings::DIR_PATH_ASSETS, _qtTree->invisibleRootItem() );
}


void AssetsPanel::_LoadIcons()
{
	ASSERT( _isInitialized );

	QDirIterator dirIterator( EditorSettings::DIR_PATH_ICONS_ASSETS, QDir::Files );

	while (dirIterator.hasNext())
	{
		dirIterator.next();
		QFileInfo& fileInfo = dirIterator.fileInfo();

		_icons.Set( fileInfo.completeBaseName().toUtf8().data(), 
					QIcon( fileInfo.filePath().toUtf8().data() ) );
	}
}


void AssetsPanel::_ParseDirectory( const Char* dirPath, QTreeWidgetItem* parentItem )
{
	ASSERT( _isInitialized );

	QDirIterator dirIterator( dirPath, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot );

	while ( dirIterator.hasNext() )
	{
		dirIterator.next();
		QFileInfo& fileInfo = dirIterator.fileInfo();

		if ( fileInfo.isFile() )
		{
			QString fileFormat = fileInfo.suffix();
			fileFormat.prepend( "." );
			QString fileName = fileInfo.completeBaseName();
			QIcon icon;
			Bool isUnknown = FALSE;
			Asset* asset = NULL;
			StatusAssetAdd statusAdd = StatusAssetAdd::SUCCESS;


			if ( fileFormat.compare( FileManager::FILE_FORMAT_SCENE ) == 0 )
			{
				icon = _icons.Get( ICON_SCENE );

				Scene* scene = Memory::Alloc<Scene>();
				scene->SetEmitEvents( FALSE );
				scene->SetName( fileName.toUtf8().constData() );
				scene->SetDirPath( dirPath );
				scene->LoadUniqueID();
				scene->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddScene( scene );
				asset = scene; // will be checked later
			}
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_MATERIAL ) == 0 )
			{
				icon = _icons.Get( ICON_MATERIAL );

				Material* material = Memory::Alloc<Material>();
				material->SetEmitEvents( FALSE );
				material->SetName( fileName.toUtf8().constData() );
				material->SetDirPath( dirPath );
				material->LoadUniqueID();
				material->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddMaterial( material );
				asset = material; // will be checked later
			}
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_MESH ) == 0 )
			{
				icon = _icons.Get( ICON_MESH );
				
				Mesh* mesh = Memory::Alloc<Mesh>();
				mesh->SetEmitEvents( FALSE );
				mesh->SetName( fileName.toUtf8().constData() );
				mesh->SetDirPath( dirPath );
				mesh->LoadUniqueID();
				mesh->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddMesh( mesh );
				asset = mesh; // will be checked later
			}
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_SHADER ) == 0 )
			{
				icon = _icons.Get( ICON_SHADER );
				
				Shader* shader = Memory::Alloc<Shader>();
				shader->SetEmitEvents( FALSE );
				shader->SetName( fileName.toUtf8().constData() );
				shader->SetDirPath( dirPath );
				shader->LoadUniqueID();
				shader->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddShader( shader );
				asset = shader; // will be checked later
			}
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_TEXTURE ) == 0 )
			{
				icon = _icons.Get( ICON_TEXTURE );
				
				Texture* texture = Memory::Alloc<Texture>();
				texture->SetEmitEvents( FALSE );
				texture->SetName( fileName.toUtf8().constData() );
				texture->SetDirPath( dirPath );
				texture->LoadUniqueID();
				texture->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddTexture( texture );
				asset = texture; // will be checked later
			}
			else /*unknown file*/
			{
				isUnknown = TRUE;
				fileFormat = NULL;
				fileName = NULL;
			}

			if ( !isUnknown )
			{
				switch ( statusAdd )
				{
					case StatusAssetAdd::SUCCESS:
						if ( asset != NULL )
						{
							asset->SetIsTemporary( FALSE );
						}
						break;

					case StatusAssetAdd::FAIL_INVALID_ID:
					{
						Char warning[ MAX_SIZE_CUSTOM_STRING ];
						CUSTOM_STRING( warning, DEBUG_INVALID_UID,
										fileName.toUtf8().constData() );
						DebugManager::Singleton()->Log( warning );

						asset->SetUniqueID( Random::GenerateUniqueID().GetChar() );
						break;
					}

					case StatusAssetAdd::FAIL_EXISTING:
					{
						// if temporary, reload the temporary one and delete this one
						Asset* other = NULL;
						const Char* uid = asset->GetUniqueID();

						switch ( asset->GetAssetType() )
						{
							case AssetType::MATERIAL:
								other = AssetManager::Singleton()->GetMaterial( uid );
								break;

							case AssetType::MESH:
								other = AssetManager::Singleton()->GetMesh( uid );
								break;

							case AssetType::SHADER:
								other = AssetManager::Singleton()->GetShader( uid );
								break;

							case AssetType::TEXTURE:
								other = AssetManager::Singleton()->GetTexture( uid );
								break;

							case AssetType::SCENE:
								other = AssetManager::Singleton()->GetScene( uid );
								break;
						}

						if ( other->IsTemporary() )
						{
							other->SetEmitEvents( FALSE );
							other->SetName( fileName.toUtf8().constData() );
							other->SetDirPath( dirPath );
							other->SetIsTemporary( FALSE );
							other->SetEmitEvents( TRUE );

							Memory::Free( asset );
							asset = other;
						}
						else
						{
							Char warning[MAX_SIZE_CUSTOM_STRING];
							CUSTOM_STRING( warning, DEBUG_EXISTING_ASSET,
										   fileName.toUtf8().constData() );
							DebugManager::Singleton()->Log( warning );

							Memory::Free( asset );
							asset = NULL;
						}
						break;
					}
				}

				if ( asset != NULL && asset->GetAssetType() != AssetType::SCENE )
				{
					asset->SetEmitEvents( FALSE );
					asset->LoadFullFile();
					asset->SetEmitEvents( TRUE );
				}
			}

			_QtTreeItem* treeItem = Memory::Alloc<_QtTreeItem>();
			treeItem->asset = asset;
			treeItem->setWhatsThis( 0, fileFormat );
			treeItem->setWhatsThis( 1, fileInfo.filePath() ); 
			treeItem->setFlags( Qt::ItemIsSelectable | 
								Qt::ItemIsEnabled | 
								Qt::ItemIsEditable );
			treeItem->setText( 0, fileName );
			treeItem->setIcon( 0, icon );

			parentItem->addChild( treeItem );

			if ( isUnknown )
			{
				QLabel* fileLabel = Memory::Alloc<QLabel>( fileInfo.fileName() );
				fileLabel->setObjectName( EditorSkin::ASSET_UNKNOWN );
				_qtTree->setItemWidget( treeItem, 0, fileLabel );
			}
		}

		if ( fileInfo.isDir() )
		{
			_QtTreeItem* treeItem = Memory::Alloc<_QtTreeItem>();
			treeItem->setText( 0, fileInfo.completeBaseName() );
			treeItem->setWhatsThis( 0, TYPE_FOLDER );  // we use this field to store data
			treeItem->setWhatsThis( 1, fileInfo.filePath() ); 
			treeItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
			treeItem->setIcon( 0, _icons.Get( ICON_FOLDER ) );

			parentItem->addChild( treeItem );

			QString newDirPath = fileInfo.filePath();
			newDirPath.append( "/" );

			_ParseDirectory( newDirPath.toUtf8().constData(), treeItem );
		}
	}
}


void AssetsPanel::_CreateRightClickMenu()
{
	ASSERT( _isInitialized );

	_qtRightClickMenu = Memory::Alloc<QMenu>( this );

	_qtTree->setContextMenuPolicy( Qt::CustomContextMenu );
	QObject::connect( _qtTree, &QWidget::customContextMenuRequested, 
					  this, &AssetsPanel::A_RightClickMenu );
}


void AssetsPanel::_AddRightClickActions( QTreeWidgetItem* item )
{
	ASSERT( _isInitialized );

	_qtRightClickMenu->clear();

	Asset* asset = CAST_S( _QtTreeItem*, _qtTree->currentItem() )->asset;

	if ( asset != NULL )
	{
		if ( asset->GetAssetType() == AssetType::SCENE )
		{
			QAction* actionOpenScene	= _qtRightClickMenu->addAction( MENU_OPEN_SCENE );
			QAction* actionLoadScene	= _qtRightClickMenu->addAction( MENU_LOAD_SCENE );

			QObject::connect( actionOpenScene,	&QAction::triggered, this, &AssetsPanel::A_OpenScene );
			QObject::connect( actionLoadScene,	&QAction::triggered, this, &AssetsPanel::A_LoadScene );
		}

		QAction* actionReload = _qtRightClickMenu->addAction( MENU_RELOAD );

		QObject::connect( actionReload,	&QAction::triggered, this, &AssetsPanel::A_ReloadAsset );
	}

	QAction* actionOpenOnDisk	= _qtRightClickMenu->addAction( MENU_OPEN_DISK );
	QAction* actionShowOnDisk	= _qtRightClickMenu->addAction( MENU_SHOW_DISK );
	QAction* actionRename		= _qtRightClickMenu->addAction( MENU_RENAME );

	QObject::connect( actionOpenOnDisk,	&QAction::triggered, this, &AssetsPanel::A_OpenOnDisk );
	QObject::connect( actionShowOnDisk,	&QAction::triggered, this, &AssetsPanel::A_ShowOnDisk );
	QObject::connect( actionRename,		&QAction::triggered, this, &AssetsPanel::A_Rename );
}


void AssetsPanel::_SaveAssetToFile( Asset* asset, const Char* oldName )
{
	ASSERT( asset != NULL );

	// first try to write new file, then delete old one
	// this way, if writing fails, you still got old file

	Char filePath[ MAX_SIZE_CUSTOM_STRING ];
	const Char* fileFormat = NULL;
	String data;

	switch ( asset->GetAssetType() )
	{
		case AssetType::TEXTURE:
		{
			fileFormat = FileManager::FILE_FORMAT_TEXTURE;
			Texture* texture = CAST_S( Texture*, asset );
			data = FileManager::Singleton()->Serialize<Texture>( texture );
			break;
		}

		case AssetType::SHADER:
		{
			fileFormat = FileManager::FILE_FORMAT_SHADER;
			Shader* shader = CAST_S( Shader*, asset );
			data = FileManager::Singleton()->Serialize<Shader>( shader );
			break;
		}

		case AssetType::MATERIAL:
		{
			fileFormat = FileManager::FILE_FORMAT_MATERIAL;
			Material* material = CAST_S( Material*, asset );
			data = FileManager::Singleton()->Serialize<Material>( material );
			break;
		}
		
		default:
			return;
	}

	CUSTOM_STRING( filePath, "%s%s%s", asset->GetDirPath(), asset->GetName(), fileFormat );

	Bool success = FileManager::Singleton()->WriteFile( filePath, data.GetChar() );

	// now delete old file
	if ( success && String(oldName) != asset->GetName() )
	{
		CUSTOM_STRING( filePath, "%s%s%s", asset->GetDirPath(), oldName, fileFormat );
		FileManager::Singleton()->DeleteFile( filePath );
	}
}


Bool AssetsPanel::_IsFilePathDuplicate( Asset* asset )
{
	AssetManager* assetMgr = AssetManager::Singleton();
	QString name( asset->GetName() );
	QString dirPath( asset->GetDirPath() );

	switch ( asset->GetAssetType() )
	{
		case AssetType::TEXTURE:
		{
			for ( UInt i = 0; i < assetMgr->GetTextureCount(); ++i )
			{
				Texture* other = assetMgr->GetTextureAt( i );
				if ( asset != other && 
					 name.compare( other->GetName(), Qt::CaseInsensitive ) == 0 && 
					 dirPath.compare( other->GetDirPath(), Qt::CaseInsensitive ) == 0 )
				{
					return TRUE;
				}
			}
			break;
		}

		case AssetType::SHADER:
		{
			for ( UInt i = 0; i < assetMgr->GetShaderCount(); ++i )
			{
				Shader* other = assetMgr->GetShaderAt( i );
				if ( asset != other && 
					 name.compare( other->GetName(), Qt::CaseInsensitive ) == 0 && 
					 dirPath.compare( other->GetDirPath(), Qt::CaseInsensitive ) == 0 )
				{
					return TRUE;
				}
			}
			break;
		}

		case AssetType::MATERIAL:
		{
			for ( UInt i = 0; i < assetMgr->GetMaterialCount(); ++i )
			{
				Material* other = assetMgr->GetMaterialAt( i );
				if ( asset != other && 
					 name.compare( other->GetName(), Qt::CaseInsensitive ) == 0 && 
					 dirPath.compare( other->GetDirPath(), Qt::CaseInsensitive ) == 0 )
				{
					return TRUE;
				}
			}
			break;
		}
	}

	return FALSE;
}


AssetsPanel::_QtTreeItem* AssetsPanel::_FindTreeItem( Asset* asset )
{
	QTreeWidgetItemIterator it( _qtTree );
	while ( *it != NULL ) 
	{
		_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, *it );

		if ( asset == treeItem->asset )
		{
			return treeItem;
		}

		++it;
	}

	return NULL;
}


