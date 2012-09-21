//
//  HashTable.h
//  OpenSource
//
//  Created by Marcus Gilchrist on 9/16/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
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
