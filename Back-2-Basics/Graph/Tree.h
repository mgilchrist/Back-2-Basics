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

//#include "Graph.h"
#include <vector>
using namespace std;

namespace Tree {
  
#define VERBOSITY 1
  
#define DEBUG_PRIINT(a, b)  do { if (a < VERBOSITY) cout << b;} while((0))
  
  

  template <class TreeNodeType, class EntryType, class KeyType>
  class Tree;
  
  template <class TreeNodeType, class EntryType, class KeyType>
  class TreeNode;
  
  template <class TreeNodeType, class EntryType, class KeyType>
  class TreeNode
  {
  protected:
    TreeNodeType *leftNode = NULL;
    TreeNodeType *rightNode = NULL;
    
  public:
    EntryType data;
    KeyType key;
    
    TreeNode() {
      
    }
    
    friend Tree<TreeNodeType,EntryType,KeyType>;
  };
  
  /* Tree */
  
  template <class TreeNodeType, class EntryType, class KeyType>
  class Tree
  {
    
  protected:
    
    uint64_t numNodes = 0;
    
    TreeNodeType *nullNode;
    
    TreeNodeType *findOpening(KeyType, TreeNodeType *);
    TreeNodeType *getNodeMinGreaterThan(KeyType, TreeNodeType *);
    
    static void rSelect(bool (*action)(TreeNodeType *, void *), void *, TreeNodeType *, vector<TreeNodeType *> *updates);
    static void rModifyAll(KeyType (*action)(TreeNodeType *, void *), void *, TreeNodeType *, vector<TreeNodeType *> *victims);
    
  public:
    Tree() {
      nullNode = new TreeNodeType();
      treeRoot = nullNode;
      
      nullNode->leftNode = nullNode;
      nullNode->rightNode = nullNode;
    }
    
    ~Tree();
    
    static inline void setLeft(TreeNodeType *node, TreeNodeType *child) {
      node->leftNode = child;
    }
    
    static inline void setRight(TreeNodeType *node, TreeNodeType *child) {
      node->rightNode = child;
    }
    
    static inline TreeNodeType *leftOf(TreeNodeType *node) {
      return node->leftNode;
    }
    
    static inline TreeNodeType *rightOf(TreeNodeType *node) {
      return node->rightNode;
    }
    
    TreeNodeType *treeRoot;
    
    EntryType search(KeyType key);
    static TreeNodeType *search(TreeNodeType *node, KeyType key);
    TreeNodeType *next(TreeNodeType *tNode);
    TreeNodeType *min(TreeNodeType *);
    
    uint64_t removal(bool (*action)(TreeNodeType *, void *), void *);
    uint64_t deletion(bool (*action)(TreeNodeType *, void *), void *);
    vector<EntryType> *select(bool (*criteria)(TreeNodeType *, void *), void *);
    void modifyAll(KeyType (*action)(TreeNodeType *, void *), void *);
    uint64_t size() {return numNodes;}
    
    virtual void insert(EntryType, KeyType) =0;
    virtual void removeMax() =0;
    virtual void removeMin() =0;
    virtual void remove(EntryType, KeyType) {
      assert(false);
    }
    
  };
  
  
  /* Tree */
  
  template <class TreeNodeType, class EntryType, class KeyType>
  Tree<TreeNodeType,EntryType,KeyType>::~Tree() {
    removal(NULL, NULL);
  }
  
  
  template <class TreeNodeType, class EntryType, class KeyType>
  EntryType Tree<TreeNodeType,EntryType,KeyType>::search(KeyType key) {
    TreeNodeType *ret;
    
    ret = search(treeRoot, key);
    
    if (ret == NULL) {
      return NULL;
    }
    
    return ret->data;
  }
  
  template <class TreeNodeType, class EntryType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,EntryType,KeyType>::search(TreeNodeType *node, KeyType key) {
    
    while (node != leftOf(node)) {
      if (key == node->key) {
        return node;
      } else if (key < node->key) {
        node = leftOf(node);
      } else {
        node = rightOf(node);
      }
    }
    return NULL;
  }
  
  template <class TreeNodeType, class EntryType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,EntryType,KeyType>::next(TreeNodeType *tNode) {
    
    TreeNodeType *tmp;
    TreeNodeType *lastGT = nullNode;
    
    if (rightOf(tNode) != nullNode) {
      return min(rightOf(tNode));
    }
    
    if (treeRoot == tNode) {
      return NULL;
    }
    
    tmp = treeRoot;
    
    while (tmp != nullNode) {
      if ((leftOf(tmp)->key == tNode->key) && (leftOf(tmp) != nullNode)) {
        return tmp;
      }
      
      if ((rightOf(tmp)->key == tNode->key) && (rightOf(tmp) != nullNode)) {
        break;
      }
      
      if (tmp->key > tNode->key) {
        lastGT = tmp;
        tmp = leftOf(tmp);
      } else {
        tmp = rightOf(tmp);
      }
      
    }
    
    if (lastGT == nullNode) {
      return NULL;
    }
    
    return lastGT;
  }
  
