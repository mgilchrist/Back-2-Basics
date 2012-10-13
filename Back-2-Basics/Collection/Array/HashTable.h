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
    std::vector<uint64_t> fullHashMap;
    
    std::vector<uint64_t> *SHA_2_Prep(std::vector<uint8_t> &data);
    uint64_t *SHA_2(Key<KeyType> *);
    void grow();
    
  public:
    HashTable();
    uint64_t search(KeyType&);
    void remove(uint64_t);
    void insert(ElementType, KeyType&);
    ElementType get(uint64_t);
    void update(uint64_t,ElementType);
    //ElementType *clone();
  };
  
  
  /* Class: HashTable */
  
  template <class ElementType, class KeyType>
  ElementType HashTable<ElementType,KeyType>::get(uint64_t index) {
    return collection[index];
  }
  
  template <class ElementType, class KeyType>
  void HashTable<ElementType,KeyType>::update(uint64_t index, ElementType data) {
    collection[index] = data;
  }
  
  
  template <class ElementType, class KeyType>
  HashTable<ElementType,KeyType>::HashTable() {
    
    collection.resize(STD_COLLECTION_SIZE, 0);
    keyMap.resize(STD_COLLECTION_SIZE, 0);
    fullHashMap.resize(STD_COLLECTION_SIZE, 0);
    
    
  }
  
  template <class ElementType, class KeyType>
  void HashTable<ElementType,KeyType>::grow() {
    
    uint64_t newIndex;
    uint64_t oldSize = fullHashMap.size()*2;
    
    collection.resize(collection.size()*2, 0);
    keyMap.resize(keyMap.size()*2, 0);
    fullHashMap.resize(fullHashMap.size()*2, 0);
    
    for (uint64_t ix = oldSize-1; ix > 0; ix--) {
      newIndex = fullHashMap[ix] % fullHashMap.size();
      
      if (ix != newIndex) {
        collection[newIndex] = collection[ix];
        keyMap[newIndex] = keyMap[ix];
        fullHashMap[newIndex] = fullHashMap[ix];
        collection[ix] = (ElementType)NULL;
        keyMap[ix] = (Key<KeyType> *)NULL;
        fullHashMap[ix] = 0;
      }
    }
    
    
  }
  
  template <class ElementType, class KeyType>
  uint64_t HashTable<ElementType,KeyType>::search(KeyType &key) {
    
    uint8_t useHash;
    uint64_t index;
    Key<KeyType> *kKey = new Key<KeyType>(key);
    uint64_t *hash = SHA_2(kKey);
    useHash = 8;
    
    while (useHash) {
      useHash--;
      index = hash[useHash] % keyMap.size();
      if (index) {
        if ((keyMap[index] != NULL) && (*(keyMap[index]) == key)) {
          delete hash;
          return index;
        }
      }
    }
    
    delete hash;
    return 0;
    
  }
  
  template <class ElementType, class KeyType>
  void HashTable<ElementType,KeyType>::remove(uint64_t index) {
    if (keyMap[index] != NULL) {
      delete keyMap[index];
      keyMap[index] = NULL;
      fullHashMap[index] = 0;
      collection[index] = NULL;
    }
  }
  
  template <class ElementType, class KeyType>
  void HashTable<ElementType,KeyType>::insert(ElementType data, KeyType &key) {
    
    uint8_t useHash = 8;
    uint64_t index;
    Key<KeyType> *kKey = new Key<KeyType>(key);
    uint64_t *hash = SHA_2(kKey);
    
    do {
      while (useHash) {
        useHash--;
        index = hash[useHash] % keyMap.size();
        if (index) {
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
      
      this->grow();
      useHash = 8;
      
    } while ((1));
    
    
    delete hash;
    
  }
  
  template <class ElementType, class KeyType>
  std::vector<uint64_t> *HashTable<ElementType,KeyType>::SHA_2_Prep(std::vector<uint8_t> &data) {
    uint64_t dataSize = data.size();
    uint64_t arraySize = (128 - (dataSize % 128) % 111) + dataSize;
    std::vector<uint8_t> *ret = new std::vector<uint8_t>(data.begin(), data.end());
    
    ret->resize(arraySize, 0);
    (*ret)[dataSize] = 0x01;
    
    *((uint64_t *)(&(*ret)[arraySize-8])) = dataSize;
    
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
