//
//  ArrayList.h
//  OpenSource
//
//  Created by Marcus Gilchrist on 9/17/12.
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

#ifndef OpenSource_ArrayList_h
#define OpenSource_ArrayList_h

#include "List.h"
#include "Stack.h"

namespace Collection {
  
  template <class ElementType, class KeyType>
  class ArrayList : public List<ElementType,KeyType>, public Array<Comparable<ElementType, KeyType> *>
  {
    
  public:
    ArrayList();
    ArrayList(uint64_t size);
    
    Comparable<ElementType,KeyType> *atIndex(uint64_t index);
    //Comparable<ElementType,KeyType> **ptrToIndex(uint64_t index);
    
    uint8_t median5(Comparable<ElementType,KeyType> *array, uint8_t size);
    uint64_t medianOfMedians(uint64_t l, uint64_t r);
    
    ArrayList<ElementType,KeyType> *cloneSort();
    
    ArrayList<ElementType,KeyType> *mergeSort(uint64_t l, uint64_t r);
    ArrayList<ElementType,KeyType> *merge(ArrayList<ElementType,KeyType> *l,
                                          ArrayList<ElementType,KeyType> *r);
    
    uint64_t binary_search_first_term(uint64_t key, uint64_t min, uint64_t max);
    uint64_t binary_search_last_term(uint64_t key, uint64_t min, uint64_t max);
    
    void inPlaceSort() {
      this->quickSort(0, this->size-1);
    }
    
    void swap(uint64_t first, uint64_t second) {
      Comparable<ElementType,KeyType> *a, *b;
      
      a = this->collection[first];
      b = this->collection[second];
      
      
      swapComparables(a,b);
    } 
    
    
    void swapComparables(Comparable<ElementType,KeyType> *a, Comparable<ElementType,KeyType> *b) {
      uint64_t tmp;
      
      
      this->collection[*(a->index)] = b;
      this->collection[*(b->index)] = a;
      
      tmp = *(a->index);
      *(a->index) = *(b->index);
      *(b->index) = tmp;
      
    }
    
    uint64_t choosePivot(uint64_t l, uint64_t r) {
      return this->chooseRandomPivot(l, r);
    }
    
    uint64_t partition(uint64_t p, uint64_t l, uint64_t r) {
      
      KeyType pValue = this->collection[p]->key;
      uint64_t i = l;
      uint64_t j;
      
      // Move pivot out of the way
      swapComparables(this->collection[r], this->collection[p]);
      
      for (j = l + 1; j < r; j++) {
        // Move to left if value is less than pivot
        if (this->collection[j]->key <= pValue) {
          i++;
          swapComparables(this->collection[i], this->collection[j]);
        }
      }
      
      // move pivot to median
      swapComparables(this->collection[i], this->collection[p]);
      
      return i;
      
    }
    
    ArrayList *clone() {
      ArrayList<ElementType,KeyType> *nArrayList = new ArrayList<ElementType,KeyType>(this->size);
      
      for (int ix = 0; ix < this->size; ix++) {
        nArrayList->collection[ix] = this->collection[ix];
      }
      
      nArrayList->size = this->size;
      nArrayList->capacity = this->size;
      
      return nArrayList;
    }
  };
  
  
  /* ArrayList */
  
  template <class ElementType, class KeyType>
  ArrayList<ElementType,KeyType>::ArrayList() {
    this->collection = new Comparable<ElementType,KeyType>*[STD_COLLECTION_SIZE];
    this->size = STD_COLLECTION_SIZE;
    this->capacity = STD_COLLECTION_SIZE;
  }
  
  template <class ElementType, class KeyType>
  ArrayList<ElementType,KeyType>::ArrayList(uint64_t size) {
    this->collection = new Comparable<ElementType,KeyType>*[size];
    this->size = size;
    this->capacity = size;
  }
    
  template <class ElementType, class KeyType>
  Comparable<ElementType,KeyType> *ArrayList<ElementType,KeyType>::atIndex(uint64_t index) {
    return this->collection[index];
  }
 
  /*
  template <class ElementType, class KeyType>
  Comparable<ElementType,KeyType> *ArrayList<ElementType,KeyType>::ptrToIndex(uint64_t index) {
    return &(this->collection[index]);
  }
   */
  
  template <class ElementType, class KeyType>
  uint8_t ArrayList<ElementType,KeyType>::median5(Comparable<ElementType,KeyType> *array, uint8_t size) {
    if (size > 5) {
      return ERROR;
    }
    if (size < 3) {
      return 0;
    }
    
    const uint8_t vQual = ((size-1)/2);
    uint8_t lessThans = 0;
    uint8_t moreThans = 0;
    
    for (int ix = 0; ix < size-1; ix++) {
      for (int jx = ix+1; jx < size; jx++) {
        if (array[ix].key > array[jx].key) {
          moreThans++;
        } else {
          lessThans++;
        }
      }
      if ((moreThans > vQual) && (lessThans > vQual)) {
        return ix;
      }
    }
    
    return size-1;
    
  }
  
