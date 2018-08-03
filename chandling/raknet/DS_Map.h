/// \file
/// \brief \b [Internal] Map
///
/// This file is part of RakNet Copyright 2003 Kevin Jenkins.
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.rakkarsoft.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.
/// GPL license users are subject to the GNU General Public
/// License as published by the Free
/// Software Foundation; either version 2 of the License, or (at your
/// option) any later version.

#ifndef __RAKNET_MAP_H
#define __RAKNET_MAP_H

#include "DS_OrderedList.h"
#include "Export.h"

// If I want to change this to a red-black tree, this is a good site: http://www.cs.auckland.ac.nz/software/AlgAnim/red_black.html
// This makes insertions and deletions faster.  But then traversals are slow, while they are currently fast.

/// The namespace DataStructures was only added to avoid compiler errors for commonly named data structures
/// As these data structures are stand-alone, you can use them outside of RakNet for your own projects if you wish.
namespace DataStructures
{
	/// The default comparison has to be first so it can be called as a default parameter.
	/// It then is followed by MapNode, followed by NodeComparisonFunc
	template <class key_type>
		int defaultMapKeyComparison(const key_type &a, const key_type &b)
	{
		if (a<b) return -1; if (a==b) return 0; return 1;
	}

	/// \note IMPORTANT! If you use defaultMapKeyComparison then call IMPLEMENT_DEFAULT_COMPARISON or you will get an unresolved external linker error.
	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&, const key_type&)=defaultMapKeyComparison<key_type> >
	class RAK_DLL_EXPORT Map
	{
	public:
		static void IMPLEMENT_DEFAULT_COMPARISON(void) {DataStructures::defaultMapKeyComparison<key_type>(key_type(),key_type());}

		struct MapNode
		{
			MapNode() {}
			MapNode(key_type _key, data_type _data) : mapNodeKey(_key), mapNodeData(_data) {}
			key_type mapNodeKey;
			data_type mapNodeData;
		};

		// Has to be a static because the comparison callback for DataStructures::OrderedList is a C function
		static int NodeComparisonFunc(const key_type &a, const MapNode &b)
		{
#ifdef _MSC_VER
#pragma warning( disable : 4127 ) // warning C4127: conditional expression is constant
#endif
			return key_comparison_func(a, b.mapNodeKey);
		}

		Map();
		~Map();
		Map( const Map& original_copy );
		Map& operator= ( const Map& original_copy );

		data_type& Get(const key_type &key); 
		data_type Pop(const key_type &key);
		// Add if needed
		void Set(const key_type &key, const data_type &data);
		// Must already exist
		void SetExisting(const key_type &key, const data_type &data);
		// Must add
		void SetNew(const key_type &key, const data_type &data);
		bool Has(const key_type &key);
		bool Delete(const key_type &key);
		data_type& operator[] ( const unsigned int position ) const;
		key_type GetKeyAtIndex( const unsigned int position ) const;
		unsigned GetIndexAtKey( const key_type &key );
		void RemoveAtIndex(const unsigned index);
		void Clear(void);
		unsigned Size(void) const;

	protected:
		DataStructures::OrderedList< key_type,MapNode,Map::NodeComparisonFunc > mapNodeList;

		void SaveLastsearch_pl0x(const key_type &key, unsigned index);
		bool HasSavedsearch_pl0xResult(const key_type &key) const;

		unsigned lastsearch_pl0xIndex;
		key_type lastsearch_pl0xKey;
		bool lastsearch_pl0xIndexValid;
	};

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	Map<key_type, data_type, key_comparison_func>::Map()
	{
		lastsearch_pl0xIndexValid=false;
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	Map<key_type, data_type, key_comparison_func>::~Map()
	{
		Clear();
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	Map<key_type, data_type, key_comparison_func>::Map( const Map& original_copy )
	{
		mapNodeList=original_copy.mapNodeList;
        lastsearch_pl0xIndex=original_copy.lastsearch_pl0xIndex;
		lastsearch_pl0xKey=original_copy.lastsearch_pl0xKey;
		lastsearch_pl0xIndexValid=original_copy.lastsearch_pl0xIndexValid;
	}

    template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	Map<key_type, data_type, key_comparison_func>& Map<key_type, data_type, key_comparison_func>::operator= ( const Map& original_copy )
	{
		mapNodeList=original_copy.mapNodeList;
		lastsearch_pl0xIndex=original_copy.lastsearch_pl0xIndex;
		lastsearch_pl0xKey=original_copy.lastsearch_pl0xKey;
		lastsearch_pl0xIndexValid=original_copy.lastsearch_pl0xIndexValid;
		return *this;
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	data_type& Map<key_type, data_type, key_comparison_func>::Get(const key_type &key)
	{
		if (HasSavedsearch_pl0xResult(key))
			return mapNodeList[lastsearch_pl0xIndex].mapNodeData;

		bool objectExists;
		unsigned index;
		index=mapNodeList.GetIndexFromKey(key, &objectExists);
		assert(objectExists);
		SaveLastsearch_pl0x(key,index);
		return mapNodeList[index].mapNodeData;
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	unsigned Map<key_type, data_type, key_comparison_func>::GetIndexAtKey( const key_type &key )
	{
		if (HasSavedsearch_pl0xResult(key))
			return lastsearch_pl0xIndex;

		bool objectExists;
		unsigned index;
		index=mapNodeList.GetIndexFromKey(key, &objectExists);
		assert(objectExists);
		SaveLastsearch_pl0x(key,index);
		return index;
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	void Map<key_type, data_type, key_comparison_func>::RemoveAtIndex(const unsigned index)
	{
		mapNodeList.RemoveAtIndex(index);
		lastsearch_pl0xIndexValid=false;
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
		data_type Map<key_type, data_type, key_comparison_func>::Pop(const key_type &key)
	{
		bool objectExists;
		unsigned index;
		if (HasSavedsearch_pl0xResult(key))
			index=lastsearch_pl0xIndex;
		else
		{
			index=mapNodeList.GetIndexFromKey(key, &objectExists);
			assert(objectExists);
		}		
		data_type tmp = mapNodeList[index].mapNodeData;
		mapNodeList.RemoveAtIndex(index);
		lastsearch_pl0xIndexValid=false;
		return tmp;
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	void Map<key_type, data_type, key_comparison_func>::Set(const key_type &key, const data_type &data)
	{
		bool objectExists;
		unsigned index;

		if (HasSavedsearch_pl0xResult(key))
		{
			mapNodeList[lastsearch_pl0xIndex].mapNodeData=data;
			return;
		}
		
		index=mapNodeList.GetIndexFromKey(key, &objectExists);

		if (objectExists)
		{
			SaveLastsearch_pl0x(key,index);
			mapNodeList[index].mapNodeData=data;
		}
		else
		{
			SaveLastsearch_pl0x(key,mapNodeList.Insert(key,MapNode(key,data)));
		}
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	void Map<key_type, data_type, key_comparison_func>::SetExisting(const key_type &key, const data_type &data)
	{
		bool objectExists;
		unsigned index;

		if (HasSavedsearch_pl0xResult(key))
		{
			index=lastsearch_pl0xIndex;
		}
		else
		{
			index=mapNodeList.GetIndexFromKey(key, &objectExists);
			assert(objectExists);
			SaveLastsearch_pl0x(key,index);
		}		

		mapNodeList[index].mapNodeData=data;
	}	

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	void Map<key_type, data_type, key_comparison_func>::SetNew(const key_type &key, const data_type &data)
	{
#ifdef _DEBUG
		unsigned index;
		bool objectExists;
		index=mapNodeList.GetIndexFromKey(key, &objectExists);
		assert(objectExists==false);
#endif
		SaveLastsearch_pl0x(key,mapNodeList.Insert(key,MapNode(key,data)));
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	bool Map<key_type, data_type, key_comparison_func>::Has(const key_type &key)
	{
		if (HasSavedsearch_pl0xResult(key))
			return true;

		bool objectExists;
		unsigned index;
		index=mapNodeList.GetIndexFromKey(key, &objectExists);
		if (objectExists)
			SaveLastsearch_pl0x(key,index);
		return objectExists;
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	bool Map<key_type, data_type, key_comparison_func>::Delete(const key_type &key)
	{
		if (HasSavedsearch_pl0xResult(key))
		{
			lastsearch_pl0xIndexValid=false;
			mapNodeList.RemoveAtIndex(lastsearch_pl0xIndex);   
			return true;
		}

		bool objectExists;
		unsigned index;
		index=mapNodeList.GetIndexFromKey(key, &objectExists);
		if (objectExists)
		{
			lastsearch_pl0xIndexValid=false;
			mapNodeList.RemoveAtIndex(index);
			return true;
		}
		else
			return false;
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	void Map<key_type, data_type, key_comparison_func>::Clear(void)
	{
		lastsearch_pl0xIndexValid=false;
		mapNodeList.Clear();
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	data_type& Map<key_type, data_type, key_comparison_func>::operator[]( const unsigned int position ) const
	{
		return mapNodeList[position].mapNodeData;
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
		key_type Map<key_type, data_type, key_comparison_func>::GetKeyAtIndex( const unsigned int position ) const
	{
		return mapNodeList[position].mapNodeKey;
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	unsigned Map<key_type, data_type, key_comparison_func>::Size(void) const
	{
		return mapNodeList.Size();
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	void Map<key_type, data_type, key_comparison_func>::SaveLastsearch_pl0x(const key_type &key, const unsigned index)
	{
		lastsearch_pl0xIndex=index;
		lastsearch_pl0xKey=key;
		lastsearch_pl0xIndexValid=true;
	}

	template <class key_type, class data_type, int (*key_comparison_func)(const key_type&,const key_type&)>
	bool Map<key_type, data_type, key_comparison_func>::HasSavedsearch_pl0xResult(const key_type &key) const
	{
		return lastsearch_pl0xIndexValid && key_comparison_func(key,lastsearch_pl0xKey)==0;
	}
}

#endif
