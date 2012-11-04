//
//  LLRB_Tree.h
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 10/13/12.
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

#ifndef Back_2_Basics_LLRB_Tree_h
#define Back_2_Basics_LLRB_Tree_h

#include "Tree.h"

namespace Graph {
  
  
  
  template <class DataType, class KeyType>
  class LLRB_TreeNode : public TreeNode<LLRB_TreeNode<DataType,KeyType>,DataType,KeyType>
  {
    static const bool BLACK = false;
    static const bool RED = true;
    
  public:
    
    bool color = BLACK;
    
    LLRB_TreeNode() {
      TreeNode<LLRB_TreeNode<DataType,KeyType>,DataType,KeyType>(2);
    }
  };
  
  template <class EntryType, class KeyType>
  class LLRB_Tree : public Tree<LLRB_TreeNode<EntryType,KeyType>,EntryType,KeyType>
  {
    
  private:
    static const bool RED = true;
    static const bool BLACK = false;
    
    bool isLeft(LLRB_TreeNode<EntryType,KeyType> *);
    bool isRight(LLRB_TreeNode<EntryType,KeyType> *);
    
    LLRB_TreeNode<EntryType,KeyType> *repair(LLRB_TreeNode<EntryType,KeyType> *);
    void recolor(LLRB_TreeNode<EntryType,KeyType> *);
    LLRB_TreeNode<EntryType,KeyType> *rotateLeft(LLRB_TreeNode<EntryType,KeyType> *);
    LLRB_TreeNode<EntryType,KeyType> *rotateRight(LLRB_TreeNode<EntryType,KeyType> *);
    
    LLRB_TreeNode<EntryType,KeyType> * insert(LLRB_TreeNode<EntryType,KeyType> *, EntryType, KeyType);
    
    LLRB_TreeNode<EntryType,KeyType> *moveViolationRight(LLRB_TreeNode<EntryType,KeyType> *);
    LLRB_TreeNode<EntryType,KeyType> *moveViolationLeft(LLRB_TreeNode<EntryType,KeyType> *);
    
    LLRB_TreeNode<EntryType,KeyType> *removeMin(LLRB_TreeNode<EntryType,KeyType> *);
    LLRB_TreeNode<EntryType,KeyType> *removeMax(LLRB_TreeNode<EntryType,KeyType> *);
    LLRB_TreeNode<EntryType,KeyType> *remove(LLRB_TreeNode<EntryType,KeyType> *, KeyType);
    
  public:
    LLRB_Tree();
    
    void insert(EntryType n, KeyType key);
    void remove(KeyType key);
    void removeMax();
    void removeMin();
    void updateKey(KeyType oldKey, KeyType newKey, uint64_t instance);
    void update(EntryType n, KeyType newKey, uint64_t instance);
    
  };
  
  
  template <class EntryType, class KeyType>
  LLRB_Tree<EntryType,KeyType>::LLRB_Tree() {
    
  }
  
  template <class EntryType, class KeyType>
  bool LLRB_Tree<EntryType,KeyType>::isRight(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    return (node->parent->getLeaf(RIGHT) == node) ? true : false;
  }
  
  template <class EntryType, class KeyType>
  bool LLRB_Tree<EntryType,KeyType>::isLeft(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    return (node->parent->getLeaf(LEFT) == node) ? true : false;
  }
  
  template <class EntryType, class KeyType>
  void LLRB_Tree<EntryType,KeyType>::updateKey(KeyType oldKey, KeyType newKey, uint64_t instance) {
    // TODO
  }
  
