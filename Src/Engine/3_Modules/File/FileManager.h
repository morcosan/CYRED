// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include "Fragments/FileSystem.h"
#include "Serialize/SerializeSystem.h"



namespace CYRED
{
	ABSTRACT class DLL FileManager
	{
		DECLARE_LOCAL_SINGLETON( FileManager )


	public:
		static const Char* const FILE_FORMAT_MESH;
		static const Char* const FILE_FORMAT_MATERIAL;
		static const Char* const FILE_FORMAT_SHADER;
		static const Char* const FILE_FORMAT_TEXTURE;
		static const Char* const FILE_FORMAT_SCENE;		
		static const Char* const FILE_FORMAT_ASSETDB;		
		


	public:
		virtual void	Initialize	( FileSystem* fileSystem )	PURE_VIRTUAL;
		virtual void	Finalize	()							PURE_VIRTUAL;

		virtual Char*	ReadFile	( const Char* filePath, OUT Int& fileSize )		PURE_VIRTUAL;
		virtual Char*	ReadFile	( const Char* filePath )						PURE_VIRTUAL;
		/* returns TRUE on success */
		virtual Bool	WriteFile	( const Char* filePath, const Char* buffer )	PURE_VIRTUAL;
		/* returns TRUE on success */
		virtual Bool	DeleteFile	( const Char* filePath )						PURE_VIRTUAL;

		virtual UChar*	ReadImage	( const Char* filePath, OUT Int *width, 
									  OUT Int *height, OUT Int *channels )		PURE_VIRTUAL;
		/* returns TRUE on success */
		virtual Bool	WriteImage	( const Char* filePath, const UChar* imageBuffer, 
									  Int width, Int height, Int channels,
									  ImageType type )							PURE_VIRTUAL;

		/* it takes ownership of the new object */
		virtual void SetSerializeSystem( SerializeSystem* serializeSystem )		PURE_VIRTUAL;

		/* requires a SerializeSystem assigned */
		template <class T> String	Serialize	( T* object );
		/* requires a SerializeSystem assigned */
		template <class T> void		Deserialize	( const Char* data, OUT T* object,
												  DeserFlag flag = DeserFlag::FULL );	
	

	protected:
		SerializeSystem*	_serializeSystem;
	};



	template<class T>
	String FileManager::Serialize( T* object )
	{
		ASSERT( _serializeSystem != NULL );

		if ( object == NULL )
		{
			return "";
		}

		return _serializeSystem->Serialize<T>( object );
	}


	template<class T>
	void FileManager::Deserialize( const Char* data, OUT T* object,
								   DeserFlag flag )
	{
		ASSERT( _serializeSystem != NULL );

		if ( data == NULL )
		{
			return;
		}

		_serializeSystem->Deserialize<T>( data, object, flag );
	}
}