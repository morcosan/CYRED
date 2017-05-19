// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "Panel_Assets.h"
#include "CyredModule_File.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"
#include "CyredModule_Debug.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Script.h"

#include "../Settings/EditorSkin.h"
#include "../Settings/ProjectSettings.h"
#include "../Settings/EditorSettings.h"
#include "../../Utils/EditorUtils.h"
#include "../../Utils/CustomTreeItem.h"
#include "../Menus/Menu_Asset.h"
#include "../Menus/Menu_AssetFolder.h"
#include "../Menus/Menu_AssetUnknown.h"

#include "QtWidgets\qboxlayout.h"
#include "QtWidgets\qtreewidget.h"
#include "QtCore\qdiriterator.h"
#include "QtCore\qfileinfo.h"
#include "QtWidgets\qmenu.h"
#include "QtCore\qurl.h"
#include "QtWidgets\qlabel.h"
#include "QtCore\qfilesystemwatcher.h"
#include "QtWidgets\qpushbutton.h"


using namespace CYRED;


Panel_Assets::Panel_Assets()
{
	this->setWindowTitle( PANEL_TITLE );
	this->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
	this->setAllowedAreas( Qt::DockWidgetArea::AllDockWidgetAreas );
	this->setMinimumSize( MIN_SIZE.x, MIN_SIZE.y );
	this->setMaximumSize( MAX_SIZE.x, MAX_SIZE.y );

	QPushButton* reloadBtn = Memory::Alloc<QPushButton>( this );
	reloadBtn->setText( BUTTON_RELOAD );
	reloadBtn->setObjectName( EditorSkin::ASSET_BUTTON );
	QObject::connect( reloadBtn, &QPushButton::clicked,	this, &Panel_Assets::A_ReloadAll );

	QHBoxLayout* topBarLayout = Memory::Alloc<QHBoxLayout>();
	topBarLayout->setSpacing( 0 );
	topBarLayout->setContentsMargins( 0, 2, 0, 1 );
	topBarLayout->addWidget( reloadBtn );

	QWidget* topBarWidget = Memory::Alloc<QWidget>();
	topBarWidget->setLayout( topBarLayout );

	_qtTree = Memory::Alloc<QTreeWidget>();
	_qtTree->setHeaderHidden( true );
	_qtTree->setObjectName( EditorSkin::ASSET_TREE );
	_qtTree->setEditTriggers( QAbstractItemView::NoEditTriggers );

	QVBoxLayout* vLayout = Memory::Alloc<QVBoxLayout>();
	vLayout->setSpacing( 0 );
	vLayout->setContentsMargins( 0, 0, 0, 0 );
	vLayout->addWidget( topBarWidget );
	vLayout->addWidget( _qtTree );

	QWidget* layoutWidget = Memory::Alloc<QWidget>();
	layoutWidget->setLayout( vLayout );

	this->setWidget( layoutWidget );

	QObject::connect( _qtTree, &QTreeWidget::itemPressed,		this, &Panel_Assets::A_ItemClicked );
	QObject::connect( _qtTree, &QTreeWidget::itemDoubleClicked, this, &Panel_Assets::A_Item2xClicked );
	QObject::connect( _qtTree, &QTreeWidget::itemChanged,		this, &Panel_Assets::A_ItemRenamed );

	_qtFileWatcher = Memory::Alloc<QFileSystemWatcher>();
	_qtFileWatcher->addPath( ProjectSettings::dirPathAssets.GetChar() );

	QObject::connect( _qtFileWatcher, &QFileSystemWatcher::directoryChanged,	
					  this, &Panel_Assets::A_DirChanged );
}


void Panel_Assets::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	_LoadIcons();
	_CreateRightClickMenu();

	// register events
	EventManager::Singleton()->RegisterListener( EventType::ALL, this );
}


void Panel_Assets::Finalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::ALL, this );
}


