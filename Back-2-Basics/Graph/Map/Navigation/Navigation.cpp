//
//  Navigation.cpp
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 9/26/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#include "Navigation.h"
#include "HashTable.h"


#define ERROR -1

namespace Graph {
  
  
  void HeuristicMap::aStar() {
    Collection::Heap<Coordinate *,double> *open;
    Collection::HashTable<bool, Coordinate *> *openTable;
    Collection::HashTable<double, Coordinate *> *closed;
    Coordinate *u;
    Collection::Stack<Path *> *ret;
    uint64_t opIndex;
    
    open = new Collection::Heap<Coordinate *,double>();
    openTable = new Collection::HashTable<bool, Coordinate *>();
    closed = new Collection::HashTable<double, Coordinate *>();
    
    // set initial distances to infinity
    for (int ix = 0; ix < this->getNumNodes(); ix++) {
      this->nodeAtIndex(ix)->previousEdge = NULL;
    }
    
    this->start->distanceFromStart = 0.0;
    
    this->nodeAtIndex(0)->auxIndex = open->push(this->start,0.0);
    openTable->insert(true, this->start);
    
    while (open->getSize()) {
      u = open->pop();
      opIndex = openTable->search(u);
      
      if ((opIndex == ERROR) || (openTable->get(opIndex) == false)) {
        break;
      }
      
      closed->insert(u->distanceFromStart, u);
      
      for (int ix = 0; ix < u->getNumEdges(); ix++) {
        if (u->getAdjacentEdge(ix)->blocked) {
          continue;
        }
        
        Coordinate *v = u->getAdjacentEdge(ix)->getForward();
        double cost = u->distanceFromStart + u->getAdjacentEdge(ix)->length;
        opIndex = openTable->search(v);
        uint64_t cIndex = closed->search(v);
        
        if (cost < v->distanceFromStart) {
          if ((opIndex != ERROR) && (openTable->get(opIndex) == true)) {
            openTable->update(opIndex, false);
          }
          
          if (cIndex != ERROR) {
            closed->remove(cIndex);
          }
        }
        
        if (((opIndex == ERROR) || (openTable->get(opIndex) == false)) &&
            (cIndex == ERROR)) {
          
          v->distanceFromStart = cost;
          
          if (opIndex == ERROR) {
            openTable->insert(true, v);
          } else {
            openTable->update(opIndex, true);
            open->removeHeapEntry(*(v->auxIndex));
          }
          this->nodeAtIndex(v->getIndex())->auxIndex =
            open->push(v, cost + costHeuristic->atIndex(v->getIndex())->getExpectation());
          v->previousEdge = u->getAdjacentEdge(ix);
          
        }
      }
    }
    
    if (shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
      shortestPathToTerminal = NULL;
    }
    
    ret = new Collection::Stack<Path *>();
    
    u = this->terminal;
    
    while (u->previousEdge != NULL) {
      if (u->previousEdge->getBackward() != NULL) {
        ret->push(u->previousEdge);
      }
    }
    
    shortestPathToTerminal = ret;
    
  }
  
  Collection::Stack<Path *> *HeuristicMap::getShortestPath() {
    
    if (shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
      shortestPathToTerminal = NULL;
    }
    
    aStar();
    
    return shortestPathToTerminal->clone();
    
  }
  
  Path::Path(Coordinate *v, Coordinate *u, double length) {
    this->u = u;
    this->v = v;
    this->length = length;
  }  
  
}