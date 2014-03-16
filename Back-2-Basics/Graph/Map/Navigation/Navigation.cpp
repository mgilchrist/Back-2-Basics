//
//  Navigation.cpp
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 9/26/12.
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


#include "Navigation.h"
#include "HashTable.h"


#define ERROR -1

namespace Graph {

  Coordinate::Coordinate() {
    this->X = 0;
    this->Y = 0;
    this->Z = 0;
  }
  
  Coordinate::Coordinate(int64_t X, int64_t Y, int64_t Z) {
    this->X = X;
    this->Y = Y;
    this->Z = Z;
  }
  
  void HeuristicMap::aStar() {
    Heap<Coordinate *,int64_t> *open;
    HashTable<bool *,Coordinate *> *openTable;
    HashTable<uint64_t *,Coordinate *> *closed;
    Coordinate *u;
    vector<Path *> *ret;
    vector <Coordinate *> *dirtyNodes;
    bool tmpTrue = true;
    
    open = new Heap<Coordinate *,int64_t>();
    openTable = new HashTable<bool *,Coordinate *>();
    closed = new HashTable<uint64_t *,Coordinate *>();
    dirtyNodes = new vector<Coordinate *>();
    
    AStarStorage *aStarStorage = new AStarStorage;
    
    aStarStorage->open = open;
    aStarStorage->openTable = openTable;
    aStarStorage->closed = closed;
    
    this->start->distanceFromStart = 0;
    
    start->auxIndex = open->push(this->start, 0);

    openTable->insert(&tmpTrue, this->start);
    
    while (open->size()) {
      u = open->pop();
      u->auxIndex = NULL;
      dirtyNodes->push_back(u);
      
      if (!openTable->get(u)) {
        break;
      }
      
      closed->insert(&u->distanceFromStart, u);
      
      u->modifyAllAdjacent(aStarGambit, (void *)aStarStorage);
    }
    
    if (shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
      shortestPathToTerminal = NULL;
    }
    
    ret = new vector<Path *>();
    
    u = this->terminal;
    
    while (u->previousEdge != NULL) {
      if (u->previousEdge->getBackward() != NULL) {
        ret->push_back(u->previousEdge);
      }
      
       u = u->previousEdge->getBackward();
    }
    
    shortestPathToTerminal = ret;
    
    // set initial distances to infinity
    for (int ix = 0; ix < dirtyNodes->size(); ix++) {
      dirtyNodes->at(ix)->distanceFromStart = 1.0/0.0;
      dirtyNodes->at(ix)->previousEdge = NULL;
    }
    
  }
  
  /*
  vector<Path *> *HeuristicMap::getShortestPath() {
    
    if (shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
      shortestPathToTerminal = NULL;
    }
    
    aStar();
    
    return shortestPathToTerminal;
    
  }
   */
  
  Path::Path(Coordinate *v, Coordinate *u, int64_t length) {
    this->u = u;
    this->v = v;
    this->attrib = length;
    
    v->references++;
    u->addEdge((Path *)this);
    
  }  
  
}