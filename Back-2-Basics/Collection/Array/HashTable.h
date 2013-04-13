//
//  HashTable.h
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 9/16/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef OpenSource_HashTable_h
#define OpenSource_HashTable_h

#include <assert.h>
#include <vector>
#include "Strings.h"
#include "Array.h"
#include "Key.h"

/* Need to correct KeyType */

namespace Collection
{
  
  
  /* Class: HashTable */
  template <class ElementType, class KeyType>
  class HashTable
  {
  private:
    //uint64_t testZone[256];
    const uint64_t seed = 0;
    
    std::vector<ElementType> elements;
    std::vector<Key<KeyType> *> keyMap;
    std::vector<uint64_t> fullHashMap;
    
    uint8_t size;
    
    uint64_t murmurHash64A (uint8_t *key, uint64_t size, uint64_t seed);
    uint64_t search(KeyType);
    void grow();
    
  public:
    HashTable();
    HashTable(uint64_t);
    
    
    void remove(KeyType);
    void insert(ElementType, KeyType);
    ElementType get(KeyType);
    void update(KeyType, ElementType);
    //ElementType *clone();
  };
  
  
  /* Class: HashTable */
  
  template <class ElementType, class KeyType>
  ElementType HashTable<ElementType,KeyType>::get(KeyType key)
  {
    uint64_t index = search(key);
    
    assert(index);
    
    return elements[index];
    
  }
  
  template <class ElementType, class KeyType>
  void HashTable<ElementType,KeyType>::update(KeyType key, ElementType data)
  {
    uint64_t index = search(key);
    
    assert(index);
    
    elements[index] = data;
    
  }
  
  
  template <class ElementType, class KeyType>
  HashTable<ElementType,KeyType>::HashTable() : seed(random())
  {
    
    HashTable(1024);
    
  }
  
  template <class ElementType, class KeyType>
  HashTable<ElementType,KeyType>::HashTable(uint64_t size) : seed(random())
  {
    uint8_t actualSize;
    
    if (size > 16)
    {
      actualSize = exp2(log2(size)+1);
    }
    else
    {
      actualSize = 4;
    }
    
    elements.resize(1 << actualSize, 0);
    keyMap.resize(1 << actualSize, (Key<KeyType> *)NULL);
    fullHashMap.resize(1 << actualSize, 0);
    
    this->size = actualSize;
  }
  
  
  template <class ElementType, class KeyType>
  void HashTable<ElementType,KeyType>::grow() {
    uint64_t newIndex;
    uint64_t capacity = 1 << this->size;
    uint64_t newCapacity = 2 << this->size;
    
    elements.resize(newCapacity, 0);
    keyMap.resize(newCapacity, 0);
    fullHashMap.resize(newCapacity, 0);
    
    
    for (uint64_t ix = 1; ix < capacity; ix++)
    {
      if (!fullHashMap[ix])
      {
        continue;
      }
      
      newIndex = fullHashMap[ix] & (newCapacity-1);
      
      if (newIndex == ix)
      {
        continue;
      }
      
      elements[newIndex] = elements[ix];
      keyMap[newIndex] = keyMap[ix];
      fullHashMap[newIndex] = fullHashMap[ix];
      
      elements[ix] = 0;
      keyMap[ix] = 0;
      fullHashMap[ix] = 0;
    }
    
    this->size++;
    
  }
  
  template <class ElementType, class KeyType>
  uint64_t HashTable<ElementType,KeyType>::search(KeyType key)
  {
    uint8_t useHash;
    uint64_t index;
    uint64_t hash;
    uint64_t seed = this->seed;
    uint64_t indexMask = (1 << this->size) - 1;
    
    useHash = 8;
    
    while (useHash)
    {
      useHash--;
      hash = murmurHash64A((uint8_t *)&key, sizeof(key), seed);
      seed = hash;
      index = hash & indexMask;
      if (index)
      {
        if ((keyMap[index] != NULL) &&
            (*(keyMap[index]) == key))
        {
          return index;
        }
        
      }
    }
    
    return 0;
    
  }
  
  template <class ElementType, class KeyType>
  void HashTable<ElementType,KeyType>::remove(KeyType key)
  {
    uint64_t index = search(key);
    
    assert((index) && (keyMap[index] != NULL));
    
    delete keyMap[index];
    
    keyMap[index] = 0;
    fullHashMap[index] = 0;
    elements[index] = 0;
  }
  
  template <class ElementType, class KeyType>
  void HashTable<ElementType,KeyType>::insert(ElementType data, KeyType key)
  {
    uint8_t useHash;
    uint64_t index;
    uint64_t hash;
    uint64_t seed = this->seed;
    uint64_t indexMask = (1 << this->size) - 1;
    
    useHash = 8;
    
    while (useHash)
    {
      useHash--;
      hash = murmurHash64A((uint8_t *)&key, sizeof(key), seed);
      seed = hash;
      index = hash & indexMask;
      
      if (index)
      {
        if (keyMap[index] == NULL)
        {
          keyMap[index] = new Key<KeyType>(key);
          fullHashMap[index] = hash;
          elements[index] = data;
          
          break;
        }
        assert(*(keyMap[index]) == key);
      }
      
      if (!useHash)
      {
        this->grow();
        useHash = 8;
        seed = this->seed;
        indexMask = (1 << this->size) - 1;
      }
      
    }
    
  }
  
  //-----------------------------------------------------------------------------
  // MurmurHash2, 64-bit versions, by Austin Appleby
  // https://sites.google.com/site/murmurhash/
  // The same caveats as 32-bit MurmurHash2 apply here - beware of alignment
  // and endian-ness issues if used across multiple platforms.
  
  // 64-bit hash for 64-bit platforms
  
  template <class ElementType, class KeyType>
  uint64_t HashTable<ElementType,KeyType>::murmurHash64A(uint8_t *key, uint64_t size, uint64_t seed)
  {
    const uint64_t m = 0xc6a4a7935bd1e995;
    const int r = 47;
    uint64_t h;
    
    const uint64_t len = (64 - (size % 64)) + size;
    uint8_t *cond = new uint8_t[len];
    
    memcpy(cond, key, size);
    
    if (len-size)
    {
      memset(cond+size, 0, len-size);
    }
    
    h = seed ^ (len * m);
    
    const uint64_t *data = (uint64_t *)cond;
    const uint64_t *end = data + (len/8);
    
    while(data != end)
    {
      uint64_t k = *data++;
      
      k *= m;
      k ^= k >> r;
      k *= m;
      
      h ^= k;
      h *= m;
    }
    
    switch(len & 7)
    {
      case 7: h ^= data[6] << 48;
      case 6: h ^= data[5] << 40;
      case 5: h ^= data[4] << 32;
      case 4: h ^= data[3] << 24;
      case 3: h ^= data[2] << 16;
      case 2: h ^= data[1] << 8;
      case 1: h ^= data[0];
        h *= m;
    };
    
    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    
    delete cond;
    
    return h;
  }
  
}




#endif