  template <class EntryType, class KeyType>
  void LLRB_Tree<EntryType,KeyType>::update(EntryType n, KeyType newKey, uint64_t instance) {
    // TODO
  }
  
  
  template <class EntryType, class KeyType>
  void LLRB_Tree<EntryType,KeyType>::insert(EntryType data, KeyType key) {
    
    insert(this->treeRoot, data, key);
    
  }
  
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::insert(LLRB_TreeNode<EntryType,KeyType> *node, EntryType data, KeyType key) {
    
    bool dir;
    
    if (node->getLeaf(0) == NULL) {
      LLRB_TreeNode<EntryType,KeyType> *left = new LLRB_TreeNode<EntryType,KeyType>();
      LLRB_TreeNode<EntryType,KeyType> *right = new LLRB_TreeNode<EntryType,KeyType>();
      
      node->setLeaf(LEFT, left);
      node->setLeaf(RIGHT, right);
      node->data = data;
      node->key = key;
      node->color = RED;
      
      return node;
    }
    
    if (key == node->key) {
      node->data = data;
    } else {
      if (key < node->key) {
        dir = LEFT;
      } else {
        dir = RIGHT;
      }
      node->setLeaf(dir, insert(node->getLeaf(dir), data, key));
    }
    
    
    node = repair(node);
    
    return node;
  }
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::repair(LLRB_TreeNode<EntryType,KeyType> *node) {
    if (node->getLeaf(RIGHT)->color == RED) {
      node = rotateLeft(node);
    }
    
    if ((node->getLeaf(LEFT)->color == RED) &&
        (node->getLeaf(LEFT)->getLeaf(LEFT)->color == RED)) {
      node = rotateRight(node);
    }
    
    if ((node->getLeaf(LEFT)->color == RED) &&
        (node->getLeaf(RIGHT)->color == RED)) {
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType>
  void LLRB_Tree<EntryType,KeyType>::recolor(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    node->color = !(node->color);
    
    node->getLeaf(LEFT)->color = !(node->getLeaf(LEFT)->color);
    node->getLeaf(RIGHT)->color = !(node->getLeaf(RIGHT)->color);
    
  }
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::rotateLeft(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    LLRB_TreeNode<EntryType,KeyType> *right = node->getLeaf(RIGHT);
    node->setLeaf(RIGHT, right->getLeaf(LEFT));
    right->setLeaf(LEFT, node);
    right->color = right->getLeaf(LEFT)->color;
    right->getLeaf(LEFT)->color = RED;
    
    return right;
    
  }
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::rotateRight(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    LLRB_TreeNode<EntryType,KeyType> *left = node->getLeaf(LEFT);
    node->setLeaf(LEFT, left->getLeaf(RIGHT));
    left->setLeaf(RIGHT, node);
    left->color = left->getLeaf(RIGHT)->color;
    left->getLeaf(RIGHT)->color = RED;
    
    return left;
    
    
  }
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::moveViolationRight(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    recolor(node);
    
    if (node->getLeaf(LEFT)->getLeaf(LEFT)->color == RED) {
      node = rotateRight(node);
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::moveViolationLeft(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    recolor(node);
    
    if (node->getLeaf(RIGHT)->getLeaf(LEFT)->color == RED) {
      node->setLeaf(RIGHT, rotateRight(node->getLeaf(RIGHT)));
      node = rotateRight(node);
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType>
  void LLRB_Tree<EntryType,KeyType>::removeMax() {
    
    this->treeRoot = removeMax(this->treeRoot);
    
    this->treeRoot->color = BLACK;
    
  }
  
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::removeMax(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    if (node->getLeaf(LEFT)->color == RED) {
      node = rotateRight(node);
    }
    
    if (node->getLeaf(RIGHT) == NULL) {
      return NULL;
    }
    
    if ((node->getLeaf(RIGHT)->color != RED) &&
        (node->getLeaf(RIGHT)->getLeaf(LEFT)->color != RED)) {
      node = moveViolationRight(node);
    }
    
    node->setLeaf(LEFT, removeMax(node->getLeaf(LEFT)));
    
    return repair(node);
    
  }
  
  template <class EntryType, class KeyType>
  void LLRB_Tree<EntryType,KeyType>::removeMin() {
    
    this->treeRoot = removeMax(this->treeRoot);
    
    this->treeRoot->color = BLACK;
    
  }
  
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::removeMin(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    if (node->getLeaf(LEFT) == NULL) {
      return NULL;
    }
    
    if ((node->getLeaf(LEFT)->color != RED) &&
        (node->getLeaf(LEFT)->getLeaf(LEFT)->color != RED)) {
      return node->getLeaf(RIGHT);
    }
    
    if ((node->getLeaf(RIGHT)->color != RED) &&
        (node->getLeaf(RIGHT)->getLeaf(LEFT)->color != RED)) {
      node = moveViolationLeft(node);
    }
    
    node->setLeaf(LEFT, removeMin(node->getLeaf(LEFT)));
    
    return repair(node);
    
  }
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::remove(LLRB_TreeNode<EntryType,KeyType> *node, KeyType key) {
    
    char cmp = (key == node->key) ? 0 : (key < node->key) ? -1 : 1;
    
    if (cmp == -1) {
      if ((node->getLeaf(LEFT)->color != RED) &&
          (node->getLeaf(LEFT)->getLeaf(LEFT)->color != RED)) {
        moveViolationLeft(node);
      }
      node->setLeaf(LEFT, remove(node->getLeaf(LEFT), key));
    } else {
      if (node->getLeaf(LEFT)->color == RED) {
        node = rotateRight(node);
        node->color = node->right->color;
        node->right->color = RED;      }
      
      if ((cmp == 0) && (node->getLeaf(RIGHT)->getLeaf(LEFT)->color != RED)) {
        return NULL;
      }
      
      if ((node->getLeaf(RIGHT)->color != RED) &&
          (node->getLeaf(RIGHT)->getLeaf(LEFT)->color)) {
        node = moveViolationRight(node);
      }
      
      if (cmp == 0) {
        LLRB_TreeNode<EntryType,KeyType> *tNode = rLeftSuccesor(node->getLeaf(RIGHT));
        node->key = tNode->key;
        node->data = tNode->data;
        node->setLeaf(RIGHT, removeMin(node->getLeaf(RIGHT)));
      } else {
        node->setLeaf(RIGHT, remove(node->getLeaf(RIGHT), key));
      }
    }
    
    return repair(node);
  }
  
  
}



#endif
