// Copyright (c) 2015 Morco (www.morco.ro)
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
}


namespace CYRED
{
	class ProjectBuilderImpl : public ProjectBuilder
	{
		DECLARE_LOCAL_SINGLETON_IMPL( ProjectBuilderImpl )


	public:
		virtual void Initialize	()	override;
		virtual void Finalize	()	override;

		virtual void BuildWindows( const Char* buildPath )	override;
		virtual void BuildAndroid( const Char* buildPath )	override;


	protected:
		void _BuildTextureFile	( Texture* asset );
		void _BuildSceneFile	( Scene* asset );
		void _BuildMaterialFile	( Material* asset );
		void _BuildMeshFile		( Mesh* asset );
		void _BuildMorphFile	( Morph* asset );
		void _BuildShaderFile	( Shader* asset );
	};
}