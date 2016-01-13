// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License

#pragma once
#include "../../1_Required/Required.h"
#include <map>
#include "Iterator.h"


namespace CYRED
{
	//! a dictionary of key-value pairs
	template <typename TKey, typename TValue>
	class DataMap
	{
	public:
		DataMap();
		DataMap( const DataMap& other );
		virtual ~DataMap();


	public:
		void operator=( const DataMap& other );


	public:
		//! add a value for the key; it creates the key if doesn't exist
		void		Set		( TKey key, TValue value );

		//! it will fail with error if the key doesn't exist
		//! use Has() before calling this
		TValue	Get		( TKey key )				const;

		Bool		Has		( TKey key )				const;

		//! remove the entry from dictionary; the value is also lost
		void		Erase	( TKey key );

		UInt		Size	()							const;

		//! remove all entries
		void		Clear	();

		//! use the iterator for parsing the ictionary
		Iterator<TKey, TValue>	GetIterator();

		
	protected:
		std::map<TKey, TValue>* _map;
	};



	template <typename TKey, typename TValue>
	DataMap<TKey, TValue>::DataMap()
	{
		_map = Memory::Alloc< std::map<TKey, TValue> >();
	}


	template <typename TKey, typename TValue>
	DataMap<TKey, TValue>::DataMap( const DataMap& other )
	{
		_map = Memory::Alloc< std::map<TKey, TValue> >();

		for ( auto it = other._map->begin(); it != other._map->end(); ++it )
		{
			(*_map)[it->first] = it->second;
		}
	}


	template <typename TKey, typename TValue>
	DataMap<TKey, TValue>::~DataMap()
	{
		Memory::Free( _map );
	}


	template <typename TKey, typename TValue>
	void DataMap<TKey, TValue>::operator=( const DataMap& other )
	{
		_map->clear();

		for ( auto it = other._map->begin(); it != other._map->end(); ++it )
		{
			(*_map)[it->first] = it->second;
		}
	}


	template <typename TKey, typename TValue>
	void DataMap<TKey, TValue>::Set( TKey key, TValue value )
	{
		(*_map)[key] = value;
	}


	template <typename TKey, typename TValue>
	TValue DataMap<TKey, TValue>::Get( TKey key ) const
	{
		auto it = _map->find( key );

		ASSERT( it != _map->end() );

		return it->second;
	}


	template <typename TKey, typename TValue>
	Bool DataMap<TKey, TValue>::Has( TKey key ) const
	{
		return (_map->find( key ) != _map->end());
	}


	template <typename TKey, typename TValue>
	void DataMap<TKey, TValue>::Erase( TKey key )
	{
		ASSERT( (_map->find( key ) != _map->end()) );

		_map->erase( key );
	}


	template <typename TKey, typename TValue>
	UInt DataMap<TKey, TValue>::Size() const
	{
		return _map->size();
	}


	template <typename TKey, typename TValue>
	void DataMap<TKey, TValue>::Clear()
	{
		_map->clear();
	}


	template <typename TKey, typename TValue>
	Iterator<TKey, TValue> DataMap<TKey, TValue>::GetIterator()
	{
		return Iterator<TKey, TValue>( *_map );
	}

}

