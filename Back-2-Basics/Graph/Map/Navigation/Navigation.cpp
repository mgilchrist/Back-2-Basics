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
    Collection::HashTable<uint8_t,Coordinate *> *openTable;
    Collection::HashTable<double,Coordinate *> *closed;
    Coordinate *u;
    Collection::Stack<Path *> *ret;
    
    open = new Collection::Heap<Coordinate *,double>();
    openTable = new Collection::HashTable<uint8_t,Coordinate *>();
    closed = new Collection::HashTable<double,Coordinate *>();
    
    // set initial distances to infinity
    for (int ix = 0; ix < this->getNumNodes(); ix++) {
      this->nodeAtIndex(ix)->previousEdge = NULL;
    }
    
    this->start->distanceFromStart = 0.0;
    
    this->nodeAtIndex(0)->auxIndex = open->push(this->start,0.0);

    openTable->insert(true, this->start);
    
    while (open->getSize()) {
      u = open->pop();
      
      if (openTable->get(u) <= 0) {
        break;
      }
      
      closed->insert(u->distanceFromStart, u);
      
      for (int ix = 0; ix < u->getNumEdges(); ix++) {
        if (u->getAdjacentEdge(ix)->blocked) {
          continue;
        }
        
        Coordinate *v = u->getAdjacentEdge(ix)->getForward();
        double cost = u->distanceFromStart + u->getAdjacentEdge(ix)->length;
        
        if (cost < v->distanceFromStart) {
          if (openTable->get(v) > 0) {
            openTable->update(-1, v);
          }
          
          closed->remove(v);
        }
        
        if ((openTable->get(v) <= 0) && (closed->get(v) != 0.0)) {
          
          v->distanceFromStart = cost;
          
          if (openTable->update(true, v) > 0) {
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