void Panel_Assets::OnEvent( EventType eType, void* eData )
{
	ASSERT( _isInitialized );

	switch ( eType ) {
		case EventType::SELECT_SCENE:
		case EventType::SELECT_PREFAB:
		case EventType::SELECT_GAMEOBJECT:
			_qtTree->setCurrentItem( NULL );
			break;

		case EventType::CHANGE_ASSET:
		{
			Asset* asset = CAST_S( Asset*, eData );

			// do not save temporary assets
			if ( asset != NULL && !asset->IsTemporary() ) {
				// get tree item
				CustomTreeItem* treeItem = _FindTreeItem( asset );

				// if tree item does not exist, create one
				if ( treeItem == NULL ) {
					// find folder tree item
					CustomTreeItem* parentItem = _FindFolderItem( asset->GetDirPath() );
					if ( parentItem == NULL ) {
						// if not found, it cannot save
						FiniteString errorMsg( DEBUG_FOLDER_NOT_FOUND, asset->GetDirPath() );
						DebugManager::Singleton()->Error( errorMsg.GetChar() );
						return;
					}

					// get icon
					const char* icon = NULL;
					switch ( asset->GetAssetType() ) {
						case AssetType::MATERIAL:	icon = EditorUtils::ICON_MATERIAL;	break;
						case AssetType::MESH:		icon = EditorUtils::ICON_MESH;		break;
						case AssetType::MORPH:		icon = EditorUtils::ICON_MORPH;		break;
						case AssetType::SCENE:		icon = EditorUtils::ICON_SCENE;		break;
						case AssetType::SCRIPT:		icon = EditorUtils::ICON_SCRIPT;	break;
						case AssetType::SHADER:		icon = EditorUtils::ICON_SHADER;	break;
						case AssetType::TEXTURE:	icon = EditorUtils::ICON_TEXTURE;	break;
						case AssetType::PREFAB:		icon = EditorUtils::ICON_PREFAB;	break;
					}
					// create tree item
					treeItem = AddAssetToTree( asset, parentItem, icon );
				}

				// check for duplicated path
				bool isDuplicate = EditorUtils::IsFilePathDuplicate( asset );
				if ( isDuplicate ) {
					DebugManager::Singleton()->Log( DEBUG_DUPLICATED_FILE_PATH );

					// change the name back
					if ( treeItem->text(0).compare( asset->GetName() ) != 0 ) {
						asset->SetName( treeItem->text(0).toUtf8().constData() );
					}
				}
				else {
					// write file
					_SaveAssetToFile( asset, treeItem->text(0).toUtf8().constData() );

					_qtTree->blockSignals( true );
					{
						// update item name
						treeItem->setText( 0, asset->GetName() );
						// change file path
						FiniteString filePath( "%s%s%s", asset->GetDirPath(), asset->GetName(), 
											   asset->GetExtension() );
						treeItem->setWhatsThis( 1, filePath.GetChar() );
					}
					_qtTree->blockSignals( false );
				}
			}
			break;
		}
	}
}


void Panel_Assets::A_ItemClicked( QTreeWidgetItem* item, int column )
{
	Asset* asset = CAST_S( CustomTreeItem*, item )->asset;

	EventManager::Singleton()->EmitEvent( EventType::SELECT_ASSET, asset );
}


void Panel_Assets::A_Item2xClicked( QTreeWidgetItem* item, int column )
{
	Asset* asset = CAST_S( CustomTreeItem*, item )->asset;

	if ( asset != NULL ) {
		if ( asset->GetAssetType() == AssetType::SCENE ) {
			SceneManager::Singleton()->OpenScene( asset->GetUniqueID() );
		}
		else if ( asset->GetAssetType() == AssetType::PREFAB ) {
			_menuAsset->A_EditPrefab();
		}
	}
}


