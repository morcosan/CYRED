// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#include "ConsoleWindows.h"

#include <iostream>
#include <cstdio>


using namespace CYRED;


void ConsoleWindows::Log( const Char* text )
{
	std::cout << text << std::endl;
}
