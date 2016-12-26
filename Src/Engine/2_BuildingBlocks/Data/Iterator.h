// Copyright (c) 2015 Morco (www.morco.ro)
// MIT License


#pragma once
#include "../../1_Required/Required.h"
#include <map>


namespace CYRED
{
	template <typename TKey, typename TValue>
	class Iterator
	{
	public:
		Iterator( std::map<TKey, TValue>& map );
		virtual ~Iterator() {}


		Bool			HasNext()	const;
		const TKey&		GetKey()	const;
		const TValue&	GetValue()	const;
		void			Next();


	protected:
		typename std::map<TKey, TValue>::iterator _curr;
		typename std::map<TKey, TValue>::iterator _end;
	};




	template <typename TKey, typename TValue>
	Iterator<TKey, TValue>::Iterator( std::map<TKey, TValue>& map )
		: _curr( map.begin() ), _end( map.end() )
	{
	}


	template <typename TKey, typename TValue>
	Bool Iterator<TKey, TValue>::HasNext() const
	{
		return (_curr != _end);
	}


	template <typename TKey, typename TValue>
	const TKey& Iterator<TKey, TValue>::GetKey() const
	{
		return _curr->first;
	}


	template <typename TKey, typename TValue>
	const TValue& Iterator<TKey, TValue>::GetValue() const
	{
		return _curr->second;
	}


	template <typename TKey, typename TValue>
	void Iterator<TKey, TValue>::Next()
	{
		_curr++;
	}

}