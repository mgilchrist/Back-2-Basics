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

#include "Array.h"

namespace Collection {
  
  /* First 32 bits of fractional part of the
   square root of {2,3,5,7,11,13,17,19} */
  uint32_t sha_2_h[8] =
  {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
  };
  
  /* First 32 bits of fractional part of the
   cube root of first 64 primes */
  uint32_t sha_2_k[64] =
  {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
  };
  
  
  /* Class: HashTable */
  template <class ElementType>
  class HashTable : public Array<ElementType>
  {
  private:
    uint64_t *keyMap;
    
    Array<uint64_t> *hashFunction(ElementType data);
    
    Array<uint64_t> *simpleHash(ElementType data);
    Array<uint64_t> *randomHash(ElementType data);
    
    Array<uint64_t> *SHA_2_Condition(char *data);
    Array<uint64_t> *SHA_2(ElementType data, uint8_t hashSize);
    
  public:
    HashTable();
    virtual ElementType get(uint64_t);
    virtual ElementType remove(uint64_t);
    virtual void add (ElementType);
    virtual ElementType *clone();
  };
  
  
  /* Class: HashTable */
  
  template <class ElementType>
  HashTable<ElementType>::HashTable() {
    this->collection = new ElementType[STD_COLLECTION_SIZE];
    keyMap = new uint64_t[STD_COLLECTION_SIZE];
    this->size = 0;
    this->capacity = STD_COLLECTION_SIZE;
  }
  
  
  Array<uint32_t> *SHA_2_Condition(char *data, uint64_t dataSize) {
    Array<uint32_t> *arrayRet;
    uint8_t *arraySize = (56 - (dataSize % 64) % 55)) + dataSize;
    ret = new uint8_t[arraySize];
    
    for (int ix = 0; ix < dataSize; ix++) {
      ret[ix] = data[ix];
    }
    
    ret[dataSize] = 0x01;
    
    for (int ix = dataSize+1; ix < arraySize-8; ix++) {
      ret[ix] = 0x00;
    }
    
    *((uint64_t *)(&ret[arraySize-8])) = dataSize;
    
    arrayType = new Array<uint32_t>((uint32_t *)ret, arraySize);
    
    return ret;
  }
  
  Array<uint64_t> *SHA_2(ElementType data) {
    Array<uint32_t> *conditionedMsg;
    uint32_t subArray[64];
    uint32_t tmpHash[8];
    
    if (sizeof(ElementType) != sizeof(data)) {
      conditionedMsg = SHA_2_Condition(data, sizeof(data));
    } else {
      conditionedMsg = SHA_2_Condition(&data, sizeof(ElementType));
    }
    
    for (int jx = 0; jx < 8; jx++) {
      hash[jx] = 0.0;
    }
    
    for (int ix = 0; ix < conditionedMsg->getSize(); ix += 16) {
      for (int jx = 0; jx < 16; jx++) {
        subArray[jx] = conditionedMsg[ix+jx];
      }
      
      for (int jx = 16; jx < 64; jx++) {
        uint32_t tmpInt0 = (((subArray[jx-15] >> 7) | (subArray[jx-15] << 25)) ^
                            ((subArray[jx-15] >> 18) | (subArray[jx-15] << 14)) ^
                            ((subArray[jx-15] >> 3) | (subArray[jx-15] << 29)));
        uint32_t tmpInt1 = (((subArray[jx-2] >> 17) | (subArray[jx-2] << 15)) ^
                            ((subArray[jx-2] >> 19) | (subArray[jx-2] << 13)) ^
                            ((subArray[jx-2] >> 10) | (subArray[jx-2] << 22)));
        subArray[jx] := subArray[jx-16] + tmpInt0 + subArray[jx-7] + tmpInt1;
      }
      
      for (int jx = 0; jx < 8; jx++) {
        tmpHash[jx] = sha_2_h[jx];
      }
      
      for (int jx = 0; jx < 64; jx++) {
        tap0 = (((tmpHash[0] >> 2) | (tmpHash[0] << 30)) ^
                   ((tmpHash[0] >> 13) | (tmpHash[0] << 9)) ^
                   ((tmpHash[0] >> 22) | (tmpHash[0] << 10)));
        majority = ((tmpHash[0] & tmpHash[1]) ^
                    (tmpHash[0] & tmpHash[2]) ^
                    (tmpHash[1] & tmpHash[2]));
        tmpInt0 = tap0 + majority;
        tap1 = (((tmpHash[4] >> 6) | (tmpHash[4] << 26)) ^
                ((tmpHash[4] >> 11) | (tmpHash[4] << 21)) ^
                ((tmpHash[4] >> 25) | (tmpHash[4] << 7)));
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
    
    
  }
  
}




#endif
