//
//  Stack.h
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

#ifndef OpenSource_Stack_h
#define OpenSource_Stack_h

#include "Array.h"

namespace Collection {
  
  
  template <class ElementType>
  class Stack : public Array<ElementType>
  {
  public:
    Stack();
    Stack(uint64_t capacity);
    
    /* Parent Methods */
    
    Stack *clone();
    
    /* Stack Methods */
    virtual void push(ElementType element);
    virtual ElementType pop();
    virtual ElementType peek();
    
  };
  
  /* Stack */
  
  template <class ElementType>
  Stack<ElementType>::Stack() {
    this->collection = new ElementType[STD_COLLECTION_SIZE];
    this->size = 0;
    this->capacity = STD_COLLECTION_SIZE;
  }
  
  template <class ElementType>
  Stack<ElementType>::Stack(uint64_t capacity) {
    this->collection = new ElementType[capacity];
    this->size = 0;
    this->capacity = capacity;
  }
  
  /* Parent Methods */
  template <class ElementType>
  Stack<ElementType> *Stack<ElementType>::clone() {
    Stack *nArray = new Stack(this->size);
    
    for (int ix = 0; ix < this->size; ix++) {
      nArray->collection[ix] = this->collection[ix];
    }
    
    nArray->size = this->size;
    nArray->capacity = this->size;
    
    return nArray;
  }
  
  template <class ElementType>
  void Stack<ElementType>::push(ElementType element) {
    if (this->size >= this->capacity) {
      ElementType *nStack = new ElementType[this->capacity * 2];
      
      for (int ix = 0; ix < this->size; ix++) {
        nStack[ix] = this->collection[ix];
      }
      
      delete this->collection;
      
      this->collection = nStack;
      this->capacity *= 2;
    }
    
    this->collection[this->size] = element;
    this->size++;
  }
  
  template <class ElementType>
  ElementType Stack<ElementType>::pop() {
    if (!this->size) {
      return NULL;
    }
    
    this->size--;
    
    return this->collection[this->size];
  }
  
  template <class ElementType>
  ElementType Stack<ElementType>::peek() {
    if (!this->size) {
      return NULL;
    }
    
    return this->collection[this->size-1];
  }
  
}

#endif
