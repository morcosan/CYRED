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
#include "../../Utils/EditorEvents.h"

#include "QtWidgets\qboxlayout.h"
#include "QtWidgets\qtreewidget.h"
#include "QtCore\qdiriterator.h"
#include "QtCore\qfileinfo.h"
#include "QtWidgets\qmenu.h"
#include "QtCore\qurl.h"
#include "QtCore\qfilesystemwatcher.h"
#include "QtWidgets\qpushbutton.h"
#include "QtGui\qevent.h"


using namespace CYRED;


class Panel_Assets::_QtTree : public QTreeWidget
{
public:
	void mousePressEvent( QMouseEvent* event ) override
	{
		QModelIndex item = indexAt( event->pos() );
		bool selected = selectionModel()->isSelected( indexAt( event->pos() ) );
		QTreeView::mousePressEvent( event );
		if ( (item.row() == -1 && item.column() == -1) || selected ) {
			this->clearSelection();
			// send event
			EventManager::Singleton()->EmitEvent( EditorEventType::ASSET_SELECT, NULL );
		}
	}


	void dropEvent( QDropEvent* e ) override
	{
		// get moved item
		CustomTreeItem* movedItem = CAST_S( CustomTreeItem*, this->currentItem() );
		// get old parent item
		CustomTreeItem* prevParent = CAST_S( CustomTreeItem*, movedItem->parent() );
		// get the order in the old hierarchy
		int prevIndexInHierarchy = prevParent->indexOfChild( movedItem );

		// apply the drop event
		QTreeWidget::dropEvent( e );	

		// get new parent item
		CustomTreeItem* newParent = CAST_S( CustomTreeItem*, movedItem->parent() );
		// check if drop is outside root
		if ( newParent == NULL ) {
			// if so, reset drop
			// remove from tree
			int tmpIndex = this->indexOfTopLevelItem( movedItem );
			this->takeTopLevelItem( tmpIndex );
			// add back to old position
			prevParent->insertChild( prevIndexInHierarchy, movedItem );
			// select item
			this->setCurrentItem( movedItem );
			// exit
			return;
		}
		// check if parent not folder
		else if ( newParent->whatsThis(0).compare( EditorUtils::NAME_FOLDER ) != 0 ) {
			// if so, reset drop
			// remove from tree
			int tmpIndex = newParent->indexOfChild( movedItem );
			newParent->takeChild( tmpIndex );
			// add back to old position
			prevParent->insertChild( prevIndexInHierarchy, movedItem );
			// select item
			this->setCurrentItem( movedItem );
			// exit
			return;
		}

		// select item
		this->setCurrentItem( movedItem );

		// get the order in the new hierarchy
		int indexInHierarchy = newParent->indexOfChild( movedItem );

		// move file on disk, also works for directories
		QFileInfo fileInfo( movedItem->whatsThis(1) );
		// create new path
		QString newFilePath( newParent->whatsThis( 1 ) );
		if ( fileInfo.isDir() ) {
			newFilePath.append( movedItem->text(0) ).append("//");
		}
		else {
			newFilePath.append( fileInfo.fileName() );
		}

		// move file
		bool success = QDir().rename( movedItem->whatsThis(1), newFilePath );

		if ( success ) {
			// update file path
			this->blockSignals( true );
			movedItem->setWhatsThis( 1, newFilePath );
			this->blockSignals( false );
		}
		else {
			// reset drop
			// remove from tree
			int tmpIndex = newParent->indexOfChild( movedItem );
			newParent->takeChild( tmpIndex );
			// add back to old position
			prevParent->insertChild( prevIndexInHierarchy, movedItem );
			// select item
			this->setCurrentItem( movedItem );
		}
	}
};


Panel_Assets::Panel_Assets()
	: _openedPrefab( NULL )
{
	this->setWindowTitle( PANEL_TITLE );
	this->setMinimumSize( MIN_SIZE.x, MIN_SIZE.y );

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

	_qtTree = Memory::Alloc<_QtTree>();
	_qtTree->setHeaderHidden( true );
	_qtTree->setDragEnabled( true );
	_qtTree->setDragDropMode( QAbstractItemView::InternalMove );
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

	_CreateRightClickMenu();

	// register events
	EventManager::Singleton()->RegisterListener( this, EventType::ALL );
}


void Panel_Assets::Finalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( this, EventType::ALL );
}


void Panel_Assets::OnEvent( int eventType, void* eventData )
{
	ASSERT( _isInitialized );

	switch ( eventType ) {
		case EditorEventType::SCENE_SELECT:
		case EditorEventType::PREFAB_SELECT:
		case EditorEventType::GAMEOBJECT_SELECT:
			_qtTree->setCurrentItem( NULL );
			break;

		case EditorEventType::PREFAB_OPEN:
			_openedPrefab = CAST_S( Prefab*, eventData );
			break;

		case EditorEventType::PREFAB_CLOSE:
			if ( _openedPrefab == eventData ) {
				_openedPrefab = NULL;
			}
			break;

		case EventType::ASSET_RENAME:
		case EventType::ASSET_UPDATE:
		{
			Asset* asset = CAST_S( Asset*, eventData );

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
					cchar* icon = NULL;
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

	EventManager::Singleton()->EmitEvent( EditorEventType::ASSET_SELECT, asset );
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
			_menuAsset->Open( pos );
		}
		else if ( treeItem == NULL || treeItem->whatsThis(0).compare( EditorUtils::NAME_FOLDER ) == 0 ) {
			// open folder menu
			_menuAssetFolder->Open( pos );
		}
		else {
			// open unknown menu
			_menuAssetUnknown->Open( pos );
		}
	}
}


