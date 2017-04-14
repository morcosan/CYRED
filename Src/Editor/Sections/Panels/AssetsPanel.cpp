// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "AssetsPanel.h"
#include "CyredModule_File.h"
#include "CyredModule_Asset.h"
#include "CyredModule_Render.h"
#include "CyredModule_Debug.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Script.h"
#include "../Settings/EditorSkin.h"
#include "../Settings/ProjectSettings.h"
#include "../Settings/EditorSettings.h"

#include "QtWidgets\qboxlayout.h"
#include "QtWidgets\qtreewidget.h"
#include "QtCore\qdiriterator.h"
#include "QtCore\qfileinfo.h"
#include "QtWidgets\qmenu.h"
#include "QtGui\QDesktopServices"
#include "QtCore\qurl.h"
#include "QtWidgets\qlabel.h"
#include "QtCore\qfilesystemwatcher.h"
#include "QtWidgets\qpushbutton.h"


using namespace CYRED;



class AssetsPanel::_QtTreeItem : public QTreeWidgetItem
{
public:
	Asset* asset;

	~_QtTreeItem()
	{
		while ( childCount() > 0 )
		{
			Memory::Free( takeChild(0) );
		}
	}
};


AssetsPanel::AssetsPanel()
{
	this->setWindowTitle( PANEL_TITLE );
	this->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
	this->setAllowedAreas( Qt::DockWidgetArea::AllDockWidgetAreas );
	this->setMinimumSize( MIN_SIZE.x, MIN_SIZE.y );
	this->setMaximumSize( MAX_SIZE.x, MAX_SIZE.y );

	QPushButton* reloadBtn = Memory::Alloc<QPushButton>( this );
	reloadBtn->setText( BUTTON_RELOAD );
	reloadBtn->setObjectName( EditorSkin::ASSET_BUTTON );
	QObject::connect( reloadBtn, &QPushButton::clicked,	this, &AssetsPanel::A_ReloadAll );

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

	QObject::connect( _qtTree, &QTreeWidget::itemPressed,		this, &AssetsPanel::A_ItemClicked );
	QObject::connect( _qtTree, &QTreeWidget::itemDoubleClicked, this, &AssetsPanel::A_Item2xClicked );
	QObject::connect( _qtTree, &QTreeWidget::itemChanged,		this, &AssetsPanel::A_ItemRenamed );

	_qtFileWatcher = Memory::Alloc<QFileSystemWatcher>();
	_qtFileWatcher->addPath( ProjectSettings::dirPathAssets.GetChar() );

	QObject::connect( _qtFileWatcher, &QFileSystemWatcher::directoryChanged,	
					  this, &AssetsPanel::A_DirChanged );
}


void AssetsPanel::Initialize()
{
	ASSERT( !_isInitialized );
	_isInitialized = TRUE;

	_LoadIcons();
	_CreateRightClickMenu();

	// register events
	EventManager::Singleton()->RegisterListener( EventType::SELECT_SCENE, this );
	EventManager::Singleton()->RegisterListener( EventType::SELECT_GAMEOBJECT, this );
	EventManager::Singleton()->RegisterListener( EventType::CHANGE_ASSET, this );
}


void AssetsPanel::Finalize()
{
	// unregister events
	EventManager::Singleton()->UnregisterListener( EventType::SELECT_SCENE, this );
	EventManager::Singleton()->UnregisterListener( EventType::SELECT_GAMEOBJECT, this );
	EventManager::Singleton()->UnregisterListener( EventType::CHANGE_ASSET, this );
}


