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
  
  
  /* Class: HashTable */
  template <class ElementType>
  class HashTable : public Collection<ElementType>
  {
    uint64_t *keyMap;
    
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
  
}

#endif
