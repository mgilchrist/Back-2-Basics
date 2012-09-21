//
//  List.h
//  OpenSource
//
//  Created by Marcus Gilchrist on 9/16/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#ifndef OpenSource_List_h
#define OpenSource_List_h

#include "Collection.h"


#define ERROR -1

namespace Collection {
  
  template <class ElementType, class KeyType>
  class List;
  
  template <class ElementType, class KeyType>
  class Comparable
  {
  public:
    ElementType data;
    KeyType key;
    uint64_t *index;

    
    Comparable();
  };
  
  /* Indexed Comparable */
  
  template <class ElementType, class KeyType>
  Comparable<ElementType,KeyType>::Comparable() {
    this->data = NULL;
    this->key = 0;
    this->index = NULL;
  }
  
  template <class ElementType, class KeyType>
  class List
  {
  protected:
    bool sorted = false;
    
  protected:
    uint64_t chooseRandomPivot(uint64_t l, uint64_t r);
    virtual uint64_t choosePivot(uint64_t l, uint64_t r) =0;
    virtual uint64_t partition(uint64_t p, uint64_t l, uint64_t r) =0;
    
  public:
    virtual void swap(uint64_t a, uint64_t b) =0;
    Comparable<ElementType, KeyType> atIndex(uint64_t index);
    virtual void swapComparables(Comparable<ElementType,KeyType> *a, Comparable<ElementType,KeyType> *b) =0;
    
    void quickSort(uint64_t l, uint64_t r);
    uint64_t selection(uint64_t l, uint64_t r, uint64_t k);
    
  public:
    
    List();
    
    void inPlaceSort();
    
  };
  
  /* Comparable Collection */
  
  template <class ElementType, class KeyType>
  List<ElementType,KeyType>::List() {
    
  }
  
  
  template <class ElementType, class KeyType>
  void List<ElementType,KeyType>::inPlaceSort() {
    quickSort(0, this->size-1);
  }
  
  
  template <class ElementType, class KeyType>
  uint64_t List<ElementType,KeyType>::chooseRandomPivot(uint64_t l, uint64_t r) {
    return (((random())%(r-l))+l);
  }
  
  // Best: O(nlogn) Worst: O(n^2) Average: O(nlogn) Space: O(logn)
  template <class ElementType, class KeyType>
  void List<ElementType,KeyType>::quickSort(uint64_t l, uint64_t r) {
    
    uint64_t pIndex;
    uint64_t p;
    
    if (l < r) {
      p = this->choosePivot(l, r);
      
      pIndex = this->partition(p, l, r);
      quickSort(l, pIndex-1);
      quickSort(pIndex+1, r);
      
    }
    
  }
  
  template <class ElementType, class KeyType>
  uint64_t List<ElementType,KeyType>::selection(uint64_t l, uint64_t r, uint64_t k) {
    
    uint64_t pIndex, p;
    uint64_t pDistance;
    
    if (l < r) {
      p = this->medianOfMedians(l, r);
      
      pIndex = this->partition(p, l, r);
      
      pDistance = pIndex - l + 1;
      
      if (pDistance == k) {
        return pIndex;
      } else if (k < pIndex) {
        return selection(l, pIndex-1, k);
      } else {
        return selection(pIndex+1, r, k);
      }
    }
    
    return l;
    
  }
  
}

#endif
