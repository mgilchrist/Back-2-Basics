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

#include <vector>
#include "Strings.h"
#include "Array.h"
#include "Key.h"

/* Need to correct KeyType */

namespace Collection {
  
  
  /* Class: HashTable */
  template <class ElementType, class KeyType>
  class HashTable
  {
  private:
    //uint64_t testZone[256];
    const uint64_t seed = 0;
    
    ElementType *elements;
    Key<KeyType> **keyMap;
    uint64_t *fullHashMap;
    
    uint64_t size;
    
    uint64_t *murmurHash64A (uint8_t *key, uint64_t size);
    uint64_t search(KeyType);
    void grow();
    
  public:
    HashTable();
    HashTable(uint64_t);
    
    
    int remove(KeyType, ElementType *);
    int insert(ElementType, KeyType);
    int get(KeyType, ElementType *);
    int update(ElementType,KeyType,ElementType *);
    //ElementType *clone();
  };
  
  
  /* Class: HashTable */
  
  template <class ElementType, class KeyType>
  int HashTable<ElementType,KeyType>::get(KeyType key, ElementType *data) {
    uint64_t index = search(key);
    
    if (!index) {
      return -1;
    }
    
    *data = elements[index];
    
    return 0;
  }
  
  template <class ElementType, class KeyType>
  int HashTable<ElementType,KeyType>::update(ElementType data, KeyType key, ElementType *ret) {
    uint64_t index = search(key);
    
    if (index) {
      *ret = elements[index];
      elements[index] = data;
      return 1;
    }
    
    return insert(data, key);
    
  }
  
  
  template <class ElementType, class KeyType>
  HashTable<ElementType,KeyType>::HashTable() {// : seed(random()) {
    
    HashTable(1024);
    
  }
  
  template <class ElementType, class KeyType>
  HashTable<ElementType,KeyType>::HashTable(uint64_t size) { //: seed(random()) {
    
    uint64_t actualSize;
    
    actualSize = exp2(log2(size)+1);
    
    elements = new ElementType[actualSize];
    keyMap = new Key<KeyType>*[actualSize];
    fullHashMap = new uint64_t[actualSize];
    
    for (int ix = 0; ix < actualSize; ix++) {
      elements[ix] = (ElementType)NULL;
      keyMap[ix] = (Key<KeyType> *)NULL;
      fullHashMap[ix] = 0;
    }
    
    this->size = actualSize;
  }
  
  
  template <class ElementType, class KeyType>
  void HashTable<ElementType,KeyType>::grow() {
    ElementType *elementsNew;
    Key<KeyType> **keyMapNew;
    uint64_t *fullHashMapNew;
    uint64_t newIndex;
    
    elementsNew = new ElementType[this->size*2];
    keyMapNew = new Key<KeyType>*[this->size*2];
    fullHashMapNew = new uint64_t[this->size*2];
    
    for (int ix = 1; ix < this->size*2; ix++) {
      elementsNew[ix] = (ElementType)NULL;
      keyMapNew[ix] = (Key<KeyType> *)NULL;
      fullHashMapNew[ix] = 0;
    }
    
    
    for (uint64_t ix = 1; ix < this->size; ix++) {
      if (!fullHashMap[ix]) {
        continue;
      }
      
      newIndex = fullHashMap[ix] % (this->size*2);
      
      elementsNew[newIndex] = elements[ix];
      keyMapNew[newIndex] = keyMap[ix];
      fullHashMapNew[newIndex] = fullHashMap[ix];
    }
    
    delete elements;
    delete keyMap;
    delete fullHashMap;
    
    elements = elementsNew;
    keyMap = keyMapNew;
    fullHashMap = fullHashMapNew;
    
    this->size *= 2;
    
  }
  
