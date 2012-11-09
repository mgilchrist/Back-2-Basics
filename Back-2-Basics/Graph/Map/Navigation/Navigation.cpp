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
    this->X = 0.0;
    this->Y = 0.0;
    this->Z = 0.0;
  }
  
  Coordinate::Coordinate(double X, double Y, double Z) {
    this->X = X;
    this->Y = Y;
    this->Z = Z;
  }
  
  void HeuristicMap::aStar() {
    Collection::Heap<Coordinate *,double> *open;
    Collection::HashTable<bool,Coordinate *> *openTable;
    Collection::HashTable<double,Coordinate *> *closed;
    Coordinate *u;
    vector<Path *> *ret;
    vector <Coordinate *> *dirtyNodes;
    bool tmpBool;
    
    open = new Collection::Heap<Coordinate *,double>();
    openTable = new Collection::HashTable<bool,Coordinate *>();
    closed = new Collection::HashTable<double,Coordinate *>();
    dirtyNodes = new vector<Coordinate *>();
    
    AStarStorage *aStarStorage = new AStarStorage;
    
    aStarStorage->open = open;
    aStarStorage->openTable = openTable;
    aStarStorage->closed = closed;
    
    this->start->distanceFromStart = 0.0;
    
    start->auxIndex = open->push(this->start,0.0);

    openTable->insert(true, this->start);
    
    while (open->getSize()) {
      u = open->pop();
      dirtyNodes->push_back(u);
      
      if (!openTable->get(u, &tmpBool) || (!tmpBool)) {
        break;
      }
      
      closed->insert(u->distanceFromStart, u);
      
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
  
  vector<Path *> *HeuristicMap::getShortestPath() {
    
    if (shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
      shortestPathToTerminal = NULL;
    }
    
    aStar();
    
    return shortestPathToTerminal;
    
  }
  
  Path::Path(Coordinate *v, Coordinate *u, double length) {
    this->u = u;
    this->v = v;
    this->length = length;
  }  
  
}