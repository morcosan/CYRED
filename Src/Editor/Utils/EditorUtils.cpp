// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "EditorUtils.h"
#include "CyredBuildingBlocks.h"
#include "CyredModule_Asset.h"
#include "CyredModule_File.h"
#include "CyredModule_Render.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Script.h"

#include "../Sections/Settings/EditorSettings.h"

#include "QtCore/QString"
#include "QtCore\qfileinfo.h"
#include "QtGui\qicon.h"
#include "QtCore\qdiriterator.h"


using namespace CYRED;


cchar* const EditorUtils::ICON_SCENE		= "scene";
cchar* const EditorUtils::ICON_FOLDER		= "folder";
cchar* const EditorUtils::ICON_MATERIAL		= "material";
cchar* const EditorUtils::ICON_MESH			= "mesh";
cchar* const EditorUtils::ICON_MORPH		= "morph";
cchar* const EditorUtils::ICON_TEXTURE		= "texture";
cchar* const EditorUtils::ICON_SHADER		= "shader";
cchar* const EditorUtils::ICON_SCRIPT		= "script";
cchar* const EditorUtils::ICON_PREFAB		= "prefab";
cchar* const EditorUtils::ICON_UNKNOWN		= "unknown";
cchar* const EditorUtils::ICON_LOG_INFO		= "info_icon";
cchar* const EditorUtils::ICON_LOG_ERROR	= "error_icon";
cchar* const EditorUtils::ICON_SETTINGS		= "settings";

cchar* const EditorUtils::NAME_FOLDER		= "Folder";
cchar* const EditorUtils::NAME_MATERIAL		= "Material";
cchar* const EditorUtils::NAME_MAT_EMPTY	= "Empty";
cchar* const EditorUtils::NAME_MAT_PS		= "Particles";
cchar* const EditorUtils::NAME_TEXTURE		= "Texture";
cchar* const EditorUtils::NAME_TEX_2D		= "Texture2D";
cchar* const EditorUtils::NAME_TEX_CM		= "CubeMap";
cchar* const EditorUtils::NAME_SHADER		= "Shader";
cchar* const EditorUtils::NAME_MESH			= "Mesh";
cchar* const EditorUtils::NAME_MORPH		= "Morph";
cchar* const EditorUtils::NAME_SCRIPT		= "Script";
cchar* const EditorUtils::NAME_SCENE		= "Scene";
cchar* const EditorUtils::NAME_PREFAB		= "Prefab";

DataMap<String, QIcon*>	EditorUtils::_icons;


void EditorUtils::Initialize()
{
	// load all icons
	QDirIterator dirIterator( EditorSettings::DIR_PATH_ICONS, QDir::Files );
	while ( dirIterator.hasNext() ) {
		dirIterator.next();
		QFileInfo& fileInfo = dirIterator.fileInfo();

		_icons.Set( fileInfo.completeBaseName().toUtf8().data(), 
					Memory::Alloc<QIcon>( fileInfo.filePath().toUtf8().data() ) );
	}
}


void EditorUtils::SetAvailableName( Asset * asset )
{
	// get base name and extension according to type
	cchar* baseName = NULL;
	cchar* extension = NULL;

	switch ( asset->GetAssetType() ) {
		case AssetType::MATERIAL:
			baseName = NAME_MATERIAL;
			extension = FileManager::FILE_FORMAT_MATERIAL;
			break;

		case AssetType::MESH:
			baseName = NAME_MESH;
			extension = FileManager::FILE_FORMAT_MESH;
			break;

		case AssetType::MORPH:
			baseName = NAME_MORPH;
			extension = FileManager::FILE_FORMAT_MORPH;
			break;

		case AssetType::SCENE:
			baseName = NAME_SCENE;
			extension = FileManager::FILE_FORMAT_SCENE;
			break;

		case AssetType::SCRIPT:
			baseName = NAME_SCRIPT;
			extension = FileManager::FILE_FORMAT_SCRIPT;
			break;

		case AssetType::SHADER:
			baseName = NAME_SHADER;
			extension = FileManager::FILE_FORMAT_SHADER;
			break;

		case AssetType::TEXTURE:
			baseName = NAME_TEXTURE;
			extension = FileManager::FILE_FORMAT_TEXTURE;
			break;

		case AssetType::PREFAB:
			baseName = NAME_PREFAB;
			extension = FileManager::FILE_FORMAT_PREFAB;
			break;
	}

	// find new name
	int assetIndex = -1;
	do {
		assetIndex++;
		FiniteString fileName( "%s%d", baseName, assetIndex );
		FiniteString filePath( "%s%s%s", asset->GetDirPath(), fileName.GetChar(), extension );
		asset->SetName( fileName.GetChar() );
	} 
	while ( IsFilePathDuplicate( asset ) );
}


bool EditorUtils::IsFilePathDuplicate( Asset * asset )
{
	AssetManager* assetMgr = AssetManager::Singleton();
	QString name( asset->GetName() );
	QString dirPath( asset->GetDirPath() );

	switch ( asset->GetAssetType() ) {
		case AssetType::TEXTURE:
		{
			for ( int i = 0; i < assetMgr->GetTextureCount(); ++i ) {
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
			for ( int i = 0; i < assetMgr->GetShaderCount(); ++i ) {
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
			for ( int i = 0; i < assetMgr->GetMaterialCount(); ++i ) {
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
			for ( int i = 0; i < assetMgr->GetMeshCount(); ++i ) {
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
			for ( int i = 0; i < assetMgr->GetMorphCount(); ++i ) {
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
			for ( int i = 0; i < assetMgr->GetScriptCount(); ++i )	{
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
			for ( int i = 0; i < assetMgr->GetSceneCount(); ++i ) {
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

		case AssetType::PREFAB:
		{
			for ( int i = 0; i < assetMgr->GetPrefabCount(); ++i ) {
				Prefab* other = assetMgr->GetPrefabAt( i );
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


QIcon* EditorUtils::GetIcon( cchar* iconName )
{
	ASSERT( _icons.Has( iconName ) );
	return _icons.Get( iconName );
}
