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
    RED,
    BLACK
  };

  
  template <class DataType, class KeyType>
  class RedBlackTreeNode : public TreeNode<RedBlackTreeNode<DataType,KeyType>,DataType,KeyType>
  {
  public:
    
    Color color = RED;
    
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
    void rotateLeft(RedBlackTreeNode<EntryType,KeyType> *node);
    void rotateRight(RedBlackTreeNode<EntryType,KeyType> *node);
    
  public:
    RedBlackTree();
    
    void insert(EntryType n, KeyType key);    
    
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
    
    RedBlackTreeNode<EntryType,KeyType> *node;
        
    node = this->insert_r(n,key);
    
    while ((node != NULL) && (node->parent != NULL) && (node->parent->color == RED)) {
      
      if (isSon(node->parent)) {
        RedBlackTreeNode<EntryType,KeyType> *aunt = node->parent->parent->getLeaf(DAUGHTER);
        
        if (aunt->color == RED) {
          recolor(node);
        } else {
          if (isDaughter(node)) {
            rotateLeft(node);
          }
          rotateRight(node);
        }
      } else if (isDaughter(node->parent)) {
        RedBlackTreeNode<EntryType,KeyType> *uncle = node->parent->parent->getLeaf(SON);
        if (uncle->color == RED) {
          recolor(node);
        } else {
          if (isSon(node)) {
            rotateRight(node);
          }
          rotateLeft(node);
        }
      }
      
      node = node->parent;
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
  void RedBlackTree<EntryType,KeyType>::rotateLeft(RedBlackTreeNode<EntryType,KeyType> *node) {
    
    RedBlackTreeNode<EntryType,KeyType> *parent = node->parent;
    RedBlackTreeNode<EntryType,KeyType> *grandParent = parent->parent;
        
    if (parent == grandParent->getLeaf(SON)) {
      if (!isSon(node)) {
        RedBlackTreeNode<EntryType,KeyType> *son = node->getLeaf(SON);
        
        /* Staightening */
        node->setLeaf(SON, parent);
        node->parent = grandParent;
        grandParent->setLeaf(SON, node);
        parent->setLeaf(DAUGHTER, son);
      }
    } else {
      if (!isSon(node)) {
        RedBlackTreeNode<EntryType,KeyType> *brother = parent->getLeaf(SON);
        
        /* Flattening */
        parent->setLeaf(SON,grandParent);
        parent->parent = grandParent->parent;
        grandParent->parent = parent;
        grandParent->setLeaf(DAUGHTER, brother);
        
      }
    }
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::rotateRight(RedBlackTreeNode<EntryType,KeyType> *node) {
    
    RedBlackTreeNode<EntryType,KeyType> *parent = node->parent;
    RedBlackTreeNode<EntryType,KeyType> *grandParent = parent->parent;
    
    if (isSon(parent)) {
      if (isSon(node)) {
        RedBlackTreeNode<EntryType,KeyType> *sister = parent->getLeaf(DAUGHTER);
        
        /* Flattening */
        parent->setLeaf(DAUGHTER,grandParent);
        parent->parent = grandParent->parent;
        grandParent->parent = parent;
        grandParent->setLeaf(SON, sister);
        
      }
    } else {
      if (isSon(node)) {
        RedBlackTreeNode<EntryType,KeyType> *daughter = node->getLeaf(DAUGHTER);
        
        /* Staightening */
        node->setLeaf(DAUGHTER, parent);
        node->parent = grandParent;
        grandParent->setLeaf(DAUGHTER,node);
        parent->setLeaf(SON,daughter);
      }
    }
    
  }
  
}


#endif