void Panel_Assets::A_ItemRenamed( QTreeWidgetItem* item, int column )
{
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, item );

	Asset* asset = treeItem->asset;

	if ( asset != NULL ) {
		FiniteString newName( item->text(0).toUtf8().constData() );

		_qtTree->blockSignals( true );
		{
			// set old name since will be changed after
			item->setText( 0, asset->GetName() );
			// change file path
			FiniteString filePath( "%s%s%s", asset->GetDirPath(), newName.GetChar(), 
								   asset->GetExtension() );
			treeItem->setWhatsThis( 1, filePath.GetChar() );
		}
		_qtTree->blockSignals( false );

		// change name
		asset->SetName( newName.GetChar() );
	}
	else {
		QFileInfo fileInfo( item->whatsThis(1) );

		if ( fileInfo.exists() ) {
			QDir dir;
			QString newFolder = dir.relativeFilePath( fileInfo.absolutePath() );
			newFolder.append( "/" ).append( item->text(0) );
			
			bool success = dir.rename( item->whatsThis(1), newFolder );
			if ( success ) {
				treeItem->setWhatsThis( 1, newFolder );
			}
		}
	}
}


void Panel_Assets::A_RightClickMenu( const QPoint& pos )
{
	// get tree item
	CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, _qtTree->itemAt( pos ) );
	_qtTree->setCurrentItem( treeItem );

	if ( treeItem != NULL ) {
		if ( treeItem->asset != NULL ) {
			// open asset menu
			_menuAsset->Open( _qtTree->mapToGlobal(pos) );
		}
		else if ( treeItem == NULL || treeItem->whatsThis(0).compare( TYPE_FOLDER ) == 0 ) {
			// open folder menu
			_menuAssetFolder->Open( _qtTree->mapToGlobal(pos) );
		}
		else {
			// open unknown menu
			_menuAssetUnknown->Open( _qtTree->mapToGlobal(pos) );
		}
	}
}


void Panel_Assets::A_Create_Folder()
{
	QTreeWidgetItem* parentItem = _qtTree->currentItem();

	QString dirPath = (parentItem != NULL) ? parentItem->whatsThis( 1 ).append( "/" ) 
										   : ProjectSettings::dirPathAssets.GetChar();

	// generate name
	// find new name
	int nextIndex = -1;
	FiniteString folderName;
	FiniteString folderPath;

	do {
		nextIndex++;
		folderName.Set( "%s%d", EditorUtils::NAME_FOLDER, nextIndex );
		folderPath.Set( "%s%s", dirPath.toUtf8().constData(), folderName.GetChar() );

		// check if exists
		QDir dir( folderPath.GetChar() );

		// if not existing, create dir and break loop
		if ( !dir.exists() ) {
			dir.mkpath( "." );
			break;
		}
	} 
	while ( true );

	// create folder
	CustomTreeItem* treeItem = Memory::Alloc<CustomTreeItem>();
	treeItem->setText( 0, folderName.GetChar() );
	treeItem->setWhatsThis( 0, TYPE_FOLDER );  // we use this field to store data
	treeItem->setWhatsThis( 1, folderPath.GetChar() ); 
	treeItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable );
	treeItem->setIcon( 0, _icons.Get( EditorUtils::ICON_FOLDER ) );

	if ( parentItem != NULL ) {
		parentItem->addChild( treeItem );
		parentItem->setExpanded( TRUE );
	}
	else {
		_qtTree->addTopLevelItem( treeItem );
	}

	// select item
	_qtTree->setCurrentItem( treeItem );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_ASSET, NULL );
}


void Panel_Assets::A_Create_Mat_Empty()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::MATERIAL );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Panel_Assets::A_Create_Mat_PS()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::MATERIAL );

	Material* material = CAST_S( Material*, asset );
	material->SetEmitEvents( FALSE );
	material->SetProperty( "particleTexture", NULL );
	material->SetProperty( "colorStart", Vector4(1, 1, 1, 1) );
	material->SetProperty( "colorEnd", Vector4(1, 1, 1, 1) );
	material->SetEmitEvents( TRUE );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Panel_Assets::A_Create_Tex_2D()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::TEXTURE );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Panel_Assets::A_Create_Tex_CM()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::TEXTURE );

	Texture* texture = CAST_S( Texture*, asset );
	texture->SetEmitEvents( FALSE );
	texture->SetTextureType( TextureType::CUBE_MAP );
	texture->SetEmitEvents( TRUE );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Panel_Assets::A_Create_Shader()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::SHADER );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Panel_Assets::A_Create_Mesh()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::MESH );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Panel_Assets::A_Create_Morph()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::MORPH );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Panel_Assets::A_Create_Script()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::SCRIPT );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void Panel_Assets::A_DirChanged( const QString& path )
{
	// TODO
}


