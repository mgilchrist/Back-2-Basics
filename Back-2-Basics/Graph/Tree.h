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

#ifndef Back_2_Basics_Tree_h
#define Back_2_Basics_Tree_h

#include "Graph.h"


namespace Graph {
  
  template <class TreeNodeType, class DataType, class KeyType>
  class TreeNode
  {
  public:
    TreeNodeType *left = NULL;
    TreeNodeType *right = NULL;
    
  public:
    DataType data = NULL;
    KeyType key;
    
    TreeNode() {
    }
  };
  
  /* Tree */
  
  template <class TreeNodeType, class DataType, class KeyType>
  class Tree
  {
    
  protected:
    TreeNodeType *treeRoot;
    uint64_t numNodes = 0;
    
    TreeNodeType *findOpening(KeyType, TreeNodeType *);
    TreeNodeType *getNodeMinGreaterThan(KeyType, TreeNodeType *);
    
    static void rModifyAll(KeyType (*action)(TreeNodeType *, void *), void *, TreeNodeType *, uint64_t);
    
  public:
    Tree();
    
    TreeNodeType nullNode;
    
    TreeNodeType *getNode(KeyType key, TreeNodeType *current);
    TreeNodeType *getTreeRoot();
    DataType search(TreeNodeType *root, KeyType key);
    TreeNodeType *next(TreeNodeType *tNode);
    TreeNodeType *min(TreeNodeType *);
    
    void modifyAll(KeyType (*action)(TreeNodeType *, void *), void *);
    uint64_t size() {return numNodes;}
    
  };
  
  
  /* Tree */
  
  template <class TreeNodeType, class DataType, class KeyType>
  Tree<TreeNodeType,DataType,KeyType>::Tree() {
    treeRoot = &nullNode;
    nullNode.left = &nullNode;
    nullNode.right = &nullNode;
    nullNode.data = NULL;
    nullNode.key = (uint64_t)-1;
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::getTreeRoot() {
    
    return this->treeRoot;
    
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::getNode(KeyType key, TreeNodeType *current) {
    
    if (current->key == key) {
      return current;
    }
    
    if (key < current->key) {
      return getNode(key, current->left);
    } else {
      return getNode(key, current->right);
    }
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::getNodeMinGreaterThan(KeyType key, TreeNodeType *current) {
    TreeNodeType *tNode;
    
    if (current == &nullNode) {
      return current;
    }
    
    if (key == current->key) {
      return NULL;
    }
    
    if (key < current->key) {
      tNode = getNodeMinGreaterThan(key, current->left);
    } else {
      tNode = getNodeMinGreaterThan(key, current->right);
    }
    
    if (tNode == &nullNode) {
      return NULL;
    }
    
    if (tNode == NULL) {
      return current;
    }

    
    return tNode;
    
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  DataType Tree<TreeNodeType,DataType,KeyType>::search(TreeNodeType *treeRoot, KeyType key) {
    
    return getNode(key,treeRoot)->data;
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::next(TreeNodeType *tNode) {
    
    TreeNodeType *tmp;
    TreeNodeType *lastGT = NULL;
    
    if (tNode->right != &nullNode) {
      return min(tNode->right);
    }
    
    if (treeRoot == tNode) {
      return NULL;
    }
    
    tmp = treeRoot;
    
    while (true) {
      if (tmp->left->key == tNode->key) {
        return tmp;
      }
      
      if (tmp->right->key == tNode->key) {
        break;
      }
      
      if (tmp->key > tNode->key) {
        lastGT = tmp;
        tmp = tmp->left;
      } else {
        tmp = tmp->right;
      }
      
    }
    
    return lastGT;
  }
  
  template <class TreeNodeType, class DataType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,DataType,KeyType>::min(TreeNodeType *tNode) {
    
    while (tNode->left != &nullNode) {
      tNode = tNode->left;
    }
    
    return tNode;
  }
  
#if 0
  template <class TreeNodeType, class DataType, class KeyType>
  void Tree<TreeNodeType,DataType,KeyType>::rModifyAll(KeyType (*action)(TreeNodeType *, void *),
                                                       void *object,
                                                       TreeNodeType *current,
                                                       uint64_t instance) {
    
    if (current->data == NULL) {
      return;
    }
    
    KeyType newKey;
    
    newKey = action(current, object);
    
    // TODO
    //updateKey(current->key, newKey, instance);
    
    if (current->left != NULL) {
      rModifyAll(action, object, current->left, ((current->key == current->left->key) ? ++instance : 0));
    }
    
    if (current->right != NULL) {
      rModifyAll(action, object, current->right, ((current->key == current->right->key) ? ++instance : 0));
    }
  }
#endif
  
  template <class TreeNodeType, class DataType, class KeyType>
  void Tree<TreeNodeType,DataType,KeyType>::rModifyAll(KeyType (*action)(TreeNodeType *, void *),
                                                       void *object,
                                                       TreeNodeType *current,
                                                       uint64_t instance) {
    vector<TreeNodeType *> *L;
    vector<TreeNodeType *> *nextL;
    TreeNodeType *u;
    KeyType newKey;
    
    L = new vector<TreeNodeType *>();
    
    if (current->data != NULL) {
      L->push_back(current);
    }
    
    
    while (!L->empty()) {
      
      // Initialize new layer
      nextL = new vector<TreeNodeType *>();
      
      while (!(L->empty())) {
        u = L->back();
        L->resize(L->size()-1);
        
        if (u->data == NULL) {
          continue;
        }
        
        newKey = (*action)(u, object);
        
        if (u->left->data != NULL) {
          nextL->push_back(u->left);
        }
        
        if (u->right->data != NULL) {
          nextL->push_back(u->right);
        }
      }
      
      L = nextL;
      
    }
    
    delete L;
    
  }

  
  template <class TreeNodeType, class DataType, class KeyType>
  void Tree<TreeNodeType,DataType,KeyType>::modifyAll(KeyType (*action)(TreeNodeType *, void *), void *object) {
    
    if (this->treeRoot != &this->nullNode) {
      rModifyAll(action,object,this->treeRoot,0);
    }
  
  }
  
  
}

#endif
