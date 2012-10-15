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
  
  /* First 64 bits of fractional part of the
   square root of {2,3,5,7,11,13,17,19} */
  
  const uint64_t sha_2_h[8] =
  {
    0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
    0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
  };
  
  /* First 64 bits of fractional part of the
   cube root of first 64 primes */
  
  const uint64_t sha_2_k[80] =
  {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
    0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
    0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
    0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
    0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
    0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
    0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
    0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
    0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
    0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
    0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
    0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
  };
  
  
  /* Class: HashTable */
  template <class ElementType, class KeyType>
  class HashTable
  {
  private:
    std::vector<ElementType> collection;
    std::vector<Key<KeyType> *> keyMap;
    std::vector<std::vector<uint64_t> *> keyMapSecure;
    std::vector<uint64_t> fullHashMap;
    const uint64_t seed;
    const bool secure;
    
    uint64_t *murmurHash64A (Key<KeyType> *key);
    std::vector<uint64_t> *SHA_2_Prep(std::vector<uint8_t> &data);
    uint64_t *SHA_2(Key<KeyType> *);
    uint64_t search(KeyType&);
    void grow();
    
  public:
    HashTable();
    HashTable(uint64_t);
    HashTable(uint64_t,bool);
    
    
    ElementType remove(KeyType &key);
    void insert(ElementType, KeyType&);
    ElementType get(KeyType&);
    ElementType update(ElementType,KeyType &);
    //ElementType *clone();
  };
  
  
  /* Class: HashTable */
  
  template <class ElementType, class KeyType>
  ElementType HashTable<ElementType,KeyType>::get(KeyType &key) {
    return collection[search(key)];
  }
  
  template <class ElementType, class KeyType>
  ElementType HashTable<ElementType,KeyType>::update(ElementType data, KeyType &key) {
    uint64_t index = search(key);
    ElementType ret = NULL;
    
    if (index) {
      ret = collection[index];
      collection[index] = data;
    } else {
      insert(data, key);
    }
    
    return ret;
  }
  
  
  template <class ElementType, class KeyType>
  HashTable<ElementType,KeyType>::HashTable() : seed(random()), secure(false) {
    
    collection.resize(1024, 0);
    keyMap.resize(1024, 0);
    fullHashMap.resize(1024, 0);
    
  }
  
  template <class ElementType, class KeyType>
  HashTable<ElementType,KeyType>::HashTable(uint64_t size) : seed(random()), secure(false) {
    
    uint64_t actualSize;
    
    actualSize = exp2(log2(size)+1);
    
    collection.resize(actualSize, 0);
    keyMap.resize(actualSize, 0);
    fullHashMap.resize(actualSize, 0);
  }
  
  template <class ElementType, class KeyType>
  HashTable<ElementType,KeyType>::HashTable(uint64_t size, bool secure) : seed(random()), secure(secure) {
    
    uint64_t actualSize;
    
    actualSize = exp2(log2(size)+1);
    
    collection.resize(actualSize, 0);
    if (secure) {
      keyMapSecure.resize(actualSize, 0);
    } else {
      keyMap.resize(actualSize, 0);
    }
    
    fullHashMap.resize(actualSize, 0);
    
  }
  
  template <class ElementType, class KeyType>
  void HashTable<ElementType,KeyType>::grow() {
    
    uint64_t newIndex;
    uint64_t oldSize = fullHashMap.size();
    
    collection.resize(collection.size()*2, 0);
    if (secure) {
      keyMapSecure.resize(keyMapSecure.size()*2, 0);
    } else {
      keyMap.resize(keyMap.size()*2, 0);
    }
    
    fullHashMap.resize(fullHashMap.size()*2, 0);
    
    for (uint64_t ix = 1; ix < oldSize; ix++) {
      if (!fullHashMap[ix]) {
        continue;
      }
      
      newIndex = fullHashMap[ix] % fullHashMap.size();
      
      if (ix != newIndex) {
        collection[newIndex] = collection[ix];
        if (secure) {
          keyMapSecure[newIndex] = keyMapSecure[ix];
          keyMapSecure[ix] = NULL;
        } else {
          keyMap[newIndex] = keyMap[ix];
          keyMap[ix] = (Key<KeyType> *)NULL;
        }
        fullHashMap[newIndex] = fullHashMap[ix];
        collection[ix] = (ElementType)NULL;
        fullHashMap[ix] = 0;
      }
    }
    
    
  }
  
  template <class ElementType, class KeyType>
  uint64_t HashTable<ElementType,KeyType>::search(KeyType &key) {
    
    uint8_t useHash;
    uint64_t index;
    Key<KeyType> *kKey = new Key<KeyType>(key);
    uint64_t *hash;
    
    if (secure) {
      hash = SHA_2(kKey);
      useHash = 8;
    } else {
      hash = murmurHash64A(kKey);
      useHash = 4;
    }
    
    while (useHash) {
      useHash--;
      index = hash[useHash] % collection.size();
      if (index) {
        if (secure) {
          if ((keyMapSecure[index] != NULL) && (!memcmp(&(keyMapSecure[index]->at(0)), hash, 8))) {
            delete hash;
            return index;
          }
        } else {
          if ((keyMap[index] != NULL) && (*(keyMap[index]) == key)) {
            delete hash;
            return index;
          }
        }
      }
    }
    
    delete kKey;
    delete hash;
    
    return 0;
    
  }
  
  template <class ElementType, class KeyType>
  ElementType HashTable<ElementType,KeyType>::remove(KeyType &key) {
    uint64_t index = search(key);
    ElementType data = NULL;
    
    if (secure) {
      if ((index) && (keyMapSecure[index] != NULL)) {
        data = collection[index];
        
        delete keyMapSecure[index];
        
        keyMapSecure[index] = NULL;
        fullHashMap[index] = 0;
        collection[index] = NULL;
      }
    } else {
      if ((index) && (keyMap[index] != NULL)) {
        data = collection[index];
        
        delete keyMap[index];
        
        keyMap[index] = NULL;
        fullHashMap[index] = 0;
        collection[index] = NULL;
      }
    }
    
    return data;
  }
  
  template <class ElementType, class KeyType>
  void HashTable<ElementType,KeyType>::insert(ElementType data, KeyType &key) {
    
    uint8_t useHash;
    uint64_t index;
    Key<KeyType> *kKey = new Key<KeyType>(key);
    uint64_t *hash;
    
    if (secure) {
      hash = SHA_2(kKey);
      useHash = 8;
    } else {
      hash = murmurHash64A(kKey);
      useHash = 4;
    }
    
    do {
      while (useHash) {
        useHash--;
        index = hash[useHash] % collection.size();
        if (index) {
          if (secure) {
            if (keyMapSecure[index] == NULL) {
              keyMapSecure[index] = new std::vector<uint64_t>(8);
              keyMapSecure[index]->assign(hash, hash+8);
              fullHashMap[index] = hash[useHash];
              collection[index] = data;
              
              delete kKey;
              delete hash;
              return;
            } else if (!memcmp(&(keyMapSecure[index]->at(0)), hash, 8)) {
              delete kKey;
              delete hash;
              return;
            }
          } else {
            if (keyMap[index] == NULL) {
              keyMap[index] = kKey;
              fullHashMap[index] = hash[useHash];
              collection[index] = data;
              
              delete hash;
              return;
            } else if (*keyMap[index] == *kKey) {
              delete hash;
              delete kKey;
              return;
            }
          }
        }
      }
      
      this->grow();
      
      if (secure) {
        useHash = 8;
      } else {
        useHash = 4;
      }
      
    } while ((1));
    
  }
  
  //-----------------------------------------------------------------------------
  // MurmurHash2, 64-bit versions, by Austin Appleby
  // https://sites.google.com/site/murmurhash/
  // The same caveats as 32-bit MurmurHash2 apply here - beware of alignment
  // and endian-ness issues if used across multiple platforms.
  
  // 64-bit hash for 64-bit platforms
  
  template <class ElementType, class KeyType>
  uint64_t *HashTable<ElementType,KeyType>::murmurHash64A(Key<KeyType> *key)
  {
    const uint64_t m = 0xc6a4a7935bd1e995;
    const int r = 47;
    const uint64_t len = key->name.size();
    uint64_t *h = new uint64_t(8);
    const uint64_t * data = (const uint64_t *)(&key->name.at(0));
    const uint64_t * end = data + (len/8);
    
    for (int ix = 0; ix < 4; ix++) {
      if (ix) {
        h[ix] = h[ix-1] ^ (len * m);
      } else {
        h[0] = seed ^ (len * m);
      }
      
      while(data != end)
      {
        uint64_t k = *data++;
        
        k *= m;
        k ^= k >> r;
        k *= m;
        
        h[ix] ^= k;
        h[ix] *= m;
      }
      
      const unsigned char * data2 = (const unsigned char*)data;
      
      switch(len & 7)
      {
        case 7: h[ix] ^= uint64_t(data2[6]) << 48;
        case 6: h[ix] ^= uint64_t(data2[5]) << 40;
        case 5: h[ix] ^= uint64_t(data2[4]) << 32;
        case 4: h[ix] ^= uint64_t(data2[3]) << 24;
        case 3: h[ix] ^= uint64_t(data2[2]) << 16;
        case 2: h[ix] ^= uint64_t(data2[1]) << 8;
        case 1: h[ix] ^= uint64_t(data2[0]);
          h[ix] *= m;
      };
      
      h[ix] ^= h[ix] >> r;
      h[ix] *= m;
      h[ix] ^= h[ix] >> r;
    }
    
    return h;
  }
  
  template <class ElementType, class KeyType>
  std::vector<uint64_t> *HashTable<ElementType,KeyType>::SHA_2_Prep(std::vector<uint8_t> &data) {
    uint64_t dataSize = data.size()+8;
    uint64_t arraySize = (128 - (dataSize % 128) % 111) + dataSize;
    std::vector<uint8_t> *ret = new std::vector<uint8_t>(data.begin(), data.end());
    
    ret->resize(arraySize, 0);
    *((uint64_t *)(&(*ret)[dataSize-8])) = seed;
    (*ret)[dataSize] = 0x01;
    
    *((uint64_t *)(&(*ret)[arraySize-8])) = dataSize+8;
    
    return (std::vector<uint64_t> *)ret;
  }
  
  template <class ElementType, class KeyType>
  uint64_t *HashTable<ElementType,KeyType>::SHA_2(Key<KeyType> *key) {
    std::vector<uint64_t> *conditionedMsg;
    uint64_t subArray[80];
    uint64_t tmpHash[8];
    uint64_t *hash;
    uint64_t tmpInt0, tmpInt1;
    uint64_t tap0, tap1, majority, change;
    
    hash = new uint64_t[8];
    
    
    conditionedMsg = SHA_2_Prep(key->name);
    
    for (int jx = 0; jx < 8; jx++) {
      hash[jx] = 0;
    }
    
    for (int ix = 0; ix < conditionedMsg->size(); ix += 16) {
      for (int jx = 0; jx < 16; jx++) {
        subArray[jx] = conditionedMsg->at(ix+jx);
      }
      
      for (int jx = 16; jx < 80; jx++) {
        /* 4.6 or 4.12 */
        tmpInt0 = (((subArray[jx-15] >> 1) | (subArray[jx-15] << 63)) ^
                   ((subArray[jx-15] >> 8) | (subArray[jx-15] << 56)) ^
                   ((subArray[jx-15] >> 7)));
        /* 4.7 or 4.13 */
        tmpInt1 = (((subArray[jx-2] >> 19) | (subArray[jx-2] << 45)) ^
                   ((subArray[jx-2] >> 61) | (subArray[jx-2] << 3)) ^
                   ((subArray[jx-2] >> 6)));
        subArray[jx] = subArray[jx-16] + tmpInt0 + subArray[jx-7] + tmpInt1;
      }
      
      for (int jx = 0; jx < 8; jx++) {
        tmpHash[jx] = sha_2_h[jx];
      }
      
      for (int jx = 0; jx < 80; jx++) {
        /* 4.4 and 4.10 */
        tap0 = (((tmpHash[0] >> 28) | (tmpHash[0] << 36)) ^
                ((tmpHash[0] >> 34) | (tmpHash[0] << 30)) ^
                ((tmpHash[0] >> 39) | (tmpHash[0] << 25)));
        majority = ((tmpHash[0] & tmpHash[1]) ^
                    (tmpHash[0] & tmpHash[2]) ^
                    (tmpHash[1] & tmpHash[2]));
        tmpInt0 = tap0 + majority;
        /* 4.5 or 4.11 */
        tap1 = (((tmpHash[4] >> 14) | (tmpHash[4] << 50)) ^
                ((tmpHash[4] >> 18) | (tmpHash[4] << 46)) ^
                ((tmpHash[4] >> 41) | (tmpHash[4] << 23)));
        change = ((tmpHash[4] & tmpHash[5]) ^
                  ((~tmpHash[4]) & tmpHash[6]));
        tmpInt1 = tmpHash[7] + tap1 + change + sha_2_k[jx] + subArray[jx];
        
        tmpHash[7] = tmpHash[6];
        tmpHash[6] = tmpHash[5];
        tmpHash[5] = tmpHash[4];
        tmpHash[4] = tmpHash[3] + tmpInt1;
        tmpHash[3] = tmpHash[2];
        tmpHash[2] = tmpHash[1];
        tmpHash[1] = tmpHash[0];
        tmpHash[0] = tmpInt0 + tmpInt1;
        
      }
      
      for (int jx = 0; jx < 8; jx++) {
        hash[jx] += tmpHash[jx];
      }
    }
    
    delete conditionedMsg;
    
    return hash;
  }
  
}




#endif