  template <class ElementType, class KeyType>
  uint64_t HashTable<ElementType,KeyType>::search(KeyType key) {
    
    uint8_t useHash;
    uint64_t index;
    uint64_t *hash;
    
    
    hash = murmurHash64A((uint8_t *)&key, sizeof(key));
    useHash = 4;
    
    while (useHash) {
      useHash--;
      index = hash[useHash] % this->size;
      if (index) {
        if ((keyMap[index] != NULL) &&
            (keyMap[index]->name.size() == sizeof(key)) &&
            (!memcmp(&(keyMap[index]->name), &key, sizeof(key)))) {
          delete hash;
          return index;
        }
        
      }
    }
    
    delete hash;
    
    return 0;
    
  }
  
  template <class ElementType, class KeyType>
  int HashTable<ElementType,KeyType>::remove(KeyType key, ElementType *ret) {
    uint64_t index = search(key);
    
    if ((index) && (keyMap[index] != NULL)) {
      *ret = elements[index];
      
      delete keyMap[index];
      
      keyMap[index] = NULL;
      fullHashMap[index] = 0;
      elements[index] = NULL;
      
      return 0;
    }
    
    
    return -1;
  }
  
  template <class ElementType, class KeyType>
  int HashTable<ElementType,KeyType>::insert(ElementType data, KeyType key) {
    
    uint8_t useHash;
    uint64_t index;
    uint64_t *hash;
    
    hash = murmurHash64A((uint8_t *)&key, sizeof(key));
    useHash = 4;
    
    while (useHash) {
      useHash--;
      index = hash[useHash] % this->size;
      if (index) {
        if (keyMap[index] == NULL) {
          keyMap[index] = new Key<KeyType>(key);
          fullHashMap[index] = hash[useHash];
          elements[index] = data;
          
          delete hash;
          break;
        }
        if ((keyMap[index]->name.size() == sizeof(key)) &&
            (!memcmp(&(keyMap[index]->name.at(0)), &key, keyMap[index]->name.size()))) {
          delete hash;
          return -1;
        }
      }
      
      if (!useHash) {
        this->grow();
        useHash = 4;
      }
      
    }
    
    return 0;
    
  }
  
  //-----------------------------------------------------------------------------
  // MurmurHash2, 64-bit versions, by Austin Appleby
  // https://sites.google.com/site/murmurhash/
  // The same caveats as 32-bit MurmurHash2 apply here - beware of alignment
  // and endian-ness issues if used across multiple platforms.
  
  // 64-bit hash for 64-bit platforms
  
  template <class ElementType, class KeyType>
  uint64_t *HashTable<ElementType,KeyType>::murmurHash64A(uint8_t *key, uint64_t size)
  {
    const uint64_t m = 0xc6a4a7935bd1e995;
    const int r = 47;
    uint64_t *h = new uint64_t[4];
    
    const uint64_t len = (64 - (size % 64)) + size;
    uint8_t *cond = new uint8_t[len];
    
    memcpy(cond, key, size);
    
    if (len-size) {
      memset(cond+size, 0, len-size);
    }
    
    for (int ix = 0; ix < 4; ix++) {
      if (ix) {
        h[ix] = h[ix-1] ^ (len * m);
      } else {
        h[0] = seed ^ (len * m);
      }
      
      const uint64_t *data = (uint64_t *)cond;
      const uint64_t *end = data + (len/8);
      
      while(data != end)
      {
        uint64_t k = *data++;
        
        k *= m;
        k ^= k >> r;
        k *= m;
        
        h[ix] ^= k;
        h[ix] *= m;
      }
      
      switch(len & 7)
      {
        case 7: h[ix] ^= data[6] << 48;
        case 6: h[ix] ^= data[5] << 40;
        case 5: h[ix] ^= data[4] << 32;
        case 4: h[ix] ^= data[3] << 24;
        case 3: h[ix] ^= data[2] << 16;
        case 2: h[ix] ^= data[1] << 8;
        case 1: h[ix] ^= data[0];
          h[ix] *= m;
      };
      
      h[ix] ^= h[ix] >> r;
      h[ix] *= m;
      h[ix] ^= h[ix] >> r;
    }
    
    delete cond;
    
    return h;
  }
  
}




#endif