void Panel_Assets::A_ReloadAll()
{
	ReloadAllAssets();
}


void Panel_Assets::ReloadAllAssets()
{
	ASSERT( _isInitialized );

	// clear assets from managers
	AssetManager::Singleton()->ClearAll();

	// clear tree
	while ( _qtTree->topLevelItemCount() > 0 ) {
		Memory::Free( _qtTree->takeTopLevelItem(0) );
	}

	// add all editor assets
	_ParseDirectory( EditorSettings::DIR_DEFALUT_ASSETS, 
					 EditorSettings::DIR_PATH_DEFALUT_ASSETS,
					 _qtTree->invisibleRootItem() );
	// add all project assets
	_ParseDirectory( EditorSettings::projectName.GetChar(), 
					 ProjectSettings::dirPathAssets.GetChar(), 
					 _qtTree->invisibleRootItem() );

	// expand main folders
	for ( int i = 0; i < _qtTree->topLevelItemCount(); i++ ) {
		_qtTree->topLevelItem( i )->setExpanded( TRUE );
	}
}


CustomTreeItem* Panel_Assets::AddAssetToTree( Asset* asset, QTreeWidgetItem* parentItem, 
											  const char* icon )
{
	// create item
	CustomTreeItem* treeItem = Memory::Alloc<CustomTreeItem>();

	// set item data
	FiniteString filePath( "%s%s%s", asset->GetDirPath(), asset->GetName(), asset->GetExtension() );
	treeItem->asset = asset;
	treeItem->setWhatsThis( 1, filePath.GetChar() ); 
	treeItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable );
	treeItem->setText( 0, asset->GetName() );
	treeItem->setIcon( 0, _icons.Get( icon ) );

	if ( parentItem != NULL ) {
		parentItem->addChild( treeItem );
		parentItem->setExpanded( TRUE );
	}
	else {
		_qtTree->addTopLevelItem( treeItem );
	}

	return treeItem;
}


void Panel_Assets::_LoadIcons()
{
	ASSERT( _isInitialized );

	// parse directory
	QDirIterator dirIterator( EditorSettings::DIR_PATH_ICONS_ASSETS, QDir::Files );
	while ( dirIterator.hasNext() ) {
		dirIterator.next();
		QFileInfo& fileInfo = dirIterator.fileInfo();

		_icons.Set( fileInfo.completeBaseName().toUtf8().data(), 
					QIcon( fileInfo.filePath().toUtf8().data() ) );
	}
}


