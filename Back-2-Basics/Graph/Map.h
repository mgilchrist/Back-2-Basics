//
//  Map.h
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

#ifndef OpenSource_Map_h
#define OpenSource_Map_h

#include "Heap.h"
#include "Graph.h"

namespace Graph {
  
  
  template <class NodeType, class EdgeType>
  class Location;
  
  template <class LocationType>
  class Via : public Edge<LocationType>
  {
  public:
    double      length;
    Via<LocationType>();
    Via<LocationType>(Location<LocationType,Via> *v, Location<LocationType,Via> *u, double length);
  };
  
  
  template <class NodeType, class EdgeType>
  class Location : public Node<NodeType,EdgeType>
  {
    
  public:
    
    bool        explored;
    double      distanceFromStart;
    uint64_t    *auxIndex;
    
  public:
    
    Location();
    
  };
  
  
  /* Via */
  
  template <class LocationType>
  Via<LocationType>::Via() {
    this->u = NULL;
    this->v = NULL;
    length = 0;
  }
  
  template <class LocationType>
  Via<LocationType>::Via(Location<LocationType,Via> *v, Location<LocationType,Via> *u, double length) {
    this->u = u;
    this->v = v;
    length = length;
  }
  
  
  /* Location */
  
  template <class NodeType, class EdgeType>
  Location<NodeType,EdgeType>::Location() {
    this->forwardEdges = new Collection::Stack<EdgeType *>();
    this->discovered = false;
    explored = false;
    distanceFromStart = 0.0;
  }
  
  
  template <class NodeType, class EdgeType>
  class Map : public Graph<NodeType, EdgeType>
  {
  protected:
    
    Collection::Stack<EdgeType *> *shortestPathToTerminal;
    uint64_t numNegativeEdges;
    Collection::Stack<EdgeType *> *shortPathTree;
    
  private:
    
    void destroyPath();
    
  protected:
    
    void dijkstras();
    void bellmanFord();
    
  public:
    
    Map();
    ~Map();
    
    EdgeType *addEdge(NodeType *a, NodeType *b, double location);
    void setStart(NodeType *start);
    void setTerminal(NodeType *terminal);
    virtual Collection::Stack<EdgeType *> *getShortestPath();
    
  };
  
  
  /* Map */
  
  template <class NodeType, class EdgeType>
  void Map<NodeType,EdgeType>::destroyPath() {
    if (this->shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
      shortestPathToTerminal = NULL;
    }
  }
  
  template <class NodeType, class EdgeType>
  Map<NodeType,EdgeType>::Map() {
    Graph<NodeType,EdgeType>();
    shortPathTree = NULL;
    shortestPathToTerminal = NULL;
    numNegativeEdges = 0;
  }
  
  template <class NodeType, class EdgeType>
  Map<NodeType,EdgeType>::~Map() {
    
    if (shortPathTree != NULL) {
      delete shortPathTree;
    }
    
    if (shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
    }
    
  }
  
  /* Bellman-Ford */
  
