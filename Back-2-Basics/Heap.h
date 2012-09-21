//
//  Heap.h
//  OpenSource
//
//  Created by Marcus Gilchrist on 9/16/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#ifndef OpenSource_Heap_h
#define OpenSource_Heap_h

#include "Array.h"

namespace Collection {
  
  
  
  template <class ElementType, class KeyType>
  class Heap : public Collection<IndexedComparable<ElementType,KeyType>>
  {
    
  private:
    void swap(uint64_t i,uint64_t j);
    IndexedComparable<ElementType,KeyType> *peekAtHeapEntry(int index);
    IndexedComparable<ElementType,KeyType> *removeHeapEntry(int index);
    
  public:
    
    Heap();
    
    virtual void heapifyUp(uint64_t i);
    virtual void heapifyDown(uint64_t i);
    virtual uint64_t *push(ElementType,KeyType);
    virtual ElementType *pop();
    virtual ElementType *peek(uint64_t);
    
  };
  
  template <class ElementType, class KeyType>
  Heap<ElementType,KeyType>::Heap() {
    this->collection = new ElementType[STD_COLLECTION_SIZE];
    this->size = 0;
    this->capacity = STD_COLLECTION_SIZE;
  }
  
  template <class ElementType, class KeyType>
  void Heap<ElementType,KeyType>::swap(uint64_t i,uint64_t j) {
    IndexedComparable<ElementType,KeyType> tmp;
    tmp = this->collection[i];
    this->collection[i] = this->collection[j];
    this->collection[i]->index = i;
    this->collection[j] = tmp;
    this->collection[j]->index = j;
  }
  
  template <class ElementType, class KeyType>
  IndexedComparable<ElementType,KeyType> *Heap<ElementType,KeyType>::peekAtHeapEntry(int index) {
    
    return this->collection[index];
    
  }
  
  template <class ElementType, class KeyType>
  IndexedComparable<ElementType,KeyType> *Heap<ElementType,KeyType>::removeHeapEntry(int index) {
    
    IndexedComparable<ElementType,KeyType> *entry = this->collection[index];
    
    this->collection[index] = NULL;
    
    heapifyDown(index);
    
    this->size--;
    
    return entry;
  }
  
  
  template <class ElementType, class KeyType>
  void Heap<ElementType,KeyType>::heapifyUp(uint64_t i) {
    
    int j;
    IndexedComparable<ElementType,KeyType> *temp;
    
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
    ElementType *temp;
    
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
    
    if (this->collection->key < this->collection->key) {
      temp = this->swap(i,j);
    }
  }
  
  template <class ElementType, class KeyType>
  uint64_t *Heap<ElementType,KeyType>::push(ElementType element, KeyType key) {
    IndexedComparable<ElementType,KeyType> *tmp;
    
    tmp = IndexedComparable<ElementType,KeyType>(element,key);
    tmp->auxIndex = new uint64_t;
    *(tmp->auxIndex) = this->size;
    
    this->collection[this->size++] = tmp;
    
    heapifyUp(this->size-1);
    
    return tmp->auxIndex;
    
  }
  
  
}




#endif
