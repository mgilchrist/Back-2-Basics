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

#include "List.h"
#include "Array.h"

namespace Collection {
  
  
  
  template <class ElementType, class KeyType>
  class Heap : public Array<Comparable<ElementType,KeyType> *>
  {
    
  private:
    void swap(uint64_t i,uint64_t j);
    Comparable<ElementType,KeyType> *removeHeapEntry(int index);
    
  public:
    
    Heap();
    
    ElementType peekAtHeapEntry(int index);
    void heapifyUp(uint64_t i);
    void heapifyDown(uint64_t i);
    uint64_t *push(ElementType,KeyType);
    ElementType pop();
    ElementType peek(uint64_t);
    
  };
  
  template <class ElementType, class KeyType>
  Heap<ElementType,KeyType>::Heap() {
    this->collection = new Comparable<ElementType,KeyType>*[STD_COLLECTION_SIZE];
    this->size = 0;
    this->capacity = STD_COLLECTION_SIZE;
  }
  
  template <class ElementType, class KeyType>
  void Heap<ElementType,KeyType>::swap(uint64_t i,uint64_t j) {
    Comparable<ElementType,KeyType> *tmp;
    tmp = this->collection[i];
    this->collection[i] = this->collection[j];
    *(this->collection[i]->index) = i;
    this->collection[j] = tmp;
    *(this->collection[j]->index) = j;
  }
  
  template <class ElementType, class KeyType>
  ElementType Heap<ElementType,KeyType>::peek(uint64_t index) {
    
    return this->collection[index]->data;
    
  }
  
  template <class ElementType, class KeyType>
  ElementType Heap<ElementType,KeyType>::pop() {
    return removeHeapEntry(0)->data;
  }
  
  template <class ElementType, class KeyType>
  Comparable<ElementType,KeyType> *Heap<ElementType,KeyType>::removeHeapEntry(int index) {
    
    Comparable<ElementType,KeyType> *entry = this->collection[index];
    
    this->collection[index] = NULL;
    
    heapifyDown(index);
    
    this->size--;
    
    return entry;
  }
  
  
  template <class ElementType, class KeyType>
  void Heap<ElementType,KeyType>::heapifyUp(uint64_t i) {
    
    int j;
    
    if (i > 1) {
      j = i / 2; // parent of i
      
      if (this->collection[i]->key < this->collection[j]->key) {
        this->swap(i,j);
        this->heapifyUp(j);
      }
    }
  }
  
  template <class ElementType, class KeyType>
  void Heap<ElementType,KeyType>::heapifyDown(uint64_t i) {
    
    uint64_t left, right, j;
    
    uint64_t n = this->size;
    
    if ((2*i) > n) {
      return;
    } else if ((2*i) < n) {
      left = 2 * i;
      right = (2 * i) + 1;
      if (this->collection[left]->key < this->collection[right]->key) {
        j = left;
      } else {
        j = right;
      }
    } else if (2 * i == n) {
      j = 2 * i;
    }
    
    if (this->collection[i]->key < this->collection[j]->key) {
      this->swap(i,j);
    }
  }
  
  template <class ElementType, class KeyType>
  uint64_t *Heap<ElementType,KeyType>::push(ElementType element, KeyType key) {
    Comparable<ElementType,KeyType> *tmp;
    
    tmp = new Comparable<ElementType,KeyType>(element,key);
    tmp->index = new uint64_t;
    *(tmp->index) = this->size;
    
    if (this->size >= this->capacity) {
      Comparable<ElementType,KeyType> **nHeap = new Comparable<ElementType,KeyType>*[this->capacity * 2];
      
      for (int ix = 0; ix < this->size; ix++) {
        nHeap[ix] = this->collection[ix];
      }
      
      delete this->collection;
      
      this->collection = nHeap;
      this->capacity *= 2;
    }
    
    this->collection[this->size++] = tmp;
    
    heapifyUp(this->size-1);
    
    return tmp->index;
    
  }
  
  template <class ElementType, class KeyType>
  ElementType Heap<ElementType,KeyType>::peekAtHeapEntry(int index) {
    
    Comparable<ElementType,KeyType> *entry = this->collection[index];
    
    return entry->data;
  }
  
  
  
}




#endif