  template <class NodeType, class EdgeType>
  void Map<NodeType,EdgeType>::bellmanFord() {
    Collection::Stack<EdgeType *> *ret = new Collection::Stack<EdgeType *>();
    NodeType *u, *v;
    EdgeType *uv;
    
    
    for (int ix = 0; ix < this->getNumNodes(); ix++) {
      this->nodeAtIndex(ix)->distanceFromStart = 1.0/0.0;
      this->nodeAtIndex(ix)->previousEdge = NULL;
    }
    
    this->swapNodeIndexes(0, this->start->index);
    
    this->start->distanceFromStart = 0.0;
    
    for (int ix = 1; ix < this->getNumNodes(); ix++) {
      for (int jx = 0; jx < this->getNumEdges(); jx++) {
        uv = this->edgeAtIndex(jx);
        
        if (uv->blocked) {
          continue;
        }
        
        u = uv->getBackward();
        v = uv->getForward();
        if ((u->distanceFromStart + uv->length) < v->distanceFromStart) {
          v->distanceFromStart = u->distanceFromStart + uv->length;
          v->previousEdge = uv;
        }
      }
    }
    
    for (int ix = 0; ix < this->getNumEdges(); ix++) {
      uv = this->edgeAtIndex(ix);
      
      if (uv->blocked) {
        continue;
      }
      
      u = uv->getBackward();
      v = uv->getForward();
      if ((u->distanceFromStart + uv->length) < v->distanceFromStart) {
        if (shortestPathToTerminal != NULL) {
          delete shortestPathToTerminal;
          shortestPathToTerminal = NULL;
        }
        return;
      }
    }
    
    if (shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
      shortestPathToTerminal = NULL;
    }
    
    u = this->terminal;
    
    while (u->previousEdge != NULL) {
      if (u->previousEdge->getBackward() != NULL) {
        ret->push(u->previousEdge);
      }
      
      u = u->previousEdge->getBackward();
    }
    
    shortestPathToTerminal = ret;
  }
  
  
  /* Dijkstra's (For non-negative graphs) */
  template <class NodeType, class EdgeType>
  void Map<NodeType,EdgeType>::dijkstras() {
    Collection::Heap<NodeType *,double> *queue = new Collection::Heap<NodeType *,double>();
    NodeType *u, *v;
    double tmp;
    Collection::Stack<EdgeType *> *ret;
    
    // set initial distances to infinity
    for (uint64_t ix = 0; ix < this->getNumNodes(); ix++) {
      this->nodeAtIndex(ix)->distanceFromStart = 1.0/0.0;
      this->nodeAtIndex(ix)->previousEdge = NULL;
    }
    
    this->start->distanceFromStart = 0.0;
    
    for (uint64_t ix = 0; ix < this->getNumNodes(); ix++) {
      this->nodeAtIndex(ix)->auxIndex = queue->push(this->nodeAtIndex(ix),this->nodeAtIndex(ix)->distanceFromStart);
    }
    
    while (queue->getSize()) {
      u = queue->peekAtHeapEntry(0);
      if (u->distanceFromStart == 1.0/0.0) {
        break;
      }
      
      u = queue->pop();
      
      for (uint64_t ix = 0; ix < u->getNumEdges(); ix++) {
        if (u->getAdjacentEdge(ix)->blocked) {
          continue;
        }
        tmp = u->distanceFromStart + u->getAdjacentEdge(ix)->length;
        v = u->getAdjacentNode(ix);
        
        if (tmp < v->distanceFromStart) {
          v->distanceFromStart = tmp;
          v->previousEdge = u->getAdjacentEdge(ix);
          queue->update(*(v->auxIndex), tmp);
        }
      }
    }
    
    // Contruct Tree
    
    if (shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
      shortestPathToTerminal = NULL;
    }
    
    ret = new Collection::Stack<EdgeType *>();
    
    u = this->terminal;
    
    while ((u->previousEdge != NULL) && (u != this->start)) {
      if (u->previousEdge->getBackward() != NULL) {
        ret->push(u->previousEdge);
      }
      
      u = u->previousEdge->getBackward();
    }
    
    shortestPathToTerminal = ret;
    
  }
  
  template<class NodeType, class EdgeType>
  Collection::Stack<EdgeType *> *Map<NodeType,EdgeType>::getShortestPath() {
    
    if (shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
      shortestPathToTerminal = NULL;
    }
    
    if (this->numNegativeEdges) {
      bellmanFord();
    } else {
      dijkstras();
    }
    
    return shortestPathToTerminal->clone();
    
  }
  
  template <class NodeType, class EdgeType>
  EdgeType *Map<NodeType,EdgeType>::addEdge(NodeType *v, NodeType *u,
                                            double location) {
    EdgeType *tmpEdge = new EdgeType(v,u,location);
    
    if (location < 0.0) {
      numNegativeEdges++;
    }
    
    u->addAdjacentEdge(tmpEdge);
    
    this->addEdgeObj(tmpEdge);
    
    return tmpEdge;
  }
  
  template <class NodeType, class EdgeType>
  void Map<NodeType,EdgeType>::setStart(NodeType *start) {
    if (this->start != start) {
      this->start=start;
      
      if (shortestPathToTerminal == NULL) {
        return;
      }
      
      do {
        if (shortestPathToTerminal->peek() == NULL) {
          destroyPath();
        }
      } while (start != shortestPathToTerminal->pop()->getForward());
    }
  }
  
  template <class NodeType, class EdgeType>
  void Map<NodeType,EdgeType>::setTerminal(NodeType *terminal) {
    if (this->terminal != terminal) {
      this->terminal=terminal;
      destroyPath();
    }
    
  }
  
  
  
}


#endif
