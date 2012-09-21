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

#include "Graph.h"

namespace Graph {
  
  
  template <class NodeType, class EdgeType>
  class Location;
  
  template <class LocationType>
  class Link : public Edge<LocationType>
  {
  public:
    double      length;
    Link<LocationType>();
    Link<LocationType>(LocationType *v, LocationType *u, double length);
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
  
  
  /* Link */
  
  template <class LocationType>
  Link<LocationType>::Link(LocationType *v, LocationType *u, double length) {
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
    
    DataStructures::Stack<EdgeType *> shortestPathToTerminal;
    uint64_t numNegativeEdge;
    DataStructures::Stack<EdgeType *> *shortPathTree;
    
  private:
    
    void destroyPath();
    
  protected:
    
    void dijkstras();
    void bellmanFord();
    
  public:
    
    Map();
    ~Map();
    
    virtual EdgeType *addEdge(NodeType *a, NodeType *b, double location);
    virtual void setStart(NodeType *start);
    virtual void setTerminal(NodeType *terminal);
    virtual DataStructures::Stack<EdgeType *> *getShortestPath();
    
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
    numNegativeEdge = 0;
  }
  
  template <class NodeType, class EdgeType>
  Map<NodeType,EdgeType>::~Map() {
    
    if (shortPathTree != NULL) {
      delete shortPathTree;
    }
    
    if (shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
    }
    
    ~Graph<NodeType,EdgeType>();
  }
  
  /* Bellman-Ford */
  
  template <class NodeType, class EdgeType>
  void Map<NodeType,EdgeType>::bellmanFord() {
    Collection::Stack<EdgeType *> *ret = new Collection::Stack<EdgeType *>();
    NodeType *u, *v;
    EdgeType *uv;
    
    
    for (int ix = 0; ix < this->getNumNodes(); ix++) {
      this->nodeAtIndex(ix)->distance = 1.0/0.0;
      this->nodeAtIndex(ix)->previousNode = NULL;
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
        if ((u->distance + uv->weight) < v->distance) {
          v->distance = u->distance + uv->weight;
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
      if ((u->distance + uv->weight) < v->distance) {
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
    
    shortestPathToTerminal = new Collection::Stack<EdgeType *>();
    
    u = this->terminal;
    
    while (u->previousEdge != NULL) {
      if (u->previousEdge->getBackward() != NULL) {
        ret->push(u->previousEdge);
      }
    }
  }
  
  
  /* Dijkstra's (For non-negative graphs) */
  template <class NodeType, class EdgeType>
  void Map<NodeType,EdgeType>::dijkstras() {
    Collection::Heap<NodeType,double> *queue;
    Collection::Comparable<NodeType,double> *heapEntry;
    NodeType *u;
    double tmp;
    Collection::Stack<EdgeType *> ret;
    
    // set initial distances to infinity
    for (int ix = 0; ix < this->getNumNodes(); ix++) {
      this->nodeAtIndex(ix)->distanceFromStart = 1.0/0.0;
      this->nodeAtIndex(ix)->previousNode = NULL;
    }
    
    this->start->distanceFromStart = 0.0;
    
    for (int ix = 0; ix < this->getNumNodes(); ix++) {
      heapEntry = new Collection::Comparable<NodeType,double>(this->nodeAtIndex(ix),
                                                              this->nodeAtIndex(ix)->distanceFromStart);
      this->nodeAtIndex(ix)->auxIndex = addHeapEntry(queue, heapEntry);
    }
    
    while (!isHeapEmpty(queue)) {
      u = peakAtHeap(queue);
      if (u->distanceFromStart == 1.0/0.0) {
        break;
      }
      
      u = removeFromHeap(queue, 0);
      
      for (int ix = 0; ix < u->getNumForward(); ix++) {
        if (u->getAdjacentEdge(ix)->blocked) {
          continue;
        }
        tmp = u->distanceFromStart + u->getAdjacentEdge(ix)->length;
        
        if (tmp < u->getAdjacentNode(ix)->distanceFromStart) {
          u->getAdjacentNode(ix)->distanceFromStart = tmp;
          u->getAdjacentNode(ix)->previousEdge = u->getAdjacentEdge(ix);
          heapifyUp(queue, u->getAdjacentNode(ix)->auxIndex);
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
    
    while (u->previousEdge != NULL) {
      if (u->previousEdge->getBackward() != NULL) {
        ret->push(u->previousEdge);
      }
    }
    
    return ret;
    
  }
  
  template<class NodeType, class EdgeType>
  Collection::Stack<EdgeType *> *Map<NodeType,EdgeType>::getShortestPath() {
    
    if (shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
      shortestPathToTerminal = NULL;
    }
    
    if (this->numNegativeEdges) {
      shortestPathToTerminal = bellmanFord(this, this->start);
    } else {
      shortestPathToTerminal = dijkstras(this, this->start);
    }
    
    return shortestPathToTerminal.clone();
    
  }
  
  template <class NodeType, class EdgeType>
  EdgeType *Map<NodeType,EdgeType>::addEdge(NodeType *a, NodeType *b,
                                            double location) {
    
    if (location < 0.0) {
      numNegativeEdge++;
    }
    
    return addEdge(new EdgeType(a,b,location));
  }
  
  template <class NodeType, class EdgeType>
  void Map<NodeType,EdgeType>::setStart(NodeType *start) {
    if (this->start != start) {
      this->start=start;
      
      do {
        if (shortestPathToTerminal->peek() == NULL) {
          destroyPath();
        }
      } while (start != shortestPathToTerminal->pop());
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
