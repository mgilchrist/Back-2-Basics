//
//  Array.h
//  OpenSource
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

#ifndef OpenSource_Array_h
#define OpenSource_Array_h

#include "Collection.h"

namespace Collection {
  
  
  
  template <class ElementType>
  class Array;

  /* Array */
  
  template <class ElementType>
  class Array : public Collection<Array<ElementType>,ElementType>
  {
  protected:
    uint64_t capacity;
    
  public:
    Array();
    Array(uint64_t size);
    Array(ElementType *data, uint64_t size);
    void swap(uint64_t a,uint64_t b);
    ElementType atIndex(uint64_t index);
    ElementType *ptrToIndex(uint64_t index);
    void setIndex(uint64_t index, ElementType element);
    uint64_t getCapacity();
    
    Collection<Array<ElementType>, ElementType> *clone() {
      Array<ElementType> *nArray = new Array<ElementType>(this->size);
      
      for (int ix = 0; ix < this->size; ix++) {
        nArray->collection[ix] = this->collection[ix];
      }
      
      nArray->size = this->size;
      nArray->capacity = this->size;
      
      return nArray;
    }
  };
  
  
  /* Array */
  
  template <class ElementType>
  Array<ElementType>::Array() {
    this->collection = new ElementType[STD_COLLECTION_SIZE];
    this->size = STD_COLLECTION_SIZE;
    this->capacity = STD_COLLECTION_SIZE;
  }
  
  template <class ElementType>
  Array<ElementType>::Array(uint64_t size) {
    this->collection = new ElementType[size];
    this->size = size;
    this->capacity = size;
  }
  
  template <class ElementType>
  Array<ElementType>::Array(ElementType *data, uint64_t size) {
    this->collection = data;
    this->size = size;
    this->capacity = size;
  }
  
  template <class ElementType>
  void Array<ElementType>::swap(uint64_t a,uint64_t b) {
    ElementType tmp;
    
    tmp = this->collection[a];
    this->collection[b] = this->collection[a];
    this->collection[a] = tmp;
  }
  
  template <class ElementType>
  ElementType Array<ElementType>::atIndex(uint64_t index) {
    return this->collection[index];
  }
  
  template <class ElementType>
  ElementType *Array<ElementType>::ptrToIndex(uint64_t index) {
    return &(this->collection[index]);
  }
  
  template <class ElementType>
  void Array<ElementType>::setIndex(uint64_t index, ElementType element) {
    this->collection[index] = element;
  }
  
  template <class ElementType>
  uint64_t Array<ElementType>::getCapacity() {
    return this->capacity;
  }

  
}

#endif