  template <class ElementType, class KeyType>
  uint64_t ArrayList<ElementType,KeyType>::medianOfMedians(uint64_t l, uint64_t r) {
    
    if ((r - l) < 5) {
      return (l + median5(this->collection, r-l));
    }
    
    const uint8_t vRecurseDepth = (u_int8_t)(log(r-l)/log(5));
    
    Stack<Comparable<ElementType,KeyType> *> *nArray;
    Stack<uint64_t *> *nPosArray;
    Comparable<ElementType, KeyType> *layer;
    uint8_t stackPos[vRecurseDepth];
    uint64_t nextSample;
    uint64_t tmpPos;
    
    nArray = new Stack<Comparable<ElementType,KeyType> *>(vRecurseDepth);
    
    for (int ix = 0; ix < vRecurseDepth; ix++) {
      stackPos[ix] = 0;
      nArray->push(new Comparable<ElementType,KeyType>[5]);
      nPosArray->push(new uint64_t[5]);
    }
    
    while (l < r) {
      nextSample = ((l+5) > r) ? r : (l+5);
      tmpPos = l + median5(&(this->collection[l]), nextSample-l);
      nPosArray->atIndex(0)[stackPos[0]] = tmpPos;
      layer = nArray->atIndex(0);
      layer[stackPos[0]] = this->collection[tmpPos];
      stackPos[0]++;
      l = nextSample;
      for (int ix = 1; ix < vRecurseDepth; ix++) {
        if (stackPos[ix-1] == 5) {
          layer = nArray->atIndex(ix-1);
          tmpPos = this->collection[median5(layer,5)];
          nPosArray->atIndex(ix)[stackPos[ix]] = nPosArray->atIndex(ix-1)[tmpPos];
          layer = nArray->atIndex(ix);
          layer[stackPos[ix]] = this->collection[nPosArray->atIndex(ix-1)[tmpPos]];
          stackPos[ix]++;
          stackPos[ix-1] = 0;
          continue;
        }
        break;
      }
    }
    
    return nPosArray[vRecurseDepth-1][median5(nArray[vRecurseDepth-1], stackPos[vRecurseDepth-1])];
    
  }
  
  
  template <class ElementType, class KeyType>
  ArrayList<ElementType, KeyType> *ArrayList<ElementType,KeyType>::merge(ArrayList<ElementType,KeyType> *leftList,
                                                                    ArrayList<ElementType,KeyType> *rightList) {
    Comparable<ElementType, KeyType> *left, *right;
    uint64_t pos, leftPos, rightPos;
    uint64_t leftLength = leftList->getSize();
    uint64_t rightLength = rightList->getSize();
    
    right = rightList->atIndex(0);
    left = leftList->atIndex(0);
    
    pos = 0;
    leftPos = 0;
    rightPos = 0;
    
    ArrayList<ElementType,KeyType> *mergedArray;
    
    mergedArray = new ArrayList<ElementType,KeyType>(leftLength+rightLength);
    
    while ((leftLength > 0) or (rightLength > 0)) {
      if ((leftLength > 0) and (rightLength > 0)) {
        if (left->key <= right->key) {
          mergedArray->setIndex(pos, left);
          left = leftList->atIndex(++leftPos);
          leftLength--;
        } else {
          mergedArray->setIndex(pos, right);
          right = rightList->atIndex(++rightPos);
          rightLength--;
        }
      } else if (leftLength > 0) {
        mergedArray->setIndex(pos, left);
        left = leftList->atIndex(++leftPos);
        leftLength--;
      } else if (rightLength > 0) {
        mergedArray->setIndex(pos, right);
        right = rightList->atIndex(++rightPos);
        rightLength--;
      }
      pos++;
    }
    
    delete left;
    delete right;
    
    return mergedArray;
  }
  
  template <class ElementType, class KeyType>
  ArrayList<ElementType,KeyType> *ArrayList<ElementType,KeyType>::mergeSort(uint64_t l, uint64_t r) {
    
    ArrayList *left, *right;
    
    if (l == r) {
      ArrayList *ret = new ArrayList(1);
      ret->setIndex(0, this->atIndex(l));
      return ret;
    }
    
    uint64_t middle = ((r - l) / 2) + l;
    
    left = mergeSort(l, middle);
    right = mergeSort(middle+1, r);
    
    return merge(left, right);
    
  }
  
  
  /* Search */
  template <class ElementType, class KeyType>
  uint64_t ArrayList<ElementType,KeyType>::binary_search_first_term(uint64_t key, uint64_t min, uint64_t max) {
    uint64_t mid;
    
    while (max > min) {
      mid = ((max - min) / 2) + min;
      if (this->comparables[mid].key < key) {
        min = mid + 1;
      } else {
        max = mid;
      }
    }
    
    if ((max == min) && (this->comparables[min].key == key)) {
      return min;
    } else {
      return ERROR;
    }
  }
  
  template <class ElementType, class KeyType>
  uint64_t ArrayList<ElementType,KeyType>::binary_search_last_term(uint64_t key, uint64_t min, uint64_t max) {
    uint64_t mid;
    
    while (max > min) {
      mid = ((max - min) / 2) + min;
      if (this->comparables[mid].key > key) {
        max = mid - 1;
      } else {
        min = mid;
      }
    }
    
    if ((max == min) && (this->comparables[min].key == key)) {
      return min;
    } else {
      return ERROR;
    }
  }
  
  template <class ElementType, class KeyType>
  ArrayList<ElementType,KeyType> *ArrayList<ElementType,KeyType>::cloneSort() {
    return mergeSort(0,this->getSize()-1);
  }
  
 
}
#endif
