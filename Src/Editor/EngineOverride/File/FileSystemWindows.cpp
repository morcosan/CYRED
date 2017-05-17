// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "FileSystemWindows.h"

#include "QtCore\qdir.h"


using namespace CYRED;


bool FileSystemWindows::DeleteDir( const char* dirPath )
{
	QDir directory( dirPath );
	return directory.removeRecursively();
}


bool FileSystemWindows::CreateDir( const char* parentPath, const char* dirName )
{
	QDir directory( parentPath );
	return directory.mkdir( dirName );
}

