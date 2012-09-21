//
//  Data_Structure.h
//  Algorithms
//
//  Created by Marcus Gilchrist on 8/30/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#ifndef __OpenSource__Collection__
#define __OpenSource__Collection__

#include <iostream>
#include <math.h>

#define STD_COLLECTION_SIZE 16

namespace Collection {
  
  //template <class ElementType, class KeyType>
  //class Comparable;
  
  
  
  /* Virtual Class: Collection */
  template <class CollectionType, class ElementType>
  class Collection
  {
  protected:
    
    ElementType *collection;
    uint64_t size;
    
    
  public:
    
    Collection();
    
    virtual Collection<CollectionType,ElementType> *clone() =0;
    virtual uint64_t getSize();
    
  };
  
  
  template <class CollectionType, class ElementType>
  Collection<CollectionType,ElementType>::Collection() {
  }
  
  template <class CollectionType, class ElementType>
  uint64_t Collection<CollectionType,ElementType>::getSize() {
    return size;
  }
  
  
  
}


#endif /* defined(__OpenSource__Collection__) */