void Panel_Assets::_ParseDirectory( const char* dirName, const char* dirPath, 
								   QTreeWidgetItem* parentItem )
{
	ASSERT( _isInitialized );

	// create folder in panel
	CustomTreeItem* dirTreeItem = Memory::Alloc<CustomTreeItem>();
	dirTreeItem->setText( 0, dirName );
	dirTreeItem->setWhatsThis( 0, TYPE_FOLDER );  // we use this field to store data
	dirTreeItem->setWhatsThis( 1, dirPath ); 
	dirTreeItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable );
	dirTreeItem->setIcon( 0, _icons.Get( EditorUtils::ICON_FOLDER ) );
	// add to panel
	parentItem->addChild( dirTreeItem );


	// parse directory
	QDirIterator dirIterator( dirPath, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot );
	while ( dirIterator.hasNext() ) {
		dirIterator.next();
		QFileInfo& fileInfo = dirIterator.fileInfo();

		if ( fileInfo.isDir() ) {
			FiniteString newDirPath( "%s/", fileInfo.filePath().toUtf8().constData() );
			_ParseDirectory( fileInfo.completeBaseName().toUtf8().constData(), newDirPath.GetChar(), 
							 dirTreeItem );
		}
		else  if ( fileInfo.isFile() ) {
			QString fileFormat = fileInfo.suffix();
			fileFormat.prepend( "." );
			QString fileName = fileInfo.completeBaseName();
			QIcon icon;
			bool isUnknown = FALSE;
			Asset* asset = NULL;
			StatusAssetAdd statusAdd = StatusAssetAdd::SUCCESS;


			if ( fileFormat.compare( FileManager::FILE_FORMAT_SCENE ) == 0 ) {
				icon = _icons.Get( EditorUtils::ICON_SCENE );

				Scene* scene = Memory::Alloc<Scene>();
				scene->SetEmitEvents( FALSE );
				scene->SetName( fileName.toUtf8().constData() );
				scene->SetDirPath( dirPath );
				scene->LoadUniqueID();
				scene->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddScene( scene );
				asset = scene; // will be checked later
			}
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_MATERIAL ) == 0 ) {
				icon = _icons.Get( EditorUtils::ICON_MATERIAL );

				Material* material = Memory::Alloc<Material>();
				material->SetEmitEvents( FALSE );
				material->SetName( fileName.toUtf8().constData() );
				material->SetDirPath( dirPath );
				material->LoadUniqueID();
				material->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddMaterial( material );
				asset = material; // will be checked later
			}
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_MESH ) == 0 ) {
				icon = _icons.Get( EditorUtils::ICON_MESH );
				
				Mesh* mesh = Memory::Alloc<Mesh>();
				mesh->SetEmitEvents( FALSE );
				mesh->SetName( fileName.toUtf8().constData() );
				mesh->SetDirPath( dirPath );
				mesh->LoadUniqueID();
				mesh->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddMesh( mesh );
				asset = mesh; // will be checked later
			}
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_MORPH ) == 0 ) {
				icon = _icons.Get( EditorUtils::ICON_MORPH );
				
				Morph* morph = Memory::Alloc<Morph>();
				morph->SetEmitEvents( FALSE );
				morph->SetName( fileName.toUtf8().constData() );
				morph->SetDirPath( dirPath );
				morph->LoadUniqueID();
				morph->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddMorph( morph );
				asset = morph; // will be checked later
			}
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_SHADER ) == 0 ) {
				icon = _icons.Get( EditorUtils::ICON_SHADER );
				
				Shader* shader = Memory::Alloc<Shader>();
				shader->SetEmitEvents( FALSE );
				shader->SetName( fileName.toUtf8().constData() );
				shader->SetDirPath( dirPath );
				shader->LoadUniqueID();
				shader->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddShader( shader );
				asset = shader; // will be checked later
			}
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_TEXTURE ) == 0 ) {
				icon = _icons.Get( EditorUtils::ICON_TEXTURE );
				
				Texture* texture = Memory::Alloc<Texture>();
				texture->SetEmitEvents( FALSE );
				texture->SetName( fileName.toUtf8().constData() );
				texture->SetDirPath( dirPath );
				texture->LoadUniqueID();
				texture->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddTexture( texture );
				asset = texture; // will be checked later
			}
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_SCRIPT ) == 0 ) {
				icon = _icons.Get( EditorUtils::ICON_SCRIPT );
				
				Script* script = Memory::Alloc<Script>();
				script->SetEmitEvents( FALSE );
				script->SetName( fileName.toUtf8().constData() );
				script->SetDirPath( dirPath );
				script->LoadUniqueID();
				script->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddScript( script );
				asset = script; // will be checked later
			}
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_PREFAB ) == 0 ) {
				icon = _icons.Get( EditorUtils::ICON_PREFAB );

				Prefab* prefab = Memory::Alloc<Prefab>();
				prefab->SetEmitEvents( FALSE );
				prefab->SetName( fileName.toUtf8().constData() );
				prefab->SetDirPath( dirPath );
				prefab->LoadUniqueID();
				prefab->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddPrefab( prefab );
				asset = prefab; // will be checked later
			}
			else { // unknown file
				isUnknown = TRUE;
				fileFormat = NULL;
				fileName = NULL;
			}

			if ( !isUnknown ) {
				switch ( statusAdd ) {
					case StatusAssetAdd::SUCCESS:
						if ( asset != NULL ) {
							asset->SetIsTemporary( FALSE );
						}
						break;

					case StatusAssetAdd::FAIL_INVALID_ID:
					{
						FiniteString warning( DEBUG_INVALID_UID, 
											  fileName.toUtf8().constData(),
											  asset->GetExtension() );
						DebugManager::Singleton()->Log( warning.GetChar() );

						asset->SetUniqueID( Random::GenerateUniqueID().GetChar() );
						break;
					}

					case StatusAssetAdd::FAIL_EXISTING:
					{
						// if temporary, reload the temporary one and delete this one
						Asset* other = NULL;
						const char* uid = asset->GetUniqueID();

						switch ( asset->GetAssetType() ) {
							case AssetType::MATERIAL:
								other = AssetManager::Singleton()->GetMaterial( uid );
								break;

							case AssetType::MESH:
								other = AssetManager::Singleton()->GetMesh( uid );
								break;

							case AssetType::MORPH:
								other = AssetManager::Singleton()->GetMorph( uid );
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

							case AssetType::SCRIPT:
								other = AssetManager::Singleton()->GetScript( uid );
								break;

							case AssetType::PREFAB:
								other = AssetManager::Singleton()->GetPrefab( uid );
								break;
						}

						if ( other->IsTemporary() )	{
							other->SetEmitEvents( FALSE );
							other->SetName( fileName.toUtf8().constData() );
							other->SetDirPath( dirPath );
							other->SetIsTemporary( FALSE );
							other->SetEmitEvents( TRUE );

							Memory::Free( asset );
							asset = other;
						}
						else {
							FiniteString warning( DEBUG_EXISTING_ASSET,   
												  fileName.toUtf8().constData(),
												  asset->GetExtension() );
							DebugManager::Singleton()->Log( warning.GetChar() );

							Memory::Free( asset );
							asset = NULL;
						}
						break;
					}
				}

				if ( asset != NULL && asset->GetAssetType() != AssetType::SCENE ) {
					asset->SetEmitEvents( FALSE );
					asset->LoadFullFile();

					// for script, also load data
					if ( asset->GetAssetType() == AssetType::SCRIPT ) {
						CAST_S( Script*, asset )->LoadLuaFiles();
					}

					asset->SetEmitEvents( TRUE );
				}
			}

			CustomTreeItem* treeItem = Memory::Alloc<CustomTreeItem>();
			treeItem->asset = asset;
			treeItem->setWhatsThis( 1, fileInfo.filePath() ); 
			treeItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable );
			treeItem->setText( 0, fileName );
			treeItem->setIcon( 0, icon );

			dirTreeItem->addChild( treeItem );

			if ( isUnknown ) {
				QLabel* fileLabel = Memory::Alloc<QLabel>( fileInfo.fileName() );
				fileLabel->setObjectName( EditorSkin::ASSET_UNKNOWN );
				_qtTree->setItemWidget( treeItem, 0, fileLabel );
			}
		}
	}
}


