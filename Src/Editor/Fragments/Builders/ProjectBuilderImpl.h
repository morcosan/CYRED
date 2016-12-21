// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "ProjectBuilder.h"


namespace CYRED
{
	class Asset;
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
		void _BuildAssetFiles( Asset* asset );
	};
}