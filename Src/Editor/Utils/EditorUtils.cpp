// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "EditorUtils.h"
#include "CyredBuildingBlocks.h"
#include "CyredModule_Asset.h"
#include "CyredModule_File.h"
#include "CyredModule_Render.h"
#include "CyredModule_Scene.h"
#include "CyredModule_Script.h"

#include "QtCore/QString"


using namespace CYRED;


const char* const EditorUtils::ICON_SCENE		= "scene";
const char* const EditorUtils::ICON_FOLDER		= "folder";
const char* const EditorUtils::ICON_MATERIAL	= "material";
const char* const EditorUtils::ICON_MESH		= "mesh";
const char* const EditorUtils::ICON_MORPH		= "morph";
const char* const EditorUtils::ICON_TEXTURE		= "texture";
const char* const EditorUtils::ICON_SHADER		= "shader";
const char* const EditorUtils::ICON_SCRIPT		= "script";
const char* const EditorUtils::ICON_PREFAB		= "prefab";
const char* const EditorUtils::ICON_UNKNOWN		= "unknown";

const char* const EditorUtils::NAME_FOLDER		= "Folder";
const char* const EditorUtils::NAME_MATERIAL	= "Material";
const char* const EditorUtils::NAME_MAT_EMPTY	= "Empty";
const char* const EditorUtils::NAME_MAT_PS		= "Particles";
const char* const EditorUtils::NAME_TEXTURE		= "Texture";
const char* const EditorUtils::NAME_TEX_2D		= "Texture2D";
const char* const EditorUtils::NAME_TEX_CM		= "CubeMap";
const char* const EditorUtils::NAME_SHADER		= "Shader";
const char* const EditorUtils::NAME_MESH		= "Mesh";
const char* const EditorUtils::NAME_MORPH		= "Morph";
const char* const EditorUtils::NAME_SCRIPT		= "Script";
const char* const EditorUtils::NAME_SCENE		= "Scene";


void EditorUtils::SetAvailableName( Asset * asset )
{
	// get base name and extension according to type
	const char* baseName = NULL;
	const char* extension = NULL;

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
