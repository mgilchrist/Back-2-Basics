//
//  Tree.h
//  OpenSource
//
//  Created by Marcus Gilchrist on 9/16/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#ifndef OpenSource_Tree_h
#define OpenSource_Tree_h

#include "Graph.h"


namespace Graph {
  
  
  /* Tree */
  
  template <class EntryType, class KeyType>
  class Tree
  {
    
  private:
    Collection::Stack<EntryType> *entryList;
    Collection::Stack<KeyType> *lowValList;
    Collection::Stack<KeyType> *keys;
    uint64_t treeRoot = 0;
    
  protected:
    virtual uint64_t findOpening(KeyType key, uint64_t current);
    virtual uint64_t parentOf(uint64_t entry);
    virtual uint64_t sonOf(uint64_t entry);
    virtual uint64_t daughterOf(uint64_t entry);
    
  public:
    Tree();
    
    virtual uint64_t getParent(uint64_t entry);
    virtual uint64_t getLeft(uint64_t entry);
    virtual uint64_t getRight(uint64_t entry);
    virtual uint64_t getTreeRoot();
    virtual void swap(uint64_t a, uint64_t b);
    virtual void move(uint64_t from, uint64_t to);
    virtual EntryType nodeAtIndex(uint64_t);
    virtual uint64_t insertTreeNode(EntryType node, KeyType key);
    virtual void removeTreeNode(uint64_t index);
    virtual uint64_t getTreeNode(KeyType key, uint64_t current);
  };
  
  
  
  
  /* Tree */
  
  template <class EntryType, class KeyType>
  Tree<EntryType,KeyType>::Tree() {
    this->entryList = new Collection::Stack<EntryType>();
    this->lowValList = new Collection::Stack<KeyType>();
    this->keys = new Collection::Stack<KeyType>();
    
    entryList->setIndex(0, NULL);
  }
  
  template <class EntryType, class KeyType>
  uint64_t Tree<EntryType,KeyType>::findOpening(KeyType key, uint64_t current) {
    
    
    if (entryList->atIndex(current) == NULL) {
      return current;
    }
    
    if (keys->atIndex(current) >= key) {
      if (lowValList->atIndex(current) > key) {
        lowValList[current] = key;
      }
      return findOpening(key, sonOf(current));
      
    } else {
      return findOpening(key, daughterOf(current));
    }
    
  }
  
  
  template <class EntryType, class KeyType>
  uint64_t Tree<EntryType,KeyType>::insertTreeNode(EntryType node, KeyType key) {
    
    uint64_t current = findOpening(key, 0);
    
    entryList->setIndex(current, node);
    keys->setIndex(current, key);
    
    entryList->setIndex(sonOf(current), NULL);
    entryList->setIndex(daughterOf(current), NULL);
    
    return current;
    
  }
  
  template <class EntryType, class KeyType>
  void Tree<EntryType,KeyType>::removeTreeNode(uint64_t index) {
    
    entryList->setIndex(index, entryList->atIndex(getRight(index)));
    
    if (entryList->atIndex(index) != NULL) {
      removeTreeNode(getRight(index));
    }
    
  }
  
  template <class EntryType, class KeyType>
  uint64_t Tree<EntryType,KeyType>::getTreeRoot() {
    
    return treeRoot;
    
  }
  
  template <class EntryType, class KeyType>
  uint64_t Tree<EntryType,KeyType>::getTreeNode(KeyType key, uint64_t current) {
    
    if (keys->atIndex(current) == key) {
      return current;
    }
    
    if ((keys->atIndex(sonOf(current))) >= key) {
      return findOpening(key, sonOf(current));
    }
    
    if ((keys->atIndex(daughterOf(current))) >= key) {
      
      if (lowValList->atIndex(daughterOf(current)) >= key) {
        return findOpening(key, sonOf(current));
      } else {
        return findOpening(key, daughterOf(current));
      }
    }
    
    return findOpening(key, daughterOf(current));
  }
  
  template <class EntryType, class KeyType>
  EntryType Tree<EntryType,KeyType>::nodeAtIndex(uint64_t index) {
    
    return entryList->atIndex(index);
    
  }
  
  template <class EntryType, class KeyType>
  uint64_t Tree<EntryType,KeyType>::parentOf(uint64_t entry) {
    
    return (entry - 1) / 2;
    
  }
  
  template <class EntryType, class KeyType>
  uint64_t Tree<EntryType,KeyType>::sonOf(uint64_t entry) {
    
    return (entry * 2) + 1;
    
  }
  
  template <class EntryType, class KeyType>
  uint64_t Tree<EntryType,KeyType>::daughterOf(uint64_t entry) {
    
    return (entry * 2) + 2;
    
  }
  
  template <class EntryType, class KeyType>
  uint64_t Tree<EntryType,KeyType>::getParent(uint64_t entry) {
    
    return parentOf(entry);
    
  }
  
  template <class EntryType, class KeyType>
  uint64_t Tree<EntryType,KeyType>::getLeft(uint64_t entry) {
    
    return sonOf(entry);
    
  }
  
  template <class EntryType, class KeyType>
  uint64_t Tree<EntryType,KeyType>::getRight(uint64_t entry) {
    
    return daughterOf(entry);
    
  }
  
  
  template <class EntryType, class KeyType>
  void Tree<EntryType,KeyType>::swap(uint64_t a, uint64_t b) {
    EntryType tmpEntry;
    
    tmpEntry = this->entryList->atIndex(a);
    this->entryList->setIndex(a, this->entryList->atIndex(b));
    this->entryList->setIndex(b, tmpEntry);
    
  }
  
  template <class EntryType, class KeyType>
  void Tree<EntryType,KeyType>::move(uint64_t from, uint64_t to) {
    
    this->entryList->setIndex(sonOf(to),
                              this->entryList->atIndex(from));
    
    this->entryList->setIndex(sonOf(from), NULL);
  }
  
  
  
}

#endif
