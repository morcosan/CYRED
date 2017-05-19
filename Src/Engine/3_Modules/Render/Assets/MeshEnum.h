// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once


namespace CYRED
{
	namespace Enum_MeshType
	{
		enum Enum
		{
			POLYGON
			, LINE
		};
	}

	namespace Enum_MeshLoadType
	{
		enum Enum
		{
			EXTERNAL
			, SCRIPTED
		};
	}

	typedef Enum_MeshType::Enum		MeshType;
	typedef Enum_MeshLoadType::Enum	MeshLoadType;
}