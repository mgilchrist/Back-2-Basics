//
//  Tree.h
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

#ifndef OpenSource_Tree_h
#define OpenSource_Tree_h

#include "Graph.h"


#define LEFT  0
#define RIGHT 1

namespace Graph {
  
  template <class TreeNodeType, class DataType, class KeyType>
  class TreeNode
  {
  private:
    TreeNodeType **leafs;
    uint8_t numLeafs;
    
  public:
    DataType data = NULL;
    TreeNodeType *parent = NULL;
    
    KeyType key;
    KeyType lowestDesendant;
    
    
    TreeNode() {
      leafs = new TreeNodeType*[2];
      
      for (int ix = 0; ix < 2; ix++) {
        leafs[ix] = NULL;
      }
    }
    
    TreeNode(uint8_t numLeafs) {
      leafs = new TreeNodeType*[numLeafs];
      this->numLeafs = numLeafs;
      
      for (int ix = 0; ix < numLeafs; ix++) {
        leafs[ix] = NULL;
      }
    }
    
    ~TreeNode() {
      delete leafs;
    }
    
    TreeNodeType *getLeaf(uint8_t index) {
      return leafs[index];
    }
    
    void setLeaf(uint8_t index, TreeNodeType *tNode) {
      leafs[index] = tNode;
    }
    
    TreeNodeType *numberOfLeafs(uint8_t index) {
      return this->numLeafs;
    }
  };
  
  /* Tree */
  
  template <class TreeNodeType, class DataType, class KeyType>
  class Tree
  {
    
  private:
    TreeNodeType *treeRoot;
    
  protected:
    virtual TreeNodeType *findOpening(KeyType key, TreeNodeType *current);
    TreeNodeType *getNode(KeyType key, TreeNodeType *current);
    TreeNodeType *getTreeRoot();
    TreeNodeType *insert_r(DataType data, KeyType key);
    
  public:
    Tree();
    
    virtual void insert(DataType data, KeyType key);
    virtual void remove(KeyType key);
    virtual DataType search(KeyType key);
  };
  
  
  /* Tree */
  
  template <class TreeNodeType, class DataType, class KeyType>
  Tree<TreeNodeType,DataType,KeyType>::Tree() {
    treeRoot = new TreeNodeType();
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::findOpening(KeyType key, TreeNodeType *current) {
    
    uint8_t tmp;
    
    if (current->getLeaf(LEFT) == NULL) {
      return current;
    }
    
    if (current->key >= key) {
      if (current->lowestDesendant > key) {
        current->lowestDesendant = key;
      }
      tmp = LEFT;
      
    } else {
      tmp = RIGHT;
    }
    
    return findOpening(key, current->getLeaf(tmp));
    
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::insert_r(DataType data, KeyType key) {
    
    TreeNodeType *tNode = findOpening(key, treeRoot);
    
    TreeNodeType *left = new TreeNodeType();
    TreeNodeType *right = new TreeNodeType();
    
    left->parent = tNode;
    right->parent = tNode;
    
    tNode->setLeaf(LEFT, left);
    tNode->setLeaf(RIGHT, right);
    tNode->key = key;
    tNode->lowestDesendant = key;
    
    return tNode;
  }
  
  
  template <class TreeNodeType, class DataType, class KeyType>
  void Tree<TreeNodeType,DataType,KeyType>::insert(DataType data, KeyType key) {
    
    insert_r(data,key);
    
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  void Tree<TreeNodeType,DataType,KeyType>::remove(KeyType key) {
    
    TreeNodeType *parent, *victim, *tmp, *opening;
    
    tmp = NULL;
    victim = getNode(key, treeRoot);
    parent = victim->parent;
    
    if (parent->getLeaf(LEFT) == victim) {
      parent->setLeaf(LEFT, victim->getLeaf(RIGHT));
      if (victim->getLeaf(LEFT)->getLeaf(0) != NULL) {
        tmp = victim->getLeaf(LEFT);
      } else {
        parent->lowestDesendant = victim->getLeaf(RIGHT)->lowestDesendant;
      }
    } else {
      parent->setLeaf(RIGHT, victim->getLeaf(LEFT));
      if (victim->getLeaf(RIGHT)->getLeaf(0) != NULL) {
        tmp = victim->getLeaf(RIGHT);
      }
    }
    
    if (tmp != NULL) {
      opening = findOpening(tmp->key, victim);
      
      parent = opening->parent;
      tmp->parent = parent;
      
      if (parent->getLeaf(LEFT) == opening) {
        parent->setLeaf(LEFT, tmp);
      } else {
        parent->setLeaf(RIGHT, tmp);
      }
      
      delete opening;
    }
    
    
    delete victim;
    
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::getTreeRoot() {
    
    return treeRoot;
    
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::getNode(KeyType key, TreeNodeType *current) {
    
    uint8_t tmp;
    
    if (current->key == key) {
      return current;
    }
    
    if (key < current->lowestDesendant) {
      return NULL;
    }
    
    if (key < current->key) {
      tmp = LEFT;
    } else {
      tmp = RIGHT;
    }
    
    return getNode(key, current->getLeaf(tmp));
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  DataType Tree<TreeNodeType,DataType,KeyType>::search(KeyType key) {
    
    return getNode(key,treeRoot)->data;
  }
  
  
  
}

#endif
