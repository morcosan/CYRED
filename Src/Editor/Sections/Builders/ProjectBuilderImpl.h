// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "ProjectBuilder.h"


namespace CYRED
{
	class Asset;
	class Texture;
	class Scene;
	class Material;
	class Mesh;
	class Morph;
	class Shader;
	class Script;
}


namespace CYRED
{
	class ProjectBuilderImpl : public ProjectBuilder
	{
		DECLARE_LOCAL_SINGLETON_IMPL( ProjectBuilderImpl )


	public:
		virtual void Initialize	()	override;
		virtual void Finalize	()	override;

		virtual void BuildWindows( cchar* buildPath )	override;
		virtual void BuildAndroid( cchar* buildPath )	override;


	private:
		void _BuildTextureFile	( Texture* asset );
		void _BuildSceneFile	( Scene* asset );
		void _BuildMaterialFile	( Material* asset );
		void _BuildMeshFile		( Mesh* asset );
		void _BuildMorphFile	( Morph* asset );
		void _BuildShaderFile	( Shader* asset );
		void _BuildScriptFile	( Script* asset );
	};
}