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

#include <assert.h>
#include "Tree.h"

namespace Tree {
  
#define TreeType Tree<TreeNodeType,EntryType,KeyType>
  
  static const bool RED = true;
  static const bool BLACK = false;
  
  template <class DataType, class KeyType>
  class LLRB_TreeNode : public TreeNode<LLRB_TreeNode<DataType,KeyType>,DataType,KeyType>
  {
    
  public:
    
    bool color;
    
    LLRB_TreeNode(DataType data, KeyType key) {
      this->data = data;
      this->key = key;
      color = RED;
    }
    
  };
  
  template <class EntryType, class KeyType, class TreeNodeType=LLRB_TreeNode<EntryType,KeyType>>
  class LLRB_Tree : public Tree<TreeNodeType,EntryType,KeyType>
  {
    
  private:
    
    const bool uniqueKeys;
    
    static TreeNodeType *repair(TreeNodeType *);
    static void recolor(TreeNodeType *);
    static TreeNodeType *rotateLeft(TreeNodeType *);
    static TreeNodeType *rotateRight(TreeNodeType *);
    
    TreeNodeType * insert(TreeNodeType *, EntryType, KeyType);
    
    static TreeNodeType *moveViolationRight(TreeNodeType *);
    static TreeNodeType *moveViolationLeft(TreeNodeType *);
    
    TreeNodeType *removeMin(TreeNodeType *);
    TreeNodeType *removeMax(TreeNodeType *);
    TreeNodeType *remove(TreeNodeType *, EntryType, KeyType);
    
  public:
    LLRB_Tree() : uniqueKeys(true) {
      this->treeRoot->color = BLACK;
    }
    
    LLRB_Tree(bool uniqueKeys) : uniqueKeys(uniqueKeys) {
      this->treeRoot->color = BLACK;
    }
    
    static inline bool isRed(TreeNodeType *node) {
      return ((node->color == RED) && (node != TreeType::leftOf(node))) ? true : false;
    }
    
    void insert(EntryType, KeyType);
    void removeMax();
    void removeMin();
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
    
    if (node == this->nullNode) {
      node = new LLRB_TreeNode<EntryType, KeyType>(data, key);
      TreeType::setLeft(node, this->nullNode);
      TreeType::setRight(node, this->nullNode);
      this->numNodes++;
      
      return node;
    }
    
#if 0
    if ((isRed(TreeType::leftOf(node))) &&
        (isRed(TreeType::rightOf(node)))) {
      recolor(node);
    }
#endif
    
    if ((uniqueKeys) && (key == node->key)) {
      node->data = data;
    } else if (key < node->key) {
      TreeType::setLeft(node, insert(TreeType::leftOf(node), data, key));
    } else {
      TreeType::setRight(node, insert(TreeType::rightOf(node), data, key));
    }
    
    if ((isRed(TreeType::rightOf(node))) &&
        (!isRed(TreeType::leftOf(node)))) {
      node = rotateLeft(node);
    }
    
    if ((isRed(TreeType::leftOf(node))) &&
        (isRed(TreeType::leftOf(TreeType::leftOf(node))))) {
      node = rotateRight(node);
    }
    
#if 1
    if ((isRed(TreeType::leftOf(node))) &&
        (isRed(TreeType::rightOf(node)))) {
      recolor(node);
    }
#endif
    
