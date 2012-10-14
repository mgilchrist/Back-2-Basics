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
    
  public:
    DataType data;
    KeyType key;
    
    TreeNode() {
      leafs = new TreeNodeType*[2];
      data = NULL;
      
      for (int ix = 0; ix < 2; ix++) {
        leafs[ix] = NULL;
      }
    }
    
    TreeNode(uint8_t numLeafs) {
      leafs = new TreeNodeType*[numLeafs];
      data = NULL;
      
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
  };
  
  /* Tree */
  
  template <class TreeNodeType, class DataType, class KeyType>
  class Tree
  {
    
  protected:
    TreeNodeType *treeRoot;
    
    virtual TreeNodeType *findOpening(KeyType key, TreeNodeType *current);
    TreeNodeType *insert_r(DataType data, KeyType key);
    //TreeNodeType *remove_r(KeyType key);
    TreeNodeType *getNodeMinGreaterThan(KeyType key, TreeNodeType *current);
    
    TreeNodeType *rLeftSuccesor(TreeNodeType *tNode);
    
  public:
    Tree();
    
    TreeNodeType *getNode(KeyType key, TreeNodeType *current);
    TreeNodeType *getTreeRoot();
    virtual void insert(DataType data, KeyType key);
    //virtual void remove(KeyType key);
    virtual DataType search(KeyType key);
    TreeNodeType *nextNode(TreeNodeType *tNode);
    TreeNodeType *firstNode(TreeNodeType *tNode);
    
  };
  
  
  /* Tree */
  
  template <class TreeNodeType, class DataType, class KeyType>
  Tree<TreeNodeType,DataType,KeyType>::Tree() {
    treeRoot = new TreeNodeType();
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::findOpening(KeyType key, TreeNodeType *current) {
    
    uint8_t tmp;
    
    if (current->getLeaf(0) == NULL) {
      return current;
    }
    
    if (key == current->key) {
      return NULL;
    }
    
    if (key < current->key) {
      tmp = LEFT;
    } else {
      tmp = RIGHT;
    }
    
    return findOpening(key, current->getLeaf(tmp));
    
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::insert_r(DataType data, KeyType key) {
    
    TreeNodeType *tNode = findOpening(key, treeRoot);
    
    if (tNode == NULL) {
      return NULL;
    }
    
    TreeNodeType *left = new TreeNodeType();
    TreeNodeType *right = new TreeNodeType();
    
    tNode->setLeaf(LEFT, left);
    tNode->setLeaf(RIGHT, right);
    tNode->data = data;
    tNode->key = key;
    
    return tNode;
  }
  
  
  template <class TreeNodeType, class DataType, class KeyType>
  void Tree<TreeNodeType,DataType,KeyType>::insert(DataType data, KeyType key) {
    
    insert_r(data,key);
    
  }
  
#if 0
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::remove_r(KeyType key) {
    
    TreeNodeType *parent, *victim, *tmp, *opening;
    
    tmp = NULL;
    victim = getNode(key, treeRoot);
    parent = victim->parent;
    
    if (parent->getLeaf(LEFT) == victim) {
      parent->setLeaf(LEFT, victim->getLeaf(RIGHT));
      if (victim->getLeaf(LEFT)->getLeaf(0) != NULL) {
        tmp = victim->getLeaf(LEFT);
      }
    } else {
      parent->setLeaf(RIGHT, victim->getLeaf(LEFT));
      if (victim->getLeaf(RIGHT)->getLeaf(0) != NULL) {
        tmp = victim->getLeaf(RIGHT);
      }
    }
    
    if (tmp->getLeaf(LEFT) != NULL) {
      opening = findOpening(tmp->key, victim);
      
      parent = opening->parent;
      tmp->parent = parent;
      
      if (parent->getLeaf(LEFT) == opening) {
        parent->setLeaf(LEFT, tmp);
      } else {
        parent->setLeaf(RIGHT, tmp);
      }
      
      delete opening;
    } else {
      delete tmp;
    }
    
    return victim;
    
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  void Tree<TreeNodeType,DataType,KeyType>::remove(KeyType key) {
    delete remove_r(key);
    
  }
#endif
  
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
    
    if (key < current->key) {
      tmp = LEFT;
    } else {
      tmp = RIGHT;
    }
    
    return getNode(key, current->getLeaf(tmp));
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::getNodeMinGreaterThan(KeyType key, TreeNodeType *current) {
    TreeNodeType *tNode = NULL;
    
    if ((current->getLeaf(0) == NULL) || (current->key == key)) {
      return NULL;
    }
    
    if (key < current->key) {
      return getNodeMinGreaterThan(key, current->getLeaf(LEFT));
    }
    
    tNode = getNodeMinGreaterThan(key, current->getLeaf(RIGHT));
    
    if (tNode == NULL) {
      return current;
    }
    
    return tNode;
    
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  DataType Tree<TreeNodeType,DataType,KeyType>::search(KeyType key) {
    
    return getNode(key,treeRoot)->data;
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::rLeftSuccesor(TreeNodeType *tNode) {
    
    if (tNode->getLeaf(LEFT)->getLeaf(0) != NULL) {
      return rLeftSuccesor(tNode->getLeaf(LEFT));
    }
    
    return tNode;
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::nextNode(TreeNodeType *tNode) {
    
    if (tNode->getLeaf(RIGHT)->getLeaf(0) != NULL) {
      return rLeftSuccesor(tNode->getLeaf(RIGHT));
    }
    
    return getNodeMinGreaterThan(tNode->key, treeRoot);
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::firstNode(TreeNodeType *tNode) {
    
    return rLeftSuccesor(tNode);
  }
  
  
}

#endif
