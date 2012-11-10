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
  
  static const bool RED = true;
  static const bool BLACK = false;
  
  template <class DataType, class KeyType>
  class LLRB_TreeNode : public TreeNode<LLRB_TreeNode<DataType,KeyType>,DataType,KeyType>
  {
    
  public:
    
    bool color = BLACK;
    
    LLRB_TreeNode() {
      
    }
  };
  
  template <class EntryType, class KeyType>
  class LLRB_Tree : public Tree<LLRB_TreeNode<EntryType,KeyType>,EntryType,KeyType>
  {
    
  private:
    
    static bool isLeft(LLRB_TreeNode<EntryType,KeyType> *);
    static bool isRight(LLRB_TreeNode<EntryType,KeyType> *);
    
    static LLRB_TreeNode<EntryType,KeyType> *repair(LLRB_TreeNode<EntryType,KeyType> *);
    static void recolor(LLRB_TreeNode<EntryType,KeyType> *);
    static LLRB_TreeNode<EntryType,KeyType> *rotateLeft(LLRB_TreeNode<EntryType,KeyType> *);
    static LLRB_TreeNode<EntryType,KeyType> *rotateRight(LLRB_TreeNode<EntryType,KeyType> *);
    
    LLRB_TreeNode<EntryType,KeyType> * insert(LLRB_TreeNode<EntryType,KeyType> *, EntryType, KeyType);
    
    static LLRB_TreeNode<EntryType,KeyType> *moveViolationRight(LLRB_TreeNode<EntryType,KeyType> *);
    static LLRB_TreeNode<EntryType,KeyType> *moveViolationLeft(LLRB_TreeNode<EntryType,KeyType> *);
    
    LLRB_TreeNode<EntryType,KeyType> *removeMin(LLRB_TreeNode<EntryType,KeyType> *);
    LLRB_TreeNode<EntryType,KeyType> *removeMax(LLRB_TreeNode<EntryType,KeyType> *);
    LLRB_TreeNode<EntryType,KeyType> *remove(LLRB_TreeNode<EntryType,KeyType> *, KeyType);
    
  public:
    LLRB_Tree();
    
    void insert(EntryType n, KeyType key);
    void removeMax();
    void removeMin();
    void remove(KeyType);
    void updateKey(KeyType, KeyType, uint64_t);
    void update(EntryType, KeyType, uint64_t);
    
  };
  
  template <class EntryType, class KeyType>
  LLRB_Tree<EntryType,KeyType>::LLRB_Tree() {
    
  }
  
  template <class EntryType, class KeyType>
  bool LLRB_Tree<EntryType,KeyType>::isRight(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    return (node->parent->right == node) ? true : false;
  }
  
  template <class EntryType, class KeyType>
  bool LLRB_Tree<EntryType,KeyType>::isLeft(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    return (node->parent->left == node) ? true : false;
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
    
    this->treeRoot = insert(this->treeRoot, data, key);
    this->treeRoot->color = BLACK;
    
  }
  
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::insert(LLRB_TreeNode<EntryType,KeyType> *node, EntryType data, KeyType key) {
    
    if (node == &this->nullNode) {
      node = new LLRB_TreeNode<EntryType, KeyType>();
      node->left =  &this->nullNode;
      node->right =  &this->nullNode;
      node->data = data;
      node->key = key;
      
      node->color = RED;
      
      this->numNodes++;
      
      return node;
    }
    
    if ((node->left->color == RED) &&
        (node->right->color == RED)) {
      recolor(node);
    }
    
    if (key == node->key) {
      node->data = data;
    }
    
    if (key < node->key) {
      node->left = insert(node->left, data, key);
    } else {
      node->right = insert(node->right, data, key);
    }
    
    if ((node->right->color == RED) &&
        (node->left->color != RED)) {
      node = rotateLeft(node);
    }
    
    if ((node->left->color == RED) &&
        (node->left->left->color == RED)) {
      node = rotateRight(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::repair(LLRB_TreeNode<EntryType,KeyType> *node) {
    if (node->right->color == RED) {
      node = rotateLeft(node);
    }
    
    if ((node->left->color == RED) &&
        (node->left->left->color == RED)) {
      node = rotateRight(node);
    }
    
    if ((node->left->color == RED) &&
        (node->right->color == RED)) {
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType>
  void LLRB_Tree<EntryType,KeyType>::recolor(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    node->color = !(node->color);
    
    node->left->color = !(node->left->color);
    node->right->color = !(node->right->color);
    
  }
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::rotateLeft(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    LLRB_TreeNode<EntryType,KeyType> *right = node->right;
    node->right = right->left;
    right->left = node;
    right->color = right->left->color;
    right->left->color = RED;
    
    return right;
    
  }
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::rotateRight(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    LLRB_TreeNode<EntryType,KeyType> *left = node->left;
    node->left = left->right;
    left->right = node;
    left->color = left->right->color;
    left->right->color = RED;
    
    return left;
    
  }
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::moveViolationRight(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    recolor(node);
    
    if (node->left->left->color == RED) {
      node = rotateRight(node);
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::moveViolationLeft(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    recolor(node);
    
    if (node->right->left->color == RED) {
      node->right = rotateRight(node->right);
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
    
    if (node->left->color == RED) {
      node = rotateRight(node);
    }
    
    if (node->right == &this->nullNode) {
      delete node->data;
      delete node;
      this->numNodes--;
      return &this->nullNode;
    }
    
    if ((node->right->color != RED) &&
        (node->right->left->color != RED)) {
      node = moveViolationRight(node);
    }
    
    node->left = removeMax(node->left);
    
    return repair(node);
    
  }
  
  template <class EntryType, class KeyType>
  void LLRB_Tree<EntryType,KeyType>::removeMin() {
    
    this->treeRoot = removeMin(this->treeRoot);
    
    this->treeRoot->color = BLACK;
    
  }
  
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::removeMin(LLRB_TreeNode<EntryType,KeyType> *node) {
    
    if (node->left == &this->nullNode) {
      delete node->data;
      delete node;
      this->numNodes--;
      return &this->nullNode;
    }
    
    if ((node->left->color != RED) &&
        (node->left->left->color != RED)) {
      return node->right;
    }
    
    if ((node->right->color != RED) &&
        (node->right->left->color != RED)) {
      node = moveViolationLeft(node);
    }
    
    node->left = removeMin(node->left);
    
    return repair(node);
    
  }
  
  template <class EntryType, class KeyType>
  LLRB_TreeNode<EntryType,KeyType> *LLRB_Tree<EntryType,KeyType>::remove(LLRB_TreeNode<EntryType,KeyType> *node, KeyType key) {
    
    char cmp = (key == node->key) ? 0 : (key < node->key) ? -1 : 1;
    
    if (cmp == -1) {
      if ((node->left->color != RED) &&
          (node->left->left->color != RED)) {
        moveViolationLeft(node);
      }
      node->left = remove(node->left, key);
    } else {
      if (node->left->color == RED) {
        node = rotateRight(node);
        node->color = node->right->color;
        node->right->color = RED;
      }
      
      if ((cmp == 0) && ((node->right == &this->nullNode) || (node->right->left->color != RED))) {
        delete node->data;
        delete node;
        this->numNodes--;
        return &this->nullNode;
      }
      
      if ((node->right->color != RED) &&
          (node->right->left->color)) {
        node = moveViolationRight(node);
      }
      
      if (cmp == 0) {
        LLRB_TreeNode<EntryType,KeyType> *tNode = this->min(node->right);
        node->key = tNode->key;
        node->data = tNode->data;
        node->right = removeMin(node->right);
      } else {
        node->right = remove(node->right, key);
      }
    }
    
    return repair(node);
  }
  
  template <class EntryType, class KeyType>
  void LLRB_Tree<EntryType,KeyType>::remove(KeyType key) {
    this->treeRoot = remove(this->treeRoot, key);
  }
  
  
}



#endif
