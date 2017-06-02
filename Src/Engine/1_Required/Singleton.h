// Copyright (c) 2015-2017 Morco (www.morco.ro)
// MIT License

#pragma once
#include "Memory.h"

//! these macros are used to create singleton classes

//! there are 2 type: local and remote
//! local singleton		= declared and defined inside the DLL
//! remote singleton	= declared in DLL, defined outside (deferred definition)

//! singletons must use Bridge pattern


#define DECLARE_LOCAL_SINGLETON( Class ) \
	public: \
		static void		CreateSingleton(); \
		static Class*	Singleton(); \
		static void		DestroySingleton(); \
		\
		Class() {} \
		virtual ~Class() {}


#define DEFINE_LOCAL_SINGLETON( Class, ClassImpl ) \
	void Class::CreateSingleton() \
	{ \
		ClassImpl::CreateSingleton(); \
	} \
	Class* Class::Singleton() \
	{ \
		return ClassImpl::Singleton(); \
	} \
	void Class::DestroySingleton() \
	{ \
		ClassImpl::DestroySingleton(); \
	}


#define DECLARE_LOCAL_SINGLETON_IMPL( ClassImpl ) \
	public: \
		static void			CreateSingleton(); \
		static ClassImpl*	Singleton(); \
		static void			DestroySingleton(); \
		\
		ClassImpl() : _isInitialized( false ) {} \
		virtual ~ClassImpl() {} \
	private: \
		static ClassImpl* _singleton; \
		bool _isInitialized;


#define DEFINE_LOCAL_SINGLETON_IMPL( ClassImpl ) \
	ClassImpl* ClassImpl::_singleton = NULL; \
	\
	void ClassImpl::CreateSingleton() \
	{ \
		_singleton = new ClassImpl(); \
	} \
	ClassImpl* ClassImpl::Singleton() \
	{ \
		return _singleton; \
	} \
	void ClassImpl::DestroySingleton() \
	{ \
		PTR_FREE( _singleton ); \
		_singleton = NULL; \
	}



#define DECLARE_REMOTE_SINGLETON( Class ) \
	public: \
		static void		CreateSingleton( Class* impl ); \
		static Class*	Singleton(); \
		static void		DestroySingleton(); \
		\
		Class() {} \
		virtual ~Class() {} \
	private: \
		static Class* _singleton;
		


#define DEFINE_REMOTE_SINGLETON( Class ) \
	Class* Class::_singleton = NULL; \
	\
	void Class::CreateSingleton( Class* impl ) \
	{ \
		_singleton = impl; \
	} \
	Class* Class::Singleton() \
	{ \
		return _singleton; \
	} \
	void Class::DestroySingleton() \
	{ \
		PTR_FREE( _singleton ); \
		_singleton = NULL; \
	}


#define DECLARE_REMOTE_SINGLETON_IMPL( ClassImpl ) \
	public: \
		static ClassImpl*	CreateSingleton(); \
		static ClassImpl*	Singleton(); \
		\
		ClassImpl() {} \
		virtual ~ClassImpl() {} \
	private: \
		static ClassImpl* _singleton;



#define DEFINE_REMOTE_SINGLETON_IMPL( ClassImpl ) \
	ClassImpl* ClassImpl::_singleton = NULL; \
	\
	ClassImpl* ClassImpl::CreateSingleton() \
	{ \
		_singleton = new ClassImpl(); \
		return _singleton; \
	} \
	ClassImpl* ClassImpl::Singleton() \
	{ \
		return _singleton; \
	} 