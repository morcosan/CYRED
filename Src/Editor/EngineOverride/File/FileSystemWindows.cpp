// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "FileSystemWindows.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image\Include\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write\Include\std_image_write.h"

#include <cstring>
#include <fstream>


using namespace CYRED;



Char* FileSystemWindows::ReadFile( const Char* filePath, OUT Int& fileSize )
{
	// open file
	FILE* file;
	if ( fopen_s( &file, filePath, "rb" ) != 0 )
	{
		return NULL;
	}

	// get file size
	fseek( file, 0, SEEK_END );
	fileSize = ftell( file );
	fseek( file, 0, SEEK_SET );

	// read file data
	Char* fileBuffer = Memory::AllocArray<Char>( fileSize + 1 );
	fread( fileBuffer, sizeof(Char), fileSize, file );
	fileBuffer[fileSize] = 0;

	// close file
	fclose(file);

	return fileBuffer;
}


Bool FileSystemWindows::WriteFile( const Char* filePath, const Char* buffer )
{
	// open file
	FILE* file;
	if ( fopen_s( &file, filePath, "wb" ) != 0 )
	{
		return FALSE;
	}

	UInt elemWritten = fwrite( buffer, sizeof( Char ), strlen( buffer ), file );

	// close file
	fclose(file);

	return (elemWritten == strlen( buffer ));
}


Bool FileSystemWindows::DeleteFile( const Char* filePath )
{
	Int fail = std::remove( filePath );
	return (fail == 0);
}


Bool FileSystemWindows::CopyFile( const Char* srcPath, const Char* dstPath )
{
	// try to open src
	std::ifstream ifs( srcPath, std::ios::binary );
	// check if src exists
	if ( !ifs.good() ) {
		return FALSE;
	}

	// try to open dst
	std::ofstream ofs( dstPath, std::ios::binary );
	// check if dest can be opened
	if ( !ofs.good() ) {
		return FALSE;
	}

	// copy content
	ofs << ifs.rdbuf();

	// all good
	return TRUE;
}


UChar* FileSystemWindows::ReadImage( const Char* filePath, OUT Int* width, OUT Int* height, 
									 OUT Int* channels )
{
	return stbi_load( filePath, width, height, channels, 0 );
}


Bool FileSystemWindows::WriteImage( const Char* filePath, const UChar* imageBuffer, 
								    Int width, Int height, Int channels, ImageType type )
{
	Int success = 0; // 0 = fail

	switch ( type )
	{
		case ImageType::BMP:
			success = stbi_write_bmp( filePath, width, height, channels, imageBuffer );
			break;

		case ImageType::PNG:
			success = stbi_write_png( filePath, width, height, channels, imageBuffer, 0 );
			break;
	}

	return (success > 0);
}