  template <class TreeNodeType, class EntryType, class KeyType>
  TreeNodeType *Tree<TreeNodeType,EntryType,KeyType>::min(TreeNodeType *tNode) {
    
    while (leftOf(tNode) != nullNode) {
      tNode = leftOf(tNode);
    }
    
    return tNode;
  }
  
  template <class TreeNodeType, class EntryType, class KeyType>
  void Tree<TreeNodeType,EntryType,KeyType>::rSelect(bool (*criteria)(TreeNodeType *, void *),
                                                    void *object,
                                                    TreeNodeType *current,
                                                    vector<TreeNodeType *> *victims) {
    vector<TreeNodeType *> L;
    TreeNodeType *u;
    
    u = current;
    
    do {
      
      /* Lean left */
      while (u != leftOf(u)) {
        L.push_back(u);
        u = leftOf(u);
      }
      
      /* Visit */
      while (!L.empty()) {
        u = L.back();
        L.resize(L.size()-1);
        
        if ((criteria == NULL) || ((*criteria)(u, object))) {
          victims->push_back(u);
        }
       
        /* Peak right */
        u = rightOf(u);
        if (u != rightOf(u)) {
          break;
        }
      }
      
    } while (u != rightOf(u));
    
  }
  
  template <class TreeNodeType, class EntryType, class KeyType>
  void Tree<TreeNodeType,EntryType,KeyType>::rModifyAll(KeyType (*action)(TreeNodeType *, void *),
                                                       void *object,
                                                       TreeNodeType *current,
                                                       vector<TreeNodeType *> *updates) {
    vector<TreeNodeType *> L;
    TreeNodeType *u;
    KeyType newKey;
    
    u = current;
    
    do {
      
      /* Lean left */
      while (u != leftOf(u)) {
        L.push_back(u);
        u = leftOf(u);
      }
      
      /* Visit */
      while (!L.empty()) {
        u = L.back();
        L.resize(L.size()-1);
        
        /* Visit */
        newKey = (*action)(u, object);
        
        if (u->key != newKey) {
          updates->push_back(u);
        }
        
        /* Peak right */
        u = rightOf(u);
        if (u != rightOf(u)) {
          break;
        }
      }
    } while (u != rightOf(u));
    
  }
  
  
  template <class TreeNodeType, class EntryType, class KeyType>
  uint64_t Tree<TreeNodeType,EntryType,KeyType>::removal(bool (*criteria)(TreeNodeType *, void *), void *object) {
    
    uint64_t ret;
    vector<TreeNodeType *> *victims = new vector<TreeNodeType *>();
    
    rSelect(criteria,object,this->treeRoot,victims);
    
    ret = victims->size();
    
    while (!victims->empty()) {
      TreeNodeType *tmp = victims->back();
      remove(tmp->data, tmp->key);
      victims->resize(victims->size()-1);
    }
    
    delete victims;
    
    return ret;
  }
  
  template <class TreeNodeType, class EntryType, class KeyType>
  uint64_t Tree<TreeNodeType,EntryType,KeyType>::deletion(bool (*criteria)(TreeNodeType *, void *), void *object) {
    
    uint64_t ret;
    vector<TreeNodeType *> *victims = new vector<TreeNodeType *>();
    
    rSelect(criteria,object,this->treeRoot,victims);
    
    ret = victims->size();
    
    while (!victims->empty()) {
      TreeNodeType *tmp = victims->back();
      EntryType tmpData = tmp->data;
      this->remove(tmpData, tmp->key);
      delete tmpData;
      victims->resize(victims->size()-1);
    }
    
    delete victims;
    
    return ret;
    
  }
  
  template <class TreeNodeType, class EntryType, class KeyType>
  vector<EntryType> *Tree<TreeNodeType,EntryType,KeyType>::select(bool (*criteria)(TreeNodeType *, void *), void *object) {
    
    vector<TreeNodeType *> *victims = new vector<TreeNodeType *>();
    vector<EntryType> *ret = new vector<EntryType>();
    
    rSelect(criteria,object,this->treeRoot,victims);
    
    for (uint64_t ix = 0; ix < victims->size(); ix++) {
      ret->push_back(victims->at(ix)->data);
    }
    
    victims->resize(0);
    delete victims;
    
    return ret;
    
  }
  
  template <class TreeNodeType, class EntryType, class KeyType>
  void Tree<TreeNodeType,EntryType,KeyType>::modifyAll(KeyType (*action)(TreeNodeType *, void *), void *object) {
    
    vector<TreeNodeType *> *updates = new vector<TreeNodeType *>();
    
    rModifyAll(action,object,this->treeRoot,updates);
    
    for (uint64_t ix = 0; ix < updates->size(); ix++) {
      EntryType data = updates->at(ix)->data;
      KeyType key = updates->at(ix)->key;
      remove(data, key);
      insert(data, key);
    }
    
  }
  
  
}

#endif
