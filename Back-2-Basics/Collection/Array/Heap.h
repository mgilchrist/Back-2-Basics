//
//  Heap.h
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

#ifndef OpenSource_Heap_h
#define OpenSource_Heap_h

#include <vector>

#include "List.h"
#include "Stack.h"

namespace Collection {
  
  
  
  template <class ElementType, class KeyType>
  class Heap
  {
    
  private:
    std::vector<Comparable<ElementType, KeyType> *> *collection;
    uint64_t numEntries;
    void swap(uint64_t i,uint64_t j);
    void heapifyUp(uint64_t i);
    void heapifyDown(uint64_t i);
    
  public:
    
    Heap();
    
    void remove(uint64_t index);
    ElementType at(uint64_t index);
    uint64_t *update(uint64_t, KeyType);
    uint64_t *push(ElementType,KeyType);
    ElementType pop();
    
    uint64_t size() {
      return numEntries;
    }
    
  };
  
  template <class ElementType, class KeyType>
  Heap<ElementType,KeyType>::Heap() {
    collection = new std::vector<Comparable<ElementType,KeyType> *>(STD_COLLECTION_SIZE);
    numEntries = 0;
  }
  
  template <class ElementType, class KeyType>
  void Heap<ElementType,KeyType>::swap(uint64_t i,uint64_t j) {
    Comparable<ElementType,KeyType> *tmp;
    
    tmp = collection->at(i);
    collection->at(i) = collection->at(j);
    collection->at(j) = tmp;
    
    if (collection->at(i) != NULL) {
      *(collection->at(i)->index) = i;
    }
    
    if (collection->at(j) != NULL) {
      *(collection->at(j)->index) = j;
    }
  }
  
  template <class ElementType, class KeyType>
  ElementType Heap<ElementType,KeyType>::pop() {
    
    Comparable<ElementType,KeyType> *entry = collection->at(0);
    ElementType ret;
    
    if (entry == NULL) {
      return NULL;
    }
    
    ret = entry->data;
    
    remove(0);
    
    return ret;
  }
  
  template <class ElementType, class KeyType>
  void Heap<ElementType,KeyType>::remove(uint64_t index) {
    
    if (numEntries > index) {
      delete collection->at(index)->index;
      delete collection->at(index);
      
      collection->at(index) = collection->at(numEntries-1);
      *(collection->at(index)->index) = index;
      collection->at(numEntries-1) = NULL;
      
      heapifyDown(index);
      
      this->numEntries--;
    }
    
  }
  
  template <class ElementType, class KeyType>
  uint64_t *Heap<ElementType,KeyType>::update(uint64_t index, KeyType key) {
    
    ElementType mark = collection->at(index)->data;
    
    remove(index);
    return push(mark, key);
    
  }
  
  template <class ElementType, class KeyType>
  void Heap<ElementType,KeyType>::heapifyUp(uint64_t i) {
    
    uint64_t j;
    
    if (i > 0) {
      j = (i-1) / 2; // parent of i
      
      if ((collection->at(j) == NULL) ||
          (collection->at(i)->key < collection->at(j)->key)) {
        this->swap(i,j);
        this->heapifyUp(j);
      }
    }
  }
  
  template <class ElementType, class KeyType>
  void Heap<ElementType,KeyType>::heapifyDown(uint64_t i) {
    
    uint64_t left;
    uint64_t right;
    uint64_t j;
    
    uint64_t n = numEntries-1;
    
    left = (2 * (i + 1)) - 1;
    right = 2 * (i + 1);
    
    if (right <= n) {
      
      if ((collection->at(left) == NULL) && (collection->at(right) == NULL)) {
        return;
      }
      
      if (collection->at(right) == NULL) {
        j = left;
      } else if (collection->at(left) == NULL) {
        j = right;
      } else {
        if (collection->at(left)->key <= collection->at(right)->key) {
          j = left;
        } else {
          j = right;
        }
      }
    } else if ((collection->at(n) != NULL) && (left == n)) {
      j = left;
    } else {
      return;
    }
    
    if ((collection->at(i) == NULL) || (collection->at(i)->key > collection->at(j)->key)) {
      swap(i,j);
      heapifyDown(j);
    }
  }
  
  template <class ElementType, class KeyType>
  uint64_t *Heap<ElementType,KeyType>::push(ElementType element, KeyType key) {
    Comparable<ElementType,KeyType> *tmp;
    
    tmp = new Comparable<ElementType,KeyType>(element,key);
    tmp->index = new uint64_t;
    *(tmp->index) = this->numEntries;
    
    if (numEntries == collection->size()) {
      collection->resize(collection->size()*2);
    }
    
    collection->at(numEntries) = tmp;
    
    numEntries++;
    
    heapifyUp(numEntries-1);
    
    return tmp->index;
    
  }
  
  template <class ElementType, class KeyType>
  ElementType Heap<ElementType,KeyType>::at(uint64_t index) {
    
    if (collection->at(index) == NULL) {
      return NULL;
    }
    
    return collection->at(index)->data;
  }
  
  
  
}




#endif
