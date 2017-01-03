// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"
#include <vector>


namespace CYRED
{
	//! an indexed list of any type
	template <typename TValue>
	class DataArray
	{
	public:
		DataArray();
		DataArray( const DataArray& other );
		virtual ~DataArray();


	public:
		void	operator=	( const DataArray& other );
		TValue&	operator[]	( UInt index )				const;


	public:
		UInt		Add		( TValue value );
		void		Insert	( UInt index, TValue value );
		void		Erase	( UInt index );
		UInt		Size	()					const;

		//! remove all elements
		void		Clear	();

		//! returns a pointer-like list
		const TValue* Data() const;


	protected:
		std::vector<TValue>* _vector;	//! STL vector used under the hood
	};


	template <typename TValue>
	DataArray<TValue>::DataArray()
	{
		_vector = Memory::Alloc<std::vector<TValue>>();
	}


	template <typename TValue>
	DataArray<TValue>::DataArray( const DataArray& other )
	{
		_vector = Memory::Alloc<std::vector<TValue>>();

		for ( UInt i = 0; i < other.Size(); ++i )
		{
			_vector->push_back( (*other._vector)[i] );
		}
	}


	template <typename TValue>
	DataArray<TValue>::~DataArray()
	{
		Memory::Free( _vector );
	}


	template <typename TValue>
	void DataArray<TValue>::operator=( const DataArray& other )
	{
		_vector->clear();

		for ( UInt i = 0; i < other.Size(); ++i )
		{
			_vector->push_back( (*other._vector)[i] );
		}
	}


	template <typename TValue>
	UInt DataArray<TValue>::Add( TValue value )
	{
		_vector->push_back( value );
		return (_vector->size() - 1);
	}


	template<typename TValue>
	void DataArray<TValue>::Insert( UInt index, TValue value )
	{
		ASSERT( index < _vector->size() );
		_vector->insert( _vector->begin() + index, value );
	}


	template <typename TValue>
	void DataArray<TValue>::Erase( UInt index )
	{
		ASSERT( index < _vector->size() );
		_vector->erase( _vector->begin() + index );
	}


	template <typename TValue>
	UInt DataArray<TValue>::Size() const
	{
		return _vector->size();
	}


	template <typename TValue>
	TValue& DataArray<TValue>::operator[] ( UInt index ) const
	{
		ASSERT( index < _vector->size() );
		return (*_vector)[index];
	}


	template<typename TValue>
	const TValue * DataArray<TValue>::Data() const
	{
		return (*_vector).data();
	}


	template <typename TValue>
	void DataArray<TValue>::Clear()
	{
		_vector->clear();
	}
}