    return node;
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::repair(TreeNodeType *node) {
    if (isRed(TreeType::rightOf(node))) {
      node = rotateLeft(node);
    }
    
    if (isRed(TreeType::leftOf(node)) &&
        isRed(TreeType::leftOf(TreeType::leftOf(node)))) {
      node = rotateRight(node);
    }
    
    if ((isRed(TreeType::leftOf(node))) &&
        (isRed(TreeType::rightOf(node)))) {
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::recolor(TreeNodeType *node) {
    
    node->color = !(node->color);
    
    TreeNodeType *left = TreeType::leftOf(node);
    TreeNodeType *right = TreeType::rightOf(node);
    
    left->color = !(left->color);
    right->color = !(right->color);
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::rotateLeft(TreeNodeType *node) {
    
    TreeNodeType *right = TreeType::rightOf(node);
    TreeType::setRight(node, TreeType::leftOf(right));
    TreeType::setLeft(right, node);
    right->color = node->color;
    node->color = RED;
    
    return right;
    
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::rotateRight(TreeNodeType *node) {
    
    TreeNodeType *left = TreeType::leftOf(node);
    TreeType::setLeft(node, TreeType::rightOf(left));
    TreeType::setRight(left, node);
    left->color = node->color;
    node->color = RED;
    
    return left;
    
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::moveViolationRight(TreeNodeType *node) {
    
    recolor(node);
    
    if (isRed(TreeType::leftOf(TreeType::leftOf(node)))) {
      node = rotateRight(node);
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::moveViolationLeft(TreeNodeType *node) {
    
    recolor(node);
    
    if (isRed(TreeType::leftOf(TreeType::rightOf(node)))) {
      TreeType::setRight(node, rotateRight(TreeType::rightOf(node)));
      node = rotateLeft(node);
      recolor(node);
    }
    
    return node;
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::removeMax() {
    assert(this->treeRoot != this->nullNode);
    
    this->treeRoot = removeMax(this->treeRoot);
    this->treeRoot->color = BLACK;
  }
  
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::removeMax(TreeNodeType *node) {
    
    if (isRed(TreeType::leftOf(node))) {
      node = rotateRight(node);
    }
    
    if (TreeType::rightOf(node) == this->nullNode) {
      this->numNodes--;
      delete node;
      
      return this->nullNode;
    }
    
    if ((!isRed(TreeType::rightOf(node))) &&
        (!isRed(TreeType::leftOf(TreeType::rightOf(node))))) {
      node = moveViolationRight(node);
    }
    
    TreeType::setLeft(node, removeMax(TreeType::leftOf(node)));
    
    return repair(node);
    
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::removeMin() {
    assert(this->treeRoot != this->nullNode);
    
    this->treeRoot = removeMin(this->treeRoot);
    this->treeRoot->color = BLACK;
  }
  
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::removeMin(TreeNodeType *node) {
    
    if (TreeType::leftOf(node) == this->nullNode) {
      this->numNodes--;

      delete node;
      return this->nullNode;
    }
    
    if ((!isRed(TreeType::leftOf(node))) &&
        (!isRed(TreeType::leftOf(TreeType::leftOf(node))))) {
      node = moveViolationLeft(node);
    }
    
    TreeType::setLeft(node, removeMin(TreeType::leftOf(node)));
    
    return repair(node);
    
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  TreeNodeType *LLRB_Tree<EntryType,KeyType,TreeNodeType>::remove(TreeNodeType *node, EntryType victimData, KeyType key) {
    
    assert(node != this->nullNode);
    
    if (key < node->key) {
      if ((!isRed(TreeType::leftOf(node))) &&
          (!isRed(TreeType::leftOf(TreeType::leftOf(node))))) {
        node = moveViolationLeft(node);
        
        assert(node != this->nullNode);
      }
      TreeType::setLeft(node, remove(TreeType::leftOf(node), victimData, key));
    } else {
      if (isRed(TreeType::leftOf(node))) {
        node = rotateRight(node);
        
        assert(node != this->nullNode);
      }
      
      if ((key == node->key) && (TreeType::rightOf(node) == this->nullNode)) {
        if (uniqueKeys || (victimData == node->data)) {
          this->numNodes--;

          delete node;
          return this->nullNode;
        } else {
          return node;
        }
      }
      
      if ((!isRed(TreeType::rightOf(node))) &&
          (!isRed(TreeType::leftOf(TreeType::rightOf(node))))) {
        node = moveViolationRight(node);
        
        assert(node != this->nullNode);
      }
      
      if ((key == node->key) && (uniqueKeys || (victimData == node->data))) {
        TreeNodeType *tNode = this->min(TreeType::rightOf(node));
        if (tNode == this->nullNode) {
          return this->nullNode;
        }
        node->data = this->search(TreeType::rightOf(node), tNode->key)->data;
        node->key = tNode->key;
        TreeType::setRight(node, removeMin(TreeType::rightOf(node)));
      } else {
        TreeType::setRight(node, remove(TreeType::rightOf(node), victimData, key));
      }
    }
    
    return repair(node);
  }
  
  template <class EntryType, class KeyType, class TreeNodeType>
  void LLRB_Tree<EntryType,KeyType,TreeNodeType>::remove(EntryType data, KeyType key) {
    assert(this->treeRoot != this->nullNode);
    
    this->treeRoot = remove(this->treeRoot, data, key);
    this->treeRoot->color = BLACK;
  }
  
  
}



#endif