Asset* Panel_Assets::_AddNewAsset( const char* dirPath, QTreeWidgetItem* parentItem,
								  AssetType assetType )
{
	Asset*		asset		= NULL;
	const char* icon		= NULL;

	switch ( assetType ) {
		case AssetType::MATERIAL:
		{
			Material* material = Memory::Alloc<Material>();
			material->SetEmitEvents( FALSE );
			material->SetDirPath( dirPath );
			material->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			material->SetIsTemporary( FALSE );
			EditorUtils::SetAvailableName( material );
			material->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddMaterial( material );

			asset = material;
			icon = EditorUtils::ICON_MATERIAL;
			break;
		}

		case AssetType::MESH:
		{
			Mesh* mesh = Memory::Alloc<Mesh>();
			mesh->SetEmitEvents( FALSE );
			mesh->SetDirPath( dirPath );
			mesh->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			mesh->SetIsTemporary( FALSE );
			EditorUtils::SetAvailableName( mesh );
			mesh->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddMesh( mesh );

			asset = mesh;
			icon = EditorUtils::ICON_MESH;
			break;
		}

		case AssetType::MORPH:
		{
			Morph* morph = Memory::Alloc<Morph>();
			morph->SetEmitEvents( FALSE );
			morph->SetDirPath( dirPath );
			morph->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			morph->SetIsTemporary( FALSE );
			EditorUtils::SetAvailableName( morph );
			morph->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddMorph( morph );

			asset = morph;
			icon = EditorUtils::ICON_MORPH;
			break;
		}

		case AssetType::SCRIPT:
		{
			Script* script = Memory::Alloc<Script>();
			script->SetEmitEvents( FALSE );
			script->SetDirPath( dirPath );
			script->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			script->SetIsTemporary( FALSE );
			EditorUtils::SetAvailableName( script );
			script->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddScript( script );

			asset = script;
			icon = EditorUtils::ICON_SCRIPT;
			break;
		}

		case AssetType::SHADER:
		{
			Shader* shader = Memory::Alloc<Shader>();
			shader->SetEmitEvents( FALSE );
			shader->SetDirPath( dirPath );
			shader->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			shader->SetIsTemporary( FALSE );
			EditorUtils::SetAvailableName( shader );
			shader->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddShader( shader );

			asset = shader;
			icon = EditorUtils::ICON_SHADER;
			break;
		}

		case AssetType::TEXTURE:
		{
			Texture* texture = Memory::Alloc<Texture>();
			texture->SetEmitEvents( FALSE );
			texture->SetDirPath( dirPath );
			texture->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			texture->SetIsTemporary( FALSE );
			EditorUtils::SetAvailableName( texture );
			texture->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddTexture( texture );

			asset = texture;
			icon = EditorUtils::ICON_TEXTURE;
			break;
		}
	}

	// add to tree
	QTreeWidgetItem* newItem = AddAssetToTree( asset, parentItem, icon );

	// select item
	_qtTree->setCurrentItem( newItem );
	EventManager::Singleton()->EmitEvent( EventType::SELECT_ASSET, asset );

	return asset;
}


