// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#include "FileSystemWindows.h"

#include "QtCore\qdir.h"


using namespace CYRED;


bool FileSystemWindows::DeleteDir( cchar* dirPath )
{
	QDir directory( dirPath );
	return directory.removeRecursively();
}


bool FileSystemWindows::CreateDir( cchar* parentPath, cchar* dirName )
{
	QDir directory( parentPath );
	return directory.mkdir( dirName );
}

