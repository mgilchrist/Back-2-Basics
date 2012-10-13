//
//  RedBlackTree.h
//  OpenSource
//
//  Created by Marcus Gilchrist on 9/16/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#ifndef OpenSource_RedBlackTree_h
#define OpenSource_RedBlackTree_h

#include "Tree.h"

#define SON       0
#define DAUGHTER  1

namespace Graph {
  
  enum Color {
    BLACK,
    RED
  };
  
  
  template <class DataType, class KeyType>
  class RedBlackTreeNode : public TreeNode<RedBlackTreeNode<DataType,KeyType>,DataType,KeyType>
  {
  public:
    
    Color color = BLACK;
    RedBlackTreeNode<DataType,KeyType> *parent;
    
    RedBlackTreeNode() {
      TreeNode<RedBlackTreeNode<DataType,KeyType>,DataType,KeyType>(2);
    }
  };
  
  template <class EntryType, class KeyType>
  class RedBlackTree : public Tree<RedBlackTreeNode<EntryType,KeyType>,EntryType,KeyType>
  {
    
  private:
    
    bool isSon(RedBlackTreeNode<EntryType,KeyType> *entry);
    bool isDaughter(RedBlackTreeNode<EntryType,KeyType> *entry);
    
    void recolor(RedBlackTreeNode<EntryType,KeyType> *node);
    void rotateLeft_F(RedBlackTreeNode<EntryType,KeyType> *node);
    void rotateRight_F(RedBlackTreeNode<EntryType,KeyType> *node);
    void rotateLeft_S(RedBlackTreeNode<EntryType,KeyType> *node);
    void rotateRight_S(RedBlackTreeNode<EntryType,KeyType> *node);
    
    
  public:
    RedBlackTree();
    
    void insert(EntryType n, KeyType key);
    void remove(KeyType key);
    
  };
  
  
  template <class EntryType, class KeyType>
  RedBlackTree<EntryType,KeyType>::RedBlackTree() {
    
  }
  
  template <class EntryType, class KeyType>
  bool RedBlackTree<EntryType,KeyType>::isSon(RedBlackTreeNode<EntryType,KeyType> *node) {
    
    if (node->parent == NULL) {
      return false;
    }
    
    return (node->parent->getLeaf(SON) == node) ? true : false;
  }
  
