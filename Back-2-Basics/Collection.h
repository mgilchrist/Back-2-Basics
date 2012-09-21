//
//  Collection.h
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 8/30/12.
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
