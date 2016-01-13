// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#include "SampleController.h"

#include "CyredEngine.h"


int main( int argc, char *argv[] )
{
	CYRED::Standalone app( argc, argv, Memory::Alloc<SampleController>() );

	return 0;
}