  template <class EntryType, class KeyType>
  bool RedBlackTree<EntryType,KeyType>::isDaughter(RedBlackTreeNode<EntryType,KeyType> *node) {
    
    if (node->parent == NULL) {
      return false;
    }
    
    return (node->parent->getLeaf(DAUGHTER) == node) ? true : false;
  }
  
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::insert(EntryType n, KeyType key) {
    
    RedBlackTreeNode<EntryType,KeyType> *node = this->findOpening(key, this->treeRoot);
    RedBlackTreeNode<EntryType,KeyType> *parent;
    
    if (node == NULL) {
      return;
    }
    
    RedBlackTreeNode<EntryType,KeyType> *left = new RedBlackTreeNode<EntryType,KeyType>();
    RedBlackTreeNode<EntryType,KeyType> *right = new RedBlackTreeNode<EntryType,KeyType>();
    
    node->setLeaf(LEFT, left);
    node->setLeaf(RIGHT, right);
    
    left->parent = node;
    right->parent = node;
    node->data = n;
    node->key = key;
    
    node->color = RED;
    node->getLeaf(LEFT)->color = BLACK;
    node->getLeaf(RIGHT)->color = BLACK;
    
    parent = node->parent;
    
    while ((parent != NULL) && (parent->color == RED)) {
      
      if (isSon(parent)) {
        RedBlackTreeNode<EntryType,KeyType> *aunt = parent->parent->getLeaf(DAUGHTER);
        
        if (aunt->color == RED) {
          recolor(node);
        } else {
          if (isDaughter(node)) {
            rotateLeft_S(node);
            rotateRight_F(node);
          } else {
            rotateRight_F(parent);
          }
        }
      } else if (isDaughter(parent)) {
        RedBlackTreeNode<EntryType,KeyType> *uncle = parent->parent->getLeaf(SON);
        if (uncle->color == RED) {
          recolor(node);
        } else {
          if (isSon(node)) {
            rotateRight_S(node);
            rotateLeft_F(node);
          } else {
            rotateLeft_F(parent);
          }
        }
      }
      
      node = parent;
      parent = node->parent;
    }
    
    
    this->getTreeRoot()->color = BLACK;
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::recolor(RedBlackTreeNode<EntryType,KeyType> *node) {
    
    node->parent->parent->color = RED;
    
    node->parent->parent->getLeaf(SON)->color = BLACK;
    node->parent->parent->getLeaf(DAUGHTER)->color = BLACK;
    
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::rotateLeft_S(RedBlackTreeNode<EntryType,KeyType> *node) {
    
    RedBlackTreeNode<EntryType,KeyType> *parent = node->parent;
    RedBlackTreeNode<EntryType,KeyType> *grandParent = parent->parent;
    RedBlackTreeNode<EntryType,KeyType> *son = node->getLeaf(SON);
    
    /* Staightening */
    node->setLeaf(SON, parent);
    parent->parent = node;
    
    grandParent->setLeaf(SON, node);
    node->parent = grandParent;
    
    parent->setLeaf(DAUGHTER, son);
    son->parent = parent;
    
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::rotateRight_S(RedBlackTreeNode<EntryType,KeyType> *node) {
    
    RedBlackTreeNode<EntryType,KeyType> *parent = node->parent;
    RedBlackTreeNode<EntryType,KeyType> *grandParent = parent->parent;
    RedBlackTreeNode<EntryType,KeyType> *daughter = node->getLeaf(DAUGHTER);
    
    /* Staightening */
    node->setLeaf(DAUGHTER, parent);
    parent->parent = node;
    
    grandParent->setLeaf(DAUGHTER,node);
    node->parent = grandParent;
    
    parent->setLeaf(SON,daughter);
    daughter->parent = parent;
    
    
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::rotateLeft_F(RedBlackTreeNode<EntryType,KeyType> *node) {
    
    RedBlackTreeNode<EntryType,KeyType> *parent = node->parent;
    RedBlackTreeNode<EntryType,KeyType> *grandParent = parent->parent;
    
    /* Flattening */
    
    if (grandParent != NULL) {
      if (grandParent->getLeaf(SON) == parent) {
        grandParent->setLeaf(SON,node);
      } else {
        grandParent->setLeaf(DAUGHTER,node);
      }
    } else {
      this->treeRoot = node;
    }
    node->parent = grandParent;
    
    parent->setLeaf(DAUGHTER,node->getLeaf(SON));
    node->getLeaf(SON)->parent = parent;
    
    node->setLeaf(SON, parent);
    parent->parent = node;
    
    node->color = BLACK;
    parent->color = RED;
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::rotateRight_F(RedBlackTreeNode<EntryType,KeyType> *node) {
    
    RedBlackTreeNode<EntryType,KeyType> *parent = node->parent;
    RedBlackTreeNode<EntryType,KeyType> *grandParent = parent->parent;
    
    /* Flattening */
    
    if (grandParent != NULL) {
      if (grandParent->getLeaf(SON) == parent) {
        grandParent->setLeaf(SON,node);
      } else {
        grandParent->setLeaf(DAUGHTER,node);
      }
    } else {
      this->treeRoot = node;
    }

    node->parent = grandParent;
    
    parent->setLeaf(SON,node->getLeaf(DAUGHTER));
    node->getLeaf(DAUGHTER)->parent = parent;
    
    node->setLeaf(DAUGHTER, parent);
    parent->parent = node;
    
    node->color = BLACK;
    parent->color = RED;    
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::remove(KeyType key) {
    
  }
  
}


#endif
