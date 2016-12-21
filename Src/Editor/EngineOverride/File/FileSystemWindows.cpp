// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "FileSystemWindows.h"

#include "QtCore\qdir.h"


using namespace CYRED;


Bool FileSystemWindows::DeleteDir( const Char* dirPath )
{
	QDir directory( dirPath );
	return directory.removeRecursively();
}


Bool FileSystemWindows::CreateDir( const Char* parentPath, const Char* dirName )
{
	QDir directory( parentPath );
	return directory.mkdir( dirName );
}

