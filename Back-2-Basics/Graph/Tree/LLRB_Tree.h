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
  
  template <class EntryType, class KeyType, class TreeNodeType=LLRB_TreeNode<EntryType,KeyType>>
  class LLRB_Tree : public Tree<TreeNodeType,EntryType,KeyType>
  {
    
  private:
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
    
    TreeNodeType *removeMin(TreeNodeType *);
    TreeNodeType *removeMax(TreeNodeType *);
    TreeNodeType *remove(TreeNodeType *, KeyType);
    
  public:
    LLRB_Tree();
    
    void insert(EntryType n, KeyType key);
    void removeMax();
    void removeMin();
    void remove(KeyType);
    void updateKey(KeyType, KeyType, uint64_t);
    void update(EntryType, KeyType, uint64_t);
    
  };
  
  template <class EntryType, class KeyType, class TreeNodeType>
  LLRB_Tree<EntryType,KeyType,TreeNodeType>::LLRB_Tree() {
    
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::updateKey(KeyType oldKey, KeyType newKey, uint64_t instance) {
    // TODO
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::update(EntryType n, KeyType newKey, uint64_t instance) {
    // TODO
  }
  
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::insert(EntryType data, KeyType key) {
    
    this->treeRoot = insert(this->treeRoot, data, key);
    this->treeRoot->color = BLACK;
    
    this->nullNode->setLeft(this->nullNode, this->treeRoot);
    this->nullNode->setRight(this->nullNode, this->treeRoot);
    
  }
  
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::insert(TreeNodeType *node, EntryType data, KeyType key) {
    
    if (node == this->nullNode) {
      node = new LLRB_TreeNode<EntryType, KeyType>();
      node->setLeft(node, this->nullNode);
      node->setRight(node, this->nullNode);
      node->data = data;
      node->key = key;
      
      node->color = RED;
      
      this->numNodes++;
      
      return node;
    }
    
    if ((node->leftOf(node)->color == RED) &&
        (node->rightOf(node)->color == RED)) {
      recolor(node);
    }
    
    if (key == node->key) {
      node->data = data;
      return node;
    }
    
    if (key < node->key) {
      node->setLeft(node, insert(node->leftOf(node), data, key));
    } else {
      node->setRight(node, insert(node->rightOf(node), data, key));
    }
    
    if ((node->rightOf(node)->color == RED) &&
        (node->leftOf(node)->color != RED)) {
      node = rotateLeft(node);
    }
    
    if ((node->leftOf(node)->color == RED) &&
        (node->leftOf(node->leftOf(node))->color == RED)) {
      node = rotateRight(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::repair(TreeNodeType *node) {
    if (node->rightOf(node)->color == RED) {
      node = rotateLeft(node);
    }
    
    if ((node->leftOf(node)->color == RED) &&
        (node->leftOf(node->leftOf(node))->color == RED)) {
      node = rotateRight(node);
    }
    
    if ((node->leftOf(node)->color == RED) &&
        (node->rightOf(node)->color == RED)) {
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::recolor(TreeNodeType *node) {
    
    node->color = !(node->color);
    
    node->leftOf(node)->color = !(node->leftOf(node)->color);
    node->rightOf(node)->color = !(node->rightOf(node)->color);
    
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
    
    if (node->leftOf(node->leftOf(node))->color == RED) {
      node = rotateRight(node);
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::moveViolationLeft(TreeNodeType *node) {
    
    recolor(node);
    
    if (node->leftOf(node->rightOf(node))->color == RED) {
      node->setRight(node, rotateRight(node->rightOf(node)));
      node = rotateRight(node);
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::removeMax() {
    
    this->treeRoot = removeMax(this->treeRoot);
    
    this->treeRoot->color = BLACK;
    
  }
  
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::removeMax(TreeNodeType *node) {
    
    if (node->leftOf(node)->color == RED) {
      node = rotateRight(node);
    }
    
    if (node->rightOf(node) == this->nullNode) {
      delete node;
      this->numNodes--;
      return this->nullNode;
    }
    
    if ((node->rightOf(node)->color != RED) &&
        (node->leftOf(node->rightOf(node)) ->color != RED)) {
      node = moveViolationRight(node);
    }
    
    node->setLeft(node, removeMax(node->leftOf(node)));
    
    return repair(node);
    
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::removeMin() {
    
    this->treeRoot = removeMin(this->treeRoot);
    
    this->treeRoot->color = BLACK;
    
  }
  
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::removeMin(TreeNodeType *node) {
    
    if (node->leftOf(node) == this->nullNode) {
      delete node;
      this->numNodes--;
      return this->nullNode;
    }
    
    if ((node->leftOf(node)->color != RED) &&
        (node->leftOf(node->leftOf(node))->color != RED)) {
      return node->rightOf(node);
    }
    
    if ((node->rightOf(node)->color != RED) &&
        (node->leftOf(node->rightOf(node))->color != RED)) {
      node = moveViolationLeft(node);
    }
    
    node->setLeft(node, removeMin(node->leftOf(node)));
    
    return repair(node);
    
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::remove(TreeNodeType *node, KeyType key) {
    
    char cmp = (key == node->key) ? 0 : (key < node->key) ? -1 : 1;
    
    if (cmp == -1) {
      if ((node->leftOf(node)->color != RED) &&
          (node->leftOf(node->leftOf(node))->color != RED)) {
        moveViolationLeft(node);
      }
      node->setLeft(node, remove(node->leftOf(node), key));
    } else {
      if (node->leftOf(node)->color == RED) {
        node = rotateRight(node);
        node->color = node->rightOf(node)->color;
        node->rightOf(node)->color = RED;
      }
      
      if ((cmp == 0) && ((node->rightOf(node) == this->nullNode) || (node->leftOf(node->rightOf(node))->color != RED))) {
        delete node;
        this->numNodes--;
        return this->nullNode;
      }
      
      if ((node->rightOf(node)->color != RED) &&
          (node->leftOf(node->rightOf(node))->color)) {
        node = moveViolationRight(node);
      }
      
      if (cmp == 0) {
        TreeNodeType *tNode = this->min(node->rightOf(node));
        node->key = tNode->key;
        node->data = tNode->data;
        node->setRight(node, removeMin(node->rightOf(node)));
      } else {
        node->setRight(node, remove(node->rightOf(node), key));
      }
    }
    
    return repair(node);
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::remove(KeyType key) {
    this->treeRoot = remove(this->treeRoot, key);
    this->treeRoot->color = BLACK;
    
    this->nullNode->setLeft(this->nullNode, this->treeRoot);
    this->nullNode->setRight(this->nullNode, this->treeRoot);
  }
  
  
}



#endif
