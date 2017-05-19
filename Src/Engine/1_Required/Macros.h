// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once

//#define NDEBUG // this will disable all asserts
#include <assert.h>

#define ASSERT(expr)				assert(expr)	
#define STATIC_ASSERT(expr, msg)	static_assert(expr, msg)


#include <typeinfo>
#include <typeindex>

#define	TYPE_INDEX				std::type_index
#define GET_TYPE_INDEX(Class)	std::type_index( typeid(Class) )


#include <type_traits>
#define IS_BASE_OF(ClassA, ClassB)	std::is_base_of<ClassA, ClassB>::value


#define CAST_S( Type, value )	static_cast<Type>( value )
#define CAST_D( Type, value )	dynamic_cast<Type>( value )
#define NO_CONST( Type, value )	const_cast<Type>( value )

#define ARRAY_SIZE( value )		sizeof( value ) / sizeof( value[0] );

#ifdef NULL
	#undef NULL
#endif
#define NULL nullptr


typedef unsigned char	uchar;
typedef unsigned int	uint;


#define TRUE	true
#define FALSE	false

#define PURE_VIRTUAL		=0

#define INVALID_INDEX		-1
#define INVALID_CANVAS_SLOT -1
#define EMPTY_BUFFER		0
#define EMPTY_SHADER		0
#define INVALID_SHADER		0
#define INVALID_UNIFORM		-1
#define INVALID_TEXTURE		0
#define EMPTY_OBJECT_UID	0

#define INPUT_FIRST_TIME_DOWN	0
#define INPUT_CONTINUOUS_DOWN	1
#define INPUT_FIRST_TIME_UP		2
#define INPUT_CONTINUOUS_UP		3
#define INPUT_INVALID_WINDOW	-1


#ifdef CREATE_DLL
#define DLL __declspec(dllexport) 
#else
#define DLL __declspec(dllimport)
#endif


// disable the dll warning
#pragma warning( disable: 4251 )