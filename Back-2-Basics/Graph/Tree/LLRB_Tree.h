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
    
    bool color = RED;
    
    LLRB_TreeNode() {
      
    }
  };
  
  template <class EntryType, class KeyType, class TreeNodeType=LLRB_TreeNode<EntryType,KeyType>>
  class LLRB_Tree : public Tree<TreeNodeType,EntryType,KeyType>
  {
    
  private:
    
    const bool uniqueKeys;
    /*
     static inline void setLeft(TreeNodeType *, TreeNodeType *);
     static inline void setRight(TreeNodeType *, TreeNodeType *);
     
     static inline TreeNodeType *left(TreeNodeType *);
     static inline TreeNodeType *right(TreeNodeType *);
     */
    
    static TreeNodeType *repair(TreeNodeType *);
    static void recolor(TreeNodeType *);
    static TreeNodeType *rotateLeft(TreeNodeType *);
    static TreeNodeType *rotateRight(TreeNodeType *);
    
    TreeNodeType * insert(TreeNodeType *, EntryType, KeyType);
    
    static TreeNodeType *moveViolationRight(TreeNodeType *);
    static TreeNodeType *moveViolationLeft(TreeNodeType *);
    
    TreeNodeType *removeMin(TreeNodeType *, EntryType);
    TreeNodeType *removeMax(TreeNodeType *, EntryType);
    TreeNodeType *remove(TreeNodeType *, EntryType, KeyType);
    
  public:
    LLRB_Tree() : uniqueKeys(true) {
      
    }
    
    LLRB_Tree(bool uniqueKeys) {
      this->uniqueKeys = uniqueKeys;
    }
    
    static bool isRed(TreeNodeType *node) {
      return ((node != NULL) && (node->color == RED)) ? true : false;
    }
    
    void insert(EntryType, KeyType);
    void removeMax(EntryType);
    void removeMin(EntryType);
    void remove(EntryType, KeyType);
    //void updateKey(KeyType, KeyType, uint64_t);
    //void update(EntryType, KeyType, uint64_t);
    
  };
  
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::insert(EntryType data, KeyType key) {
    
    this->treeRoot = insert(this->treeRoot, data, key);
    this->treeRoot->color = BLACK;
  }
  
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::insert(TreeNodeType *node, EntryType data, KeyType key) {
    
    if (node == NULL) {
      node = new LLRB_TreeNode<EntryType, KeyType>();
      node->setLeft(node, NULL);
      node->setRight(node, NULL);
      node->data = data;
      node->key = key;
      
      node->color = RED;
      
      this->numNodes++;
      
      return node;
    }
    
    if ((isRed(node->leftOf(node))) &&
        (isRed(node->rightOf(node)))) {
      recolor(node);
    }
    
    if ((uniqueKeys) && (key == node->key)) {
      node->data = data;
      return node;
    }
    
    /* Go right if equal to conserve time order .
     * Also makes removal slightly easier.
     */
    if (key < node->key) {
      node->setLeft(node, insert(node->leftOf(node), data, key));
    } else {
      node->setRight(node, insert(node->rightOf(node), data, key));
    }
    
    if ((isRed(node->rightOf(node))) &&
        (!isRed(node->leftOf(node)))) {
      node = rotateLeft(node);
    }
    
    if ((isRed(node->leftOf(node))) &&
        (isRed(node->leftOf(node->leftOf(node))))) {
      node = rotateRight(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::repair(TreeNodeType *node) {
    if (isRed(node->rightOf(node))) {
      node = rotateLeft(node);
    }
    
    if ((isRed(node->leftOf(node))) &&
        (isRed(node->leftOf(node->leftOf(node))))) {
      node = rotateRight(node);
    }
    
    if ((isRed(node->leftOf(node))) &&
        (isRed(node->rightOf(node)))) {
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::recolor(TreeNodeType *node) {
    
    node->color = !(node->color);
    
    if (node->leftOf(node) != NULL) {
      node->leftOf(node)->color = !(node->leftOf(node)->color);
    }
    
    if (node->rightOf(node)) {
      node->rightOf(node)->color = !(node->rightOf(node)->color);
    }
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::rotateLeft(TreeNodeType *node) {
    
    TreeNodeType *right = node->rightOf(node);
    node->setRight(node, right->leftOf(right));
    right->setLeft(right, node);
    right->color = right->leftOf(right)->color;
    right->leftOf(right)->color = RED;
    
    return right;
    
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::rotateRight(TreeNodeType *node) {
    
    TreeNodeType *left = node->leftOf(node);
    node->setLeft(node, left->rightOf(left));
    left->setRight(left, node);
    left->color = left->rightOf(left)->color;
    left->rightOf(left)->color = RED;
    
    return left;
    
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::moveViolationRight(TreeNodeType *node) {
    
    recolor(node);
    
    if (isRed(node->leftOf(node->leftOf(node)))) {
      node = rotateRight(node);
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::moveViolationLeft(TreeNodeType *node) {
    
    recolor(node);
    
    if (isRed(node->leftOf(node->rightOf(node)))) {
      node->setRight(node, rotateRight(node->rightOf(node)));
      node = rotateRight(node);
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::removeMax(EntryType victimData) {
    
    this->treeRoot = removeMax(this->treeRoot, victimData);
    
    this->treeRoot->color = BLACK;
    
  }
  
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::removeMax(TreeNodeType *node, EntryType victimData) {
    
    if (isRed(node->leftOf(node))) {
      node = rotateRight(node);
    }
    
    if (node->rightOf(node) == NULL) {
      return NULL;
    }
    
    if ((!isRed(node->rightOf(node))) &&
        (!isRed(node->leftOf(node->rightOf(node))))) {
      node = moveViolationRight(node);
    }
    
    node->setLeft(node, removeMax(node->leftOf(node),victimData));
    
    return repair(node);
    
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::removeMin(EntryType victimData) {
    
    this->treeRoot = removeMin(this->treeRoot, victimData);
    
    this->treeRoot->color = BLACK;
    
  }
  
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::removeMin(TreeNodeType *node, EntryType victimData) {
    
    if (node->leftOf(node) == NULL) {
      return NULL;
    }
    
    if ((!isRed(node->leftOf(node))) &&
        (!isRed(node->leftOf(node->leftOf(node))))) {
      return node->rightOf(node);
    }
    
    if ((!isRed(node->rightOf(node))) &&
        (!isRed(node->leftOf(node->rightOf(node))))) {
      node = moveViolationLeft(node);
    }
    
    node->setLeft(node, removeMin(node->leftOf(node), victimData));
    
    return repair(node);
    
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::remove(TreeNodeType *node, EntryType victimData, KeyType key) {
    
    if (node == NULL) {
      return NULL;
    }
    
    char cmp = (key == node->key) ? 0 : (key < node->key) ? -1 : 1;
    
    if (cmp == -1) {
      if ((!isRed(node->leftOf(node))) &&
          (!isRed(node->leftOf(node->leftOf(node))))) {
        node = moveViolationLeft(node);
      }
      node->setLeft(node, remove(node->leftOf(node), victimData, key));
    } else {
      if (isRed(node->leftOf(node))) {
        node = rotateRight(node);
        node->color = node->rightOf(node)->color;
        node->rightOf(node)->color = RED;
      }
      
      if ((cmp == 0) && (node->rightOf(node) == NULL)) {
        if (uniqueKeys || (victimData == node->data)) {
          this->numNodes--;
          return NULL;
        }
      }
      
      if ((!isRed(node->rightOf(node))) &&
          (!isRed(node->leftOf(node->rightOf(node))))) {
        node = moveViolationRight(node);
      }
      
      if ((cmp == 0) && (uniqueKeys || (victimData == node->data))) {
        node->data = this->search(node->rightOf(node), this->min(node->rightOf(node))->key);
        node->key = this->min(node->rightOf(node))->key;
        node->setRight(node, removeMin(node->rightOf(node), victimData));
      } else {
        node->setRight(node, remove(node->rightOf(node), victimData, key));
      }
    }
    
    return repair(node);
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::remove(EntryType data, KeyType key) {
    this->treeRoot = remove(this->treeRoot, data, key);
    
    if (this->treeRoot != NULL) {
      this->treeRoot->color = BLACK;
    }
  }
  
  
}



#endif