void Panel_Assets::_CreateRightClickMenu()
{
	ASSERT( _isInitialized );

	// create menus
	_menuAsset			= Memory::Alloc<Menu_Asset>( _qtTree, this );
	_menuAssetFolder	= Memory::Alloc<Menu_AssetFolder>( _qtTree );
	_menuAssetUnknown	= Memory::Alloc<Menu_AssetUnknown>( _qtTree );

	_qtTree->setContextMenuPolicy( Qt::CustomContextMenu );
	QObject::connect( _qtTree, &QWidget::customContextMenuRequested, 
					  this, &Panel_Assets::A_RightClickMenu );
}


void Panel_Assets::_AddRightClickActions( QTreeWidgetItem* item )
{
	ASSERT( _isInitialized );

	if ( item != NULL ) {
		// show asset menu
	}

	if ( item == NULL || item->whatsThis(0).compare( TYPE_FOLDER ) == 0 ) {
		QMenu* menuCreate = _qtRightClickMenu->addMenu( MENU_CREATE );

		QAction* actionFolder = menuCreate->addAction( EditorUtils::NAME_FOLDER );
		menuCreate->addSeparator();

		QMenu* menuMaterial = menuCreate->addMenu( EditorUtils::NAME_MATERIAL );

		QAction* actionMatEmpty = menuMaterial->addAction( EditorUtils::NAME_MAT_EMPTY );
		QAction* actionMatPS = menuMaterial->addAction( EditorUtils::NAME_MAT_PS );

		QMenu* menuTexture = menuCreate->addMenu( EditorUtils::NAME_TEXTURE );
		QAction* actionTex2D = menuTexture->addAction( EditorUtils::NAME_TEX_2D );
		QAction* actionTexCM = menuTexture->addAction( EditorUtils::NAME_TEX_CM );

		QAction* actionShader	= menuCreate->addAction( EditorUtils::NAME_SHADER );
		QAction* actionMesh		= menuCreate->addAction( EditorUtils::NAME_MESH );
		QAction* actionMorph	= menuCreate->addAction( EditorUtils::NAME_MORPH );
		QAction* actionScript	= menuCreate->addAction( EditorUtils::NAME_SCRIPT );

		QObject::connect( actionFolder,		&QAction::triggered, this, &Panel_Assets::A_Create_Folder );
		QObject::connect( actionMatEmpty,	&QAction::triggered, this, &Panel_Assets::A_Create_Mat_Empty );
		QObject::connect( actionMatPS,		&QAction::triggered, this, &Panel_Assets::A_Create_Mat_PS );
		QObject::connect( actionTex2D,		&QAction::triggered, this, &Panel_Assets::A_Create_Tex_2D );
		QObject::connect( actionTexCM,		&QAction::triggered, this, &Panel_Assets::A_Create_Tex_CM );
		QObject::connect( actionShader,		&QAction::triggered, this, &Panel_Assets::A_Create_Shader );
		QObject::connect( actionMesh,		&QAction::triggered, this, &Panel_Assets::A_Create_Mesh );
		QObject::connect( actionMorph,		&QAction::triggered, this, &Panel_Assets::A_Create_Morph );
		QObject::connect( actionScript,		&QAction::triggered, this, &Panel_Assets::A_Create_Script );
	}
}


