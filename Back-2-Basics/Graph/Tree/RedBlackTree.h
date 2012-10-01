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
  
  template <class EntryType, class KeyType>
  class RedBlackTreeNode : public TreeNode
  {
  private:
    enum Color {
      RED,
      BLACK
    };
    
    Color color = RED;
    
    RedBlackTreeNode() {
      TreeNode(2);
    }
  }
  
  template <class EntryType, class KeyType>
  class RedBlackTree : public Tree<RedBlackTreeNode,EntryType,KeyType>
  {
    
  private:
    
    bool isSon(RedBlackTreeNode *entry);
    bool isDaughter(RedBlackTreeNode *entry);
    
    void recolor(RedBlackTreeNode *node);
    void rotateLeft(RedBlackTreeNode *node);
    void rotateRight(RedBlackTreeNode *node);
    
  public:
    RedBlackTree();
    
    void insert(EntryType n, KeyType key);    
    
  };
  
  
  template <class EntryType, class KeyType>
  RedBlackTree<EntryType,KeyType>::RedBlackTree() {
  
  }
  
  template <class EntryType, class KeyType>
  bool RedBlackTree<EntryType,KeyType>::isSon(RedBlackTreeNode *node) {
    
    if (node->parent == NULL) {
      return false;
    }
    
    return (node->parent->getChild(SON) == node) ? true : false;
  }
  
  template <class EntryType, class KeyType>
  bool RedBlackTree<EntryType,KeyType>::isDaughter(RedBlackTreeNode *node) {
    
    if (node->parent == NULL) {
      return false;
    }
    
    return (node->parent->getChild(DAUGHTER) == node) ? true : false;
  }
  
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::insert(EntryType n, KeyType key) {
    
    RedBlackTreeNode *node;
    
    cout << "inserting tree node\n";
    
    node = this->insert_r(n,key);
    
    cout << "maintaining red/black rules.\n";
    
    while ((node != NULL) && (node->parent != NULL) && (node->parent->color == RED)) {
      cout << "parent is red\n";
      
      if (isSon(node->parent)) {
        RedBlackTreeNode *aunt = node->parent->parent->getChild(DAUGHTER);
        
        if (aunt->color == RED) {
          cout << "aunt is also red, just recoloring\n";
          recolor(node);
        } else {
          if (isDaughter(node)) {
            cout << "this is daughter, rotating left\n";
            rotateLeft(node);
          }
          cout << "rotating right\n";
          rotateRight(node);
        }
      } else if (isDaughter(node->parent)) {
        RedBlackTreeNode *uncle = node->parent->parent->getChild(SON);
        if (uncle->color == RED) {
          cout << "uncle is also red, just recoloring\n";
          recolor(node);
        } else {
          if (isSon(node)) {
            cout << "this is son, rotating right\n";
            rotateRight(node);
          }
          cout << "rotating left\n";
          rotateLeft(node);
        }
      }
      
      cout << "Moving up tree\n";
      node = node->parent;
    }
    
    
    this->getTreeRoot->color = BLACK;
    
    printRBTree(0);
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::recolor(RedBlackTreeNode *node) {
    
    node->parent->parent->color = RED;
    
    node->parent->parent->getChild(SON)->color = BLACK;
    node->parent->parent->getChild(DAUGHTER)->color = BLACK;
    
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::rotateLeft(RedBlackTreeNode *node) {
    
    RedBlackTreeNode *parent = node->parent;
    RedBlackTreeNode *grandParent = parent->parent;
        
    if (parent == this->sonOf(grandParent)) {
      if (!isSon(nodePosition)) {
        RedBlackTreeNode *son = node->getChild(SON);
        
        /* Staightening */
        node->setChild(SON, parent);
        node->parent = grandParent;
        grandParent->setChild(SON, node);
        parent->setChild(DAUGHTER, son);
      }
    } else {
      if (!isSon(nodePosition)) {
        RedBlackTreeNode *brother = parent->getChild(SON));
        
        /* Flattening */
        parent->setChild(SON,grandParent);
        parent->parent = grandParent->parent;
        grandParent->parent = parent;
        grandParent->setChild(DAUGHTER, brother);
        
      }
    }
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::rotateRight(RedBlackTreeNode *node) {
    
    RedBlackTreeNode *parent = node->parent;
    RedBlackTreeNode *grandParent = parent->parent;
    
    if (isSon(parent)) {
      if (isSon(node)) {
        RedBlackTreeNode *sister = parent->getChild(DAUGHTER));
        
        /* Flattening */
        parent->setChild(DAUGHTER,grandParent);
        parent->parent = grandParent->parent
        grandParent->parent = parent;
        grandParent->setChild(SON, sister);
        
      }
    } else {
      if (isSon(node)) {
        RedBlackTreeNode *daughter = node->getChild(DAUGHTER));
        
        /* Staightening */
        node->setChild(DAUGHTER,parent);
        node->parent = grandParent;
        grandParent->setChild(DAUGHTER,node);
        parent->getChild(SON,daughter);
      }
    }
    
  }
  
}


#endif
