// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "FileSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image\Include\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write\Include\std_image_write.h"

#include <string.h>
#include <fstream>


using namespace CYRED;



char* FileSystem::ReadFile( cchar* filePath, OUT int& fileSize )
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
	char* fileBuffer = new char[fileSize + 1];
	fread( fileBuffer, sizeof(char), fileSize, file );
	fileBuffer[fileSize] = 0;

	// close file
	fclose(file);

	return fileBuffer;
}


bool FileSystem::WriteFile( cchar* filePath, cchar* buffer )
{
	// open file
	FILE* file;
	if ( fopen_s( &file, filePath, "wb" ) != 0 )
	{
		return FALSE;
	}

	int elemWritten = fwrite( buffer, sizeof( char ), strlen( buffer ), file );

	// close file
	fclose(file);

	return (elemWritten == strlen( buffer ));
}


bool FileSystem::DeleteFile( cchar* filePath )
{
	int fail = std::remove( filePath );
	return (fail == 0);
}


bool FileSystem::CopyFile( cchar* srcPath, cchar* dstPath )
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


bool FileSystem::DeleteDir( cchar* dirPath )
{
	return FALSE;
}


bool FileSystem::CreateDir( cchar* parentPath, cchar* dirName )
{
	return FALSE;
}


uchar* FileSystem::ReadImage( cchar* filePath, OUT int* width, OUT int* height, 
							  OUT int* channels )
{
	return stbi_load( filePath, width, height, channels, 0 );
}


bool FileSystem::WriteImage( cchar* filePath, const uchar* imageBuffer, 
								    int width, int height, int channels, ImageType type )
{
	int success = 0; // 0 = fail

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