void Panel_Assets::_SaveAssetToFile( Asset* asset, const char* oldName )
{
	ASSERT( asset != NULL );

	// first try to write new file, then delete old one
	// this way, if writing fails, you still got old file

	FiniteString filePath;
	const char* fileFormat = NULL;
	String data;

	switch ( asset->GetAssetType() ) {
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

		case AssetType::MESH:
		{
			fileFormat = FileManager::FILE_FORMAT_MESH;
			Mesh* mesh = CAST_S( Mesh*, asset );
			data = FileManager::Singleton()->Serialize<Mesh>( mesh );
			break;
		}

		case AssetType::MORPH:
		{
			fileFormat = FileManager::FILE_FORMAT_MORPH;
			Morph* morph = CAST_S( Morph*, asset );
			data = FileManager::Singleton()->Serialize<Morph>( morph );
			break;
		}

		case AssetType::SCRIPT:
		{
			fileFormat = FileManager::FILE_FORMAT_SCRIPT;
			Script* script = CAST_S( Script*, asset );
			data = FileManager::Singleton()->Serialize<Script>( script );
			break;
		}

		case AssetType::SCENE:
		{
			fileFormat = FileManager::FILE_FORMAT_SCENE;
			filePath.Set( "%s%s%s", asset->GetDirPath(), oldName, fileFormat );

			QFile file( filePath.GetChar() );
			bool exists = file.exists();
			if ( exists ) {
				filePath.Set( "%s%s%s", asset->GetDirPath(), asset->GetName(), fileFormat );
				bool success = file.rename( filePath.GetChar() );
			}

			return;
		}

		case AssetType::PREFAB:
		{
			fileFormat = FileManager::FILE_FORMAT_PREFAB;
			Prefab* prefab = CAST_S( Prefab*, asset );
			data = FileManager::Singleton()->Serialize<Prefab>( prefab );
			break;
		}
		
		default:
			return;
	}

	filePath.Set( "%s%s%s", asset->GetDirPath(), asset->GetName(), fileFormat );

	bool success = FileManager::Singleton()->WriteFile( filePath.GetChar(), 
														data.GetChar() );

	// now delete old file
	if ( success && oldName != NULL && String(oldName) != asset->GetName() ) {
		filePath.Set( "%s%s%s", asset->GetDirPath(), oldName, fileFormat );
		FileManager::Singleton()->DeleteFile( filePath.GetChar() );
	}
}


CustomTreeItem* Panel_Assets::_FindTreeItem( Asset* asset )
{
	QTreeWidgetItemIterator it( _qtTree );
	while ( *it != NULL ) {
		CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, *it );
		if ( asset == treeItem->asset ) {
			return treeItem;
		}
		// next
		it++;
	}

	return NULL;
}


CustomTreeItem* Panel_Assets::_FindFolderItem( const char* dirPath )
{
	QTreeWidgetItemIterator it( _qtTree );
	while ( *it != NULL ) {
		CustomTreeItem* treeItem = CAST_S( CustomTreeItem*, *it );
		if ( treeItem->whatsThis( 1 ).compare( dirPath ) == 0 ) {
			return treeItem;
		}
		// next
		it++;
	}

	return NULL;
}
