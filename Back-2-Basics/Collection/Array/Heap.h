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
    uint64_t size;
    void swap(uint64_t i,uint64_t j);
    void heapifyUp(uint64_t i);
    void heapifyDown(uint64_t i);
    
  public:
    
    Heap();
    
    Comparable<ElementType,KeyType> *removeHeapEntry(uint64_t index);
    ElementType peekAtHeapEntry(uint64_t index);
    void update(uint64_t i, KeyType);
    uint64_t *push(ElementType,KeyType);
    ElementType pop();
    ElementType peek(uint64_t);
    uint64_t getSize() {
      return size;
    }
    
  };
  
  template <class ElementType, class KeyType>
  Heap<ElementType,KeyType>::Heap() {
    collection = new std::vector<Comparable<ElementType,KeyType> *>(STD_COLLECTION_SIZE);
    size = 0;
  }
  
  template <class ElementType, class KeyType>
  void Heap<ElementType,KeyType>::swap(uint64_t i,uint64_t j) {
    Comparable<ElementType,KeyType> *tmp;
    tmp = collection->at(i);
    
    collection->at(i) = collection->at(j);
    
    if (collection->at(j) != NULL) {
      *(collection->at(i)->index) = i;
    }
    collection->at(j) = tmp;
    
    if (tmp != NULL) {
      *(collection->at(j)->index) = j;
    }
  }
  
  template <class ElementType, class KeyType>
  ElementType Heap<ElementType,KeyType>::peek(uint64_t index) {
    
    if (collection->at(index) == NULL) {
      return NULL;
    }
    
    return collection->at(index)->data;
    
  }
  
  template <class ElementType, class KeyType>
  ElementType Heap<ElementType,KeyType>::pop() {
    
    Comparable<ElementType,KeyType> *entry;
    ElementType ret;
    
    if ((entry = removeHeapEntry(0)) == NULL) {
      return NULL;
    }
    
    ret = entry->data;
    
    delete entry->index;
    delete entry;
    
    return ret;
  }
  
  template <class ElementType, class KeyType>
  Comparable<ElementType,KeyType> *Heap<ElementType,KeyType>::removeHeapEntry(uint64_t index) {
    
    Comparable<ElementType,KeyType> *entry = collection->at(index);
    
    collection->at(index) = NULL;
    
    heapifyDown(index);
    
    this->size--;
    
    return entry;
  }
  
  template <class ElementType, class KeyType>
  void Heap<ElementType,KeyType>::update(uint64_t i, KeyType key) {
    
    if (collection->at(i)->key > key) {
      collection->at(i)->key = key;
      heapifyUp(i);
    }
    
    if (collection->at(i)->key < key) {
      collection->at(i)->key = key;
      heapifyDown(i);
    }
  }
  
  template <class ElementType, class KeyType>
  void Heap<ElementType,KeyType>::heapifyUp(uint64_t i) {
    
    int j;
    
    if (i > 0) {
      j = (i-1) / 2; // parent of i
      
      if (collection->at(i)->key < collection->at(j)->key) {
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
    
    uint64_t n = collection->size()-1;
    
    if ((2*(i+1)) > n) {
      collection->resize(collection->size()*2);
      n = collection->size()-1;
    }
    
    if ((2*(i+1)) < n) {
      left = (2 * (i + 1)) - 1;
      right = 2 * (i + 1);
      if ((collection->at(left) != NULL) &&
          ((collection->at(right) == NULL) || (collection->at(left)->key < collection->at(right)->key))) {
        j = left;
      } else if (collection->at(right) != NULL) {
        j = right;
      } else {
        return;
      }
    } else if ((collection->at(n)) && ((2*(i+1)) == n)) {
      j = n;
    } else {
      return;
    }
    
    if ((collection->at(i) == NULL) ||
        (collection->at(i)->key < collection->at(j)->key)) {
      this->swap(i,j);
      heapifyDown(j);
    }
  }
  
  template <class ElementType, class KeyType>
  uint64_t *Heap<ElementType,KeyType>::push(ElementType element, KeyType key) {
    Comparable<ElementType,KeyType> *tmp;
    
    tmp = new Comparable<ElementType,KeyType>(element,key);
    tmp->index = new uint64_t;
    *(tmp->index) = this->size;
    
    if (size == collection->size()) {
      collection->resize(collection->size()*2);
    }
    
    collection->at(size) = tmp;
    
    size++;
    
    heapifyUp(size-1);
    
    return tmp->index;
    
  }
  
  template <class ElementType, class KeyType>
  ElementType Heap<ElementType,KeyType>::peekAtHeapEntry(uint64_t index) {
    
    Comparable<ElementType,KeyType> *entry = collection->at(index);
    
    return entry->data;
  }
  
  
  
}




#endif