void AssetsPanel::OnEvent( EventType eType, void* eData )
{
	ASSERT( _isInitialized );

	switch ( eType )
	{
		case EventType::SELECT_SCENE:
		case EventType::SELECT_GAMEOBJECT:
		{
			_qtTree->setCurrentItem( NULL );
			break;
		}

		case EventType::CHANGE_ASSET:
		{
			Asset* asset = CAST_S( Asset*, eData );

			if ( asset != NULL && !asset->IsTemporary() ) {
				_QtTreeItem* treeItem = _FindTreeItem( asset );

				Bool isDuplicate = _IsFilePathDuplicate( asset );
				if ( isDuplicate )
				{
					DebugManager::Singleton()->Log( DEBUG_DUPLICATED_FILE_PATH );

					// change the name back
					if ( treeItem->text(0).compare( asset->GetName() ) != 0 )
					{
						asset->SetName( treeItem->text(0).toUtf8().constData() );
					}
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
	}
}


void AssetsPanel::A_ItemClicked( QTreeWidgetItem* item, int column )
{
	Asset* asset = CAST_S( _QtTreeItem*, item )->asset;

	EventManager::Singleton()->EmitEvent( EventType::SELECT_ASSET, asset );
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
	_QtTreeItem* treeItem = CAST_S( _QtTreeItem*, item );

	Asset* asset = treeItem->asset;

	if ( asset != NULL )
	{
		FiniteString newName( item->text(0).toUtf8().constData() );

		_qtTree->blockSignals( true );
		item->setText( 0, asset->GetName() );
		_qtTree->blockSignals( false );

		asset->SetName( newName.GetChar() );
	}
	else
	{
		QFileInfo fileInfo( item->whatsThis(1) );

		if ( fileInfo.exists() )
		{
			QDir dir;
			QString newFolder = dir.relativeFilePath( fileInfo.absolutePath() );
			newFolder.append( "/" ).append( item->text(0) );
			
			Bool success = dir.rename( item->whatsThis(1), newFolder );
			if ( success )
			{
				treeItem->setWhatsThis( 1, newFolder );
			}
		}
	}
}


void AssetsPanel::A_RightClickMenu( const QPoint& pos )
{
	QTreeWidgetItem* item = _qtTree->itemAt( pos );

	_qtTree->setCurrentItem( item );

	_AddRightClickActions( item );

	_qtRightClickMenu->popup( _qtTree->mapToGlobal(pos) );
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

		case AssetType::MORPH:
			{
				Morph* morph = CAST_S( Morph*, asset );
				morph->LoadFullFile();
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

		case AssetType::SCRIPT:
		{
			Script* script = CAST_S( Script*, asset );
			script->LoadFullFile();
			script->LoadLuaFiles();
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


void AssetsPanel::A_Duplicate()
{
	// get item selected
	QTreeWidgetItem* item = _qtTree->currentItem();
	// get asset
	Asset* asset = CAST_S( _QtTreeItem*, _qtTree->currentItem() )->asset;
	// create new asset
	Asset* clone = asset->Clone();

	// create asset path for copying
	clone->SetEmitEvents( TRUE );
	_SetAvailableName( clone );

	// copy file
	FiniteString assetPath( "%s%s%s", asset->GetDirPath(), asset->GetName(), asset->GetExtension() );
	FiniteString clonePath( "%s%s%s", clone->GetDirPath(), clone->GetName(), clone->GetExtension() );
	FileManager::Singleton()->CopyFile( assetPath.GetChar(), clonePath.GetChar() );

	// load full file clone
	clone->LoadFullFile();
	// set new UID
	clone->SetUniqueID( Random::GenerateUniqueID().GetChar() );
	clone->SetIsTemporary( FALSE );
	clone->SetEmitEvents( TRUE );

	// add to asset pool and get icon
	const Char* icon = NULL;

	switch ( asset->GetAssetType() ) {
		case AssetType::MATERIAL:
			AssetManager::Singleton()->AddMaterial( CAST_S( Material*, clone ) );
			icon = ICON_MATERIAL;
			break;

		case AssetType::MESH:
			AssetManager::Singleton()->AddMesh( CAST_S( Mesh*, clone ) );
			icon = ICON_MESH;
			break;

		case AssetType::MORPH:
			AssetManager::Singleton()->AddMorph( CAST_S( Morph*, clone ) );
			icon = ICON_MORPH;
			break;

		case AssetType::SCENE:
			AssetManager::Singleton()->AddScene( CAST_S( Scene*, clone ) );
			icon = ICON_SCENE;
			break;

		case AssetType::SCRIPT:
			AssetManager::Singleton()->AddScript( CAST_S( Script*, clone ) );
			icon = ICON_SCRIPT;
			break;

		case AssetType::SHADER:
			AssetManager::Singleton()->AddShader( CAST_S( Shader*, clone ) );
			icon = ICON_SHADER;
			break;

		case AssetType::TEXTURE:
			AssetManager::Singleton()->AddTexture( CAST_S( Texture*, clone ) );
			icon = ICON_TEXTURE;
			break;
	}

	// add to tree
	_AddAssetToTree( clone, item->parent(), icon );

	// send event
	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, clone );
}


void AssetsPanel::A_Delete()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	Asset* asset = CAST_S( _QtTreeItem*, _qtTree->currentItem() )->asset;

	if ( asset != NULL ) {
		// clear
		asset->ClearAsset();

		// remove from manager
		switch ( asset->GetAssetType() ) {
			case AssetType::MATERIAL:
				AssetManager::Singleton()->RemoveMaterial( CAST_S( Material*, asset ) );
				break;

			case AssetType::MESH:
				AssetManager::Singleton()->RemoveMesh( CAST_S( Mesh*, asset ) );
				break;

			case AssetType::MORPH:
				AssetManager::Singleton()->RemoveMorph( CAST_S( Morph*, asset ) );
				break;

			case AssetType::SHADER:
				AssetManager::Singleton()->RemoveShader( CAST_S( Shader*, asset ) );
				break;

			case AssetType::TEXTURE:
				AssetManager::Singleton()->RemoveTexture( CAST_S( Texture*, asset ) );
				break;

			case AssetType::SCENE:
				AssetManager::Singleton()->RemoveScene( CAST_S( Scene*, asset ) );
				break;

			case AssetType::SCRIPT:
				AssetManager::Singleton()->RemoveScript( CAST_S( Script*, asset ) );
				break;
		}
	}

	QFileInfo fileInfo( item->whatsThis(1) );
	if ( fileInfo.isDir() ) {
		QDir( item->whatsThis(1) ).removeRecursively();
	}
	else {
		QDir().remove( item->whatsThis(1) );
	}

	_qtTree->setCurrentItem( NULL );
	Memory::Free( item );

	EventManager::Singleton()->EmitEvent( EventType::SELECT_ASSET, NULL );
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


void AssetsPanel::A_Create_Folder()
{
	QTreeWidgetItem* parentItem = _qtTree->currentItem();

	QString dirPath = (parentItem != NULL) ? parentItem->whatsThis( 1 ).append( "/" ) 
										   : ProjectSettings::dirPathAssets.GetChar();

	// generate name
	// find new name
	Int nextIndex = -1;
	FiniteString folderName;
	FiniteString folderPath;

	do {
		nextIndex++;
		folderName.Set( "%s%d", MENU_C_FOLDER, nextIndex );
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
	_QtTreeItem* treeItem = Memory::Alloc<_QtTreeItem>();
	treeItem->setText( 0, folderName.GetChar() );
	treeItem->setWhatsThis( 0, TYPE_FOLDER );  // we use this field to store data
	treeItem->setWhatsThis( 1, folderPath.GetChar() ); 
	treeItem->setFlags( Qt::ItemIsSelectable | 
						Qt::ItemIsEnabled |
						Qt::ItemIsEditable );
	treeItem->setIcon( 0, _icons.Get( ICON_FOLDER ) );

	if ( parentItem != NULL ) {
		parentItem->addChild( treeItem );
		parentItem->setExpanded( TRUE );
	}
	else {
		_qtTree->addTopLevelItem( treeItem );
	}
}


void AssetsPanel::A_Create_Mat_Empty()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::MATERIAL );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void AssetsPanel::A_Create_Mat_PS()
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


void AssetsPanel::A_Create_Tex_2D()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::TEXTURE );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void AssetsPanel::A_Create_Tex_CM()
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


void AssetsPanel::A_Create_Shader()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::SHADER );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void AssetsPanel::A_Create_Mesh()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::MESH );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void AssetsPanel::A_Create_Morph()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::MORPH );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void AssetsPanel::A_Create_Script()
{
	QTreeWidgetItem* item = _qtTree->currentItem();

	QString dirPath = (item != NULL) ? item->whatsThis( 1 ).append( "/" ) : 
									   ProjectSettings::dirPathAssets.GetChar();
	Asset* asset = _AddNewAsset( dirPath.toUtf8().constData(), item, AssetType::SCRIPT );

	EventManager::Singleton()->EmitEvent( EventType::CHANGE_ASSET, asset );
}


void AssetsPanel::A_DirChanged( const QString& path )
{
	// TODO
}


void AssetsPanel::A_ReloadAll()
{
	ReloadAllAssets();
}


void AssetsPanel::ReloadAllAssets()
{
	ASSERT( _isInitialized );

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
	for ( Int i = 0; i < _qtTree->topLevelItemCount(); i++ ) {
		_qtTree->topLevelItem( i )->setExpanded( TRUE );
	}
}


void AssetsPanel::_LoadIcons()
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


void AssetsPanel::_ParseDirectory( const Char* dirName, const Char* dirPath, 
								   QTreeWidgetItem* parentItem )
{
	ASSERT( _isInitialized );

	// create folder in panel
	_QtTreeItem* dirTreeItem = Memory::Alloc<_QtTreeItem>();
	dirTreeItem->setText( 0, dirName );
	dirTreeItem->setWhatsThis( 0, TYPE_FOLDER );  // we use this field to store data
	dirTreeItem->setWhatsThis( 1, dirPath ); 
	dirTreeItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable );
	dirTreeItem->setIcon( 0, _icons.Get( ICON_FOLDER ) );
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
			Bool isUnknown = FALSE;
			Asset* asset = NULL;
			StatusAssetAdd statusAdd = StatusAssetAdd::SUCCESS;


			if ( fileFormat.compare( FileManager::FILE_FORMAT_SCENE ) == 0 ) {
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
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_MATERIAL ) == 0 ) {
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
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_MESH ) == 0 ) {
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
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_MORPH ) == 0 ) {
				icon = _icons.Get( ICON_MORPH );
				
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
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_TEXTURE ) == 0 ) {
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
			else if ( fileFormat.compare( FileManager::FILE_FORMAT_SCRIPT ) == 0 ) {
				icon = _icons.Get( ICON_SCRIPT );
				
				Script* script = Memory::Alloc<Script>();
				script->SetEmitEvents( FALSE );
				script->SetName( fileName.toUtf8().constData() );
				script->SetDirPath( dirPath );
				script->LoadUniqueID();
				script->SetEmitEvents( TRUE );

				statusAdd = AssetManager::Singleton()->AddScript( script );
				asset = script; // will be checked later
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
						FiniteString warning( DEBUG_INVALID_UID, fileName.toUtf8().constData() );
						DebugManager::Singleton()->Log( warning.GetChar() );

						asset->SetUniqueID( Random::GenerateUniqueID().GetChar() );
						break;
					}

					case StatusAssetAdd::FAIL_EXISTING:
					{
						// if temporary, reload the temporary one and delete this one
						Asset* other = NULL;
						const Char* uid = asset->GetUniqueID();

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
												  fileName.toUtf8().constData() );
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

			_QtTreeItem* treeItem = Memory::Alloc<_QtTreeItem>();
			treeItem->asset = asset;
			treeItem->setWhatsThis( 1, fileInfo.filePath() ); 
			treeItem->setFlags( Qt::ItemIsSelectable | 
								Qt::ItemIsEnabled | 
								Qt::ItemIsEditable );
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


Asset* AssetsPanel::_AddNewAsset( const Char* dirPath, QTreeWidgetItem* parentItem,
								  AssetType assetType )
{
	Asset*		asset		= NULL;
	const Char* icon		= NULL;

	switch ( assetType ) {
		case AssetType::MATERIAL:
		{
			Material* material = Memory::Alloc<Material>();
			material->SetEmitEvents( FALSE );
			material->SetDirPath( dirPath );
			material->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			material->SetIsTemporary( FALSE );
			_SetAvailableName( material );
			material->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddMaterial( material );

			asset = material;
			icon = ICON_MATERIAL;
			break;
		}

		case AssetType::MESH:
		{
			Mesh* mesh = Memory::Alloc<Mesh>();
			mesh->SetEmitEvents( FALSE );
			mesh->SetDirPath( dirPath );
			mesh->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			mesh->SetIsTemporary( FALSE );
			_SetAvailableName( mesh );
			mesh->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddMesh( mesh );

			asset = mesh;
			icon = ICON_MESH;
			break;
		}

		case AssetType::MORPH:
		{
			Morph* morph = Memory::Alloc<Morph>();
			morph->SetEmitEvents( FALSE );
			morph->SetDirPath( dirPath );
			morph->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			morph->SetIsTemporary( FALSE );
			_SetAvailableName( morph );
			morph->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddMorph( morph );

			asset = morph;
			icon = ICON_MORPH;
			break;
		}

		case AssetType::SCRIPT:
		{
			Script* script = Memory::Alloc<Script>();
			script->SetEmitEvents( FALSE );
			script->SetDirPath( dirPath );
			script->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			script->SetIsTemporary( FALSE );
			_SetAvailableName( script );
			script->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddScript( script );

			asset = script;
			icon = ICON_SCRIPT;
			break;
		}

		case AssetType::SHADER:
		{
			Shader* shader = Memory::Alloc<Shader>();
			shader->SetEmitEvents( FALSE );
			shader->SetDirPath( dirPath );
			shader->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			shader->SetIsTemporary( FALSE );
			_SetAvailableName( shader );
			shader->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddShader( shader );

			asset = shader;
			icon = ICON_SHADER;
			break;
		}

		case AssetType::TEXTURE:
		{
			Texture* texture = Memory::Alloc<Texture>();
			texture->SetEmitEvents( FALSE );
			texture->SetDirPath( dirPath );
			texture->SetUniqueID( Random::GenerateUniqueID().GetChar() );
			texture->SetIsTemporary( FALSE );
			_SetAvailableName( texture );
			texture->SetEmitEvents( TRUE );
			AssetManager::Singleton()->AddTexture( texture );

			asset = texture;
			icon = ICON_TEXTURE;
			break;
		}
	}

	// add to tree
	_AddAssetToTree( asset, parentItem, icon );

	return asset;
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

	if ( item != NULL )
	{
		Asset* asset = CAST_S( _QtTreeItem*, item )->asset;

		if ( asset != NULL )
		{
			if ( asset->GetAssetType() == AssetType::SCENE )
			{
				QAction* actionOpenScene = _qtRightClickMenu->addAction( MENU_OPEN_SCENE );
				QAction* actionLoadScene = _qtRightClickMenu->addAction( MENU_LOAD_SCENE );
				_qtRightClickMenu->addSeparator();

				QObject::connect( actionOpenScene, &QAction::triggered, this, &AssetsPanel::A_OpenScene );
				QObject::connect( actionLoadScene, &QAction::triggered, this, &AssetsPanel::A_LoadScene );
			}

			QAction* actionReload = _qtRightClickMenu->addAction( MENU_RELOAD );

			QObject::connect( actionReload, &QAction::triggered, this, &AssetsPanel::A_ReloadAsset );
		}

		QAction* actionRename = _qtRightClickMenu->addAction( MENU_RENAME );
		QAction* actionDuplicate = _qtRightClickMenu->addAction( MENU_DUPLICATE );
		_qtRightClickMenu->addSeparator();
		QAction* actionOpenOnDisk = _qtRightClickMenu->addAction( MENU_OPEN_DISK );
		QAction* actionShowOnDisk = _qtRightClickMenu->addAction( MENU_SHOW_DISK );
		_qtRightClickMenu->addSeparator();
		QAction* actionDelete = _qtRightClickMenu->addAction( MENU_DELETE );
		_qtRightClickMenu->addSeparator();

		QObject::connect( actionRename,		&QAction::triggered, this, &AssetsPanel::A_Rename );
		QObject::connect( actionOpenOnDisk, &QAction::triggered, this, &AssetsPanel::A_OpenOnDisk );
		QObject::connect( actionShowOnDisk, &QAction::triggered, this, &AssetsPanel::A_ShowOnDisk );
		QObject::connect( actionDuplicate,	&QAction::triggered, this, &AssetsPanel::A_Duplicate );
		QObject::connect( actionDelete,		&QAction::triggered, this, &AssetsPanel::A_Delete );
	}

	if ( item == NULL || item->whatsThis(0).compare( TYPE_FOLDER ) == 0 )
	{
		QMenu* menuCreate = _qtRightClickMenu->addMenu( MENU_CREATE );

		QAction* actionFolder = menuCreate->addAction( MENU_C_FOLDER );
		menuCreate->addSeparator();

		QMenu* menuMaterial = menuCreate->addMenu( MENU_C_MATERIAL );

		QAction* actionMatEmpty = menuMaterial->addAction( MENU_C_MAT_EMPTY );
		QAction* actionMatPS = menuMaterial->addAction( MENU_C_MAT_PS );

		QMenu* menuTexture = menuCreate->addMenu( MENU_C_TEXTURE );
		QAction* actionTex2D = menuTexture->addAction( MENU_C_TEX_2D );
		QAction* actionTexCM = menuTexture->addAction( MENU_C_TEX_CM );

		QAction* actionShader	= menuCreate->addAction( MENU_C_SHADER );
		QAction* actionMesh		= menuCreate->addAction( MENU_C_MESH );
		QAction* actionMorph	= menuCreate->addAction( MENU_C_MORPH );
		QAction* actionScript	= menuCreate->addAction( MENU_C_SCRIPT );

		QObject::connect( actionFolder,		&QAction::triggered, this, &AssetsPanel::A_Create_Folder );
		QObject::connect( actionMatEmpty,	&QAction::triggered, this, &AssetsPanel::A_Create_Mat_Empty );
		QObject::connect( actionMatPS,		&QAction::triggered, this, &AssetsPanel::A_Create_Mat_PS );
		QObject::connect( actionTex2D,		&QAction::triggered, this, &AssetsPanel::A_Create_Tex_2D );
		QObject::connect( actionTexCM,		&QAction::triggered, this, &AssetsPanel::A_Create_Tex_CM );
		QObject::connect( actionShader,		&QAction::triggered, this, &AssetsPanel::A_Create_Shader );
		QObject::connect( actionMesh,		&QAction::triggered, this, &AssetsPanel::A_Create_Mesh );
		QObject::connect( actionMorph,		&QAction::triggered, this, &AssetsPanel::A_Create_Morph );
		QObject::connect( actionScript,		&QAction::triggered, this, &AssetsPanel::A_Create_Script );
	}
}


void AssetsPanel::_SaveAssetToFile( Asset* asset, const Char* oldName )
{
	ASSERT( asset != NULL );

	// first try to write new file, then delete old one
	// this way, if writing fails, you still got old file

	FiniteString filePath;
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
			Bool exists = file.exists();
			if ( exists )
			{
				filePath.Set( "%s%s%s", asset->GetDirPath(), asset->GetName(), fileFormat );
				Bool success = file.rename( filePath.GetChar() );
			}

			return;
		}
		
		default:
			return;
	}

	filePath.Set( "%s%s%s", asset->GetDirPath(), asset->GetName(), fileFormat );

	Bool success = FileManager::Singleton()->WriteFile( filePath.GetChar(), 
														data.GetChar() );

	// now delete old file
	if ( success && String(oldName) != asset->GetName() )
	{
		filePath.Set( "%s%s%s", asset->GetDirPath(), oldName, fileFormat );
		FileManager::Singleton()->DeleteFile( filePath.GetChar() );
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

		case AssetType::MESH:
		{
			for ( UInt i = 0; i < assetMgr->GetMeshCount(); ++i )
			{
				Mesh* other = assetMgr->GetMeshAt( i );
				if ( asset != other && 
					 name.compare( other->GetName(), Qt::CaseInsensitive ) == 0 && 
					 dirPath.compare( other->GetDirPath(), Qt::CaseInsensitive ) == 0 )
				{
					return TRUE;
				}
			}
			break;
		}

		case AssetType::MORPH:
		{
			for ( UInt i = 0; i < assetMgr->GetMorphCount(); ++i )
			{
				Morph* other = assetMgr->GetMorphAt( i );
				if ( asset != other && 
					 name.compare( other->GetName(), Qt::CaseInsensitive ) == 0 && 
					 dirPath.compare( other->GetDirPath(), Qt::CaseInsensitive ) == 0 )
				{
					return TRUE;
				}
			}
			break;
		}

		case AssetType::SCRIPT:
		{
			for ( UInt i = 0; i < assetMgr->GetScriptCount(); ++i )
			{
				Script* other = assetMgr->GetScriptAt( i );
				if ( asset != other && 
					 name.compare( other->GetName(), Qt::CaseInsensitive ) == 0 && 
					 dirPath.compare( other->GetDirPath(), Qt::CaseInsensitive ) == 0 )
				{
					return TRUE;
				}
			}
			break;
		}

		case AssetType::SCENE:
		{
			for ( UInt i = 0; i < assetMgr->GetSceneCount(); ++i )
			{
				Scene* other = assetMgr->GetSceneAt( i );
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


void AssetsPanel::_SetAvailableName( Asset* asset )
{
	// get base name and extension according to type
	const Char* baseName = NULL;
	const Char* extension = NULL;

	switch ( asset->GetAssetType() ) {
		case AssetType::MATERIAL:
			baseName = MENU_C_MATERIAL;
			extension = FileManager::FILE_FORMAT_MATERIAL;
			break;

		case AssetType::MESH:
			baseName = MENU_C_MESH;
			extension = FileManager::FILE_FORMAT_MESH;
			break;

		case AssetType::MORPH:
			baseName = MENU_C_MORPH;
			extension = FileManager::FILE_FORMAT_MORPH;
			break;

		case AssetType::SCENE:
			baseName = MENU_C_SCENE;
			extension = FileManager::FILE_FORMAT_SCENE;
			break;

		case AssetType::SCRIPT:
			baseName = MENU_C_SCRIPT;
			extension = FileManager::FILE_FORMAT_SCRIPT;
			break;

		case AssetType::SHADER:
			baseName = MENU_C_SHADER;
			extension = FileManager::FILE_FORMAT_SHADER;
			break;

		case AssetType::TEXTURE:
			baseName = MENU_C_TEXTURE;
			extension = FileManager::FILE_FORMAT_TEXTURE;
			break;
	}

	// find new name
	Int assetIndex = -1;
	do {
		assetIndex++;
		FiniteString fileName( "%s%d", baseName, assetIndex );
		FiniteString filePath( "%s%s%s", asset->GetDirPath(), fileName.GetChar(), extension );
		asset->SetName( fileName.GetChar() );
	} 
	while ( _IsFilePathDuplicate( asset ) );
}


void AssetsPanel::_AddAssetToTree( Asset* asset, QTreeWidgetItem* parentItem, const Char* icon )
{
	// create item
	_QtTreeItem* treeItem = Memory::Alloc<_QtTreeItem>();

	// set item data
	FiniteString filePath( "%s%s%s", asset->GetDirPath(), asset->GetName(), asset->GetExtension() );
	treeItem->asset = asset;
	treeItem->setWhatsThis( 1, filePath.GetChar() ); 
	treeItem->setFlags( Qt::ItemIsSelectable | 
						Qt::ItemIsEnabled | 
						Qt::ItemIsEditable );
	treeItem->setText( 0, asset->GetName() );
	treeItem->setIcon( 0, _icons.Get( icon ) );

	if ( parentItem != NULL ) {
		parentItem->addChild( treeItem );
		parentItem->setExpanded( TRUE );
	}
	else {
		_qtTree->addTopLevelItem( treeItem );
	}
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