void Panel_Assets::A_DirChanged( const QString& path )
{
	// TODO
}


void Panel_Assets::A_ReloadAll()
{
	// close prefab first
	EventManager::Singleton()->EmitEvent( EditorEventType::PREFAB_CLOSE, _openedPrefab );

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
											  cchar* icon )
{
	// create item
	CustomTreeItem* treeItem = Memory::Alloc<CustomTreeItem>();

	// set item data
	FiniteString filePath( "%s%s%s", asset->GetDirPath(), asset->GetName(), asset->GetExtension() );
	treeItem->asset = asset;
	treeItem->setWhatsThis( 1, filePath.GetChar() ); 
	treeItem->setFlags( Qt::ItemIsSelectable | 
						Qt::ItemIsEnabled | 
						Qt::ItemIsDragEnabled |
						Qt::ItemIsDropEnabled | 
						Qt::ItemIsEditable );
	treeItem->setText( 0, asset->GetName() );
	treeItem->setIcon( 0, *EditorUtils::GetIcon( icon ) );

	if ( parentItem != NULL ) {
		parentItem->addChild( treeItem );
		parentItem->setExpanded( TRUE );
	}
	else {
		_qtTree->addTopLevelItem( treeItem );
	}

	return treeItem;
}


void Panel_Assets::_ParseDirectory( cchar* dirName, cchar* dirPath, 
								    QTreeWidgetItem* parentItem )
{
	ASSERT( _isInitialized );

	// create folder in panel
	CustomTreeItem* dirTreeItem = Memory::Alloc<CustomTreeItem>();
	dirTreeItem->setText( 0, dirName );
	dirTreeItem->setWhatsThis( 0, EditorUtils::NAME_FOLDER );  // we use this field to store data
	dirTreeItem->setWhatsThis( 1, dirPath ); 
	dirTreeItem->setIcon( 0, *EditorUtils::GetIcon( EditorUtils::ICON_FOLDER ) );

	if ( parentItem == _qtTree->invisibleRootItem() ) {
		// root folder
		dirTreeItem->setFlags( Qt::ItemIsDropEnabled | Qt::ItemIsEnabled );
	}
	else {
		// normal folder
		dirTreeItem->setFlags( Qt::ItemIsSelectable | 
							   Qt::ItemIsEnabled | 
							   Qt::ItemIsDragEnabled |
							   Qt::ItemIsDropEnabled | 
							   Qt::ItemIsEditable );
	}
	
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
				icon = *EditorUtils::GetIcon( EditorUtils::ICON_SCENE );

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
				icon = *EditorUtils::GetIcon( EditorUtils::ICON_MATERIAL );

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
				icon = *EditorUtils::GetIcon( EditorUtils::ICON_MESH );
				
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
				icon = *EditorUtils::GetIcon( EditorUtils::ICON_MORPH );
				
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
				icon = *EditorUtils::GetIcon( EditorUtils::ICON_SHADER );
				
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
				icon = *EditorUtils::GetIcon( EditorUtils::ICON_TEXTURE );
				
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
				icon = *EditorUtils::GetIcon( EditorUtils::ICON_SCRIPT );
				
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
				icon = *EditorUtils::GetIcon( EditorUtils::ICON_PREFAB );

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
						cchar* uid = asset->GetUniqueID();

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
			treeItem->setFlags( Qt::ItemIsSelectable | 
								Qt::ItemIsEnabled |
								Qt::ItemIsDragEnabled |
								Qt::ItemIsDropEnabled | 
								Qt::ItemIsEditable );
			treeItem->setText( 0, fileName );
			treeItem->setIcon( 0, icon );

			dirTreeItem->addChild( treeItem );

			if ( isUnknown ) {
				treeItem->setText( 0, fileInfo.fileName() );
			}
		}
	}
}


void Panel_Assets::_CreateRightClickMenu()
{
	ASSERT( _isInitialized );

	// create menus
	_menuAsset			= Memory::Alloc<Menu_Asset>( _qtTree, this );
	_menuAssetFolder	= Memory::Alloc<Menu_AssetFolder>( _qtTree, this );
	_menuAssetUnknown	= Memory::Alloc<Menu_AssetUnknown>( _qtTree );

	_qtTree->setContextMenuPolicy( Qt::CustomContextMenu );
	QObject::connect( _qtTree, &QWidget::customContextMenuRequested, 
					  this, &Panel_Assets::A_RightClickMenu );
}


void Panel_Assets::_SaveAssetToFile( Asset* asset, cchar* oldName )
{
	ASSERT( asset != NULL );

	// first try to write new file, then delete old one
	// this way, if writing fails, you still got old file

	FiniteString filePath;
	cchar* fileFormat = NULL;
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


CustomTreeItem* Panel_Assets::_FindFolderItem( cchar* dirPath )
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
