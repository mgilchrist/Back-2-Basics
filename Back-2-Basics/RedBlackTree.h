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


namespace Graph {
  
  template <class EntryType, class KeyType>
  class RedBlackTree : public Tree<EntryType,KeyType>
  {
    
  private:
    enum Color {
      RED,
      BLACK
    };
    
    Collection::Stack<Color> *colorList;
    
    virtual bool isSon(uint64_t entry);
    virtual bool isDaughter(uint64_t entry);
    virtual void move(uint64_t from, uint64_t to);
    
    virtual void recolor(uint64_t nodePosition);
    virtual void rotateLeft(uint64_t nodePosition);
    virtual void rotateRight(uint64_t nodePosition);
    
  public:
    RedBlackTree();
    
    virtual void insertRedBlackNode(EntryType n, KeyType key);
    void printRBTree(uint64_t start);
    
    
  };
  
  
  template <class EntryType, class KeyType>
  RedBlackTree<EntryType,KeyType>::RedBlackTree() {
    this->colorList = new Collection::Stack<Color>();
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::move(uint64_t from, uint64_t to) {
    
    Tree<EntryType,KeyType>::move(from, to);
    
    this->colorList->setIndex(this->sonOf(to),
                              this->colorList->atIndex(from));
    
    this->colorList->setIndex(this->sonOf(from), BLACK);
  }
  
  template <class EntryType, class KeyType>
  bool RedBlackTree<EntryType,KeyType>::isSon(uint64_t entry) {
    
    if (entry == 0) {
      return false;
    }
    
    return (entry % 2) ? true : false;
  }
  
  template <class EntryType, class KeyType>
  bool RedBlackTree<EntryType,KeyType>::isDaughter(uint64_t entry) {
    
    if (entry == 0) {
      return false;
    }
    
    return (entry % 2) ? false : true;
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::printRBTree(uint64_t start) {
    
    uint64_t last = 0, ix = 0;
    
    while(1) {
      for (uint64_t jx = last; jx < ((2 ^ ix) + last); jx++) {
        if (this->nodeAtIndex(jx) != NULL) {
          cout << ((this->colorList->atIndex(start) == BLACK) ? "B " : "R ");
        } else {
          cout << "  ";
        }
      }
      cout << "\n";
      last = (2 ^ ix) + last;
      ix++;
    }
    
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::insertRedBlackNode(EntryType n, KeyType key) {
    
    uint64_t nodePosition;
    
    cout << "inserting tree node\n";
    
    nodePosition = this->insertTreeNode(n,key);
    
    cout << "maintaining red/black rules.\n";
    
    this->colorList->push(RED);
    
    while ((nodePosition != 0) && (this->colorList->atIndex(this->parentOf(nodePosition)) == RED)) {
      cout << "parent of ";
      cout << nodePosition;
      cout << " is red\n";
      
      if ((this->parentOf(nodePosition) != 0) &&
          (isSon(this->parentOf(nodePosition)))) {
        uint64_t aunt = this->daughterOf(this->parentOf(this->parentOf(nodePosition)));
        
        if (this->colorList->atIndex(aunt) == RED) {
          cout << "aunt is also red, just recoloring\n";
          recolor(nodePosition);
        } else {
          if (isDaughter(nodePosition)) {
            cout << "this is daughter, rotating left\n";
            rotateLeft(nodePosition);
          }
          cout << "rotating right\n";
          rotateRight(nodePosition);
        }
      } else {
        uint64_t uncle = this->sonOf(this->parentOf(this->parentOf(nodePosition)));
        if (this->colorList->atIndex(uncle) == RED) {
          cout << "uncle is also red, just recoloring\n";
          recolor(nodePosition);
        } else {
          if (isSon(nodePosition)) {
            cout << "this is son, rotating right\n";
            rotateRight(nodePosition);
          }
          cout << "rotating left\n";
          rotateLeft(nodePosition);
        }
      }
      
      cout << "Moving up tree\n";
      nodePosition = this->parentOf(nodePosition);
    }
    
    
    this->colorList->setIndex(0, BLACK);
    
    printRBTree(0);
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::recolor(uint64_t nodePosition) {
    
    
    this->colorList->setIndex(this->parentOf(this->parentOf(nodePosition)), RED);
    
    this->colorList->setIndex(this->sonOf(this->parentOf(this->parentOf(nodePosition))), BLACK);
    this->colorList->setIndex(this->daughterOf(this->parentOf(this->parentOf(nodePosition))), BLACK);
    
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::rotateLeft(uint64_t nodePosition) {
    
    uint64_t parent = this->parentOf(nodePosition);
    uint64_t grandParent = this->parentOf(parent);
    
    
    
    if (parent == this->sonOf(grandParent)) {
      if (!isSon(nodePosition)) {
        uint64_t son = this->sonOf(nodePosition);
        
        /* Staightening */
        move(parent, this->sonOf(nodePosition));
        move(grandParent, parent);
        move(nodePosition, this->sonOf(grandParent));
        move(son, this->daughterOf(parent));
      }
    } else {
      if (!isSon(nodePosition)) {
        uint64_t brother = this->sonOf(parent);
        
        /* Flattening */
        move(grandParent, this->sonOf(parent));
        move(this->parentOf(grandParent), grandParent);
        move(parent, this->parentOf(grandParent));
        move(brother, this->daughterOf(grandParent));
        
      }
    }
  }
  
  template <class EntryType, class KeyType>
  void RedBlackTree<EntryType,KeyType>::rotateRight(uint64_t nodePosition) {
    
    uint64_t parent = this->parentOf(nodePosition);
    uint64_t grandParent = this->parentOf(parent);
    
    if (isSon(parent)) {
      if (isSon(nodePosition)) {
        uint64_t sister = this->daughterOf(parent);
        
        /* Flattening */
        move(grandParent, this->daughterOf(parent));
        move(this->parentOf(grandParent), grandParent);
        move(parent, this->parentOf(grandParent));
        move(sister, this->sonOf(grandParent));
        
      }
    } else {
      if (isSon(nodePosition)) {
        uint64_t daughter = this->daughterOf(nodePosition);
        
        /* Staightening */
        move(parent, this->daughterOf(nodePosition));
        move(grandParent, parent);
        move(nodePosition, this->daughterOf(grandParent));
        move(daughter, this->sonOf(parent));
      }
    }
    
  }
  
}


#endif
