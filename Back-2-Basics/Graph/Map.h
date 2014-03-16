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
using namespace Collection;
#include "Graph.h"
using namespace Graph;

namespace Graph {
  
  
  template <class NodeType, class EdgeType>
  class Location;
  
  template <class LocationType, class ViaType>
  class Via : public Edge<LocationType,ViaType>
  {
    
  protected:
    void initialize(Location<LocationType,Via> *v, Location<LocationType,Via> *u, int64_t length) {
      this->u = u;
      this->v = v;
      
      v->references++;
      u->addEdge((ViaType *)this);
      
      this->length = length;
    }
    
  public:
    Via<LocationType>() {
      
    }
    
    Via<LocationType>(Location<LocationType,Via> *v, Location<LocationType,Via> *u, int64_t length) {
      initialize(v,u,length);
    }
    
    inline int64_t length() {
      return this->attrib;
    }
    
    inline void setLength(int64_t length) {
      this->attrib = length;
    }
  };
  
  
  template <class NodeType, class EdgeType>
  class Location : public Node<NodeType,EdgeType>
  {
    
  public:
    
    bool        explored = false;
    uint64_t    distanceFromStart = UINT64_MAX;
    uint64_t    *auxIndex = NULL;
    
  public:
    
    Location() {
      
    }
    
  };
  
  
  /* Via */
  
  /* Location */
  
  template <class NodeType, class EdgeType>
  class Map : public Graph<NodeType, EdgeType>
  {
  protected:
    
    vector<EdgeType *> *shortestPathToTerminal;
    uint64_t numNegativeEdges;
    vector<EdgeType *> *shortPathTree;
    
  private:
    
    void destroyPath();
    
  protected:
    
    static uint64_t bellmanFordsGambit(LLRB_TreeNode<EdgeType *, uint64_t> *current, void *reserved) {
      
      EdgeType *uv = current->data;
      
      if (!uv->blocked) {
        
        NodeType *u = uv->getBackward();
        NodeType *v = uv->getForward();
        
        if ((u->distanceFromStart + uv->length()) < v->distanceFromStart) {
          v->distanceFromStart = u->distanceFromStart + uv->length();
          v->previousEdge = uv;
        }
      }
      return current->key;
    }
    
    static uint64_t dijkstrasGambit(LLRB_TreeNode<EdgeType *, uint64_t> *current, void *queue) {
      
      if (!current->data->blocked) {
        
        NodeType *u = current->data->getBackward();
        NodeType *v = current->data->getForward();
        int64_t tmp = u->distanceFromStart + current->data->length();
        
        if (tmp < v->distanceFromStart) {
          v->distanceFromStart = tmp;
          v->previousEdge = current->data;
          
          if (v->auxIndex == NULL) {
            v->auxIndex = ((Heap<NodeType *,int64_t> *)queue)->push(v, tmp);
          } else {
            v->auxIndex = ((Heap<NodeType *,int64_t> *)queue)->update(*(v->auxIndex), tmp);
          }
        }
      }
      return current->key;
    }
    
    void dijkstras();
    void bellmanFord();
    
  public:
    
    Map();
    ~Map();
    
    EdgeType *addEdge(NodeType *a, NodeType *b, int64_t location);
    void setStart(NodeType *start);
    void setTerminal(NodeType *terminal);
    virtual vector<EdgeType *> *getShortestPath();
    
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
    //Graph<NodeType,EdgeType>();
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
    vector<EdgeType *> *ret = new vector<EdgeType *>();
    NodeType *u, *v;
    EdgeType *uv;
    
    vector<NodeType *> *nodes = this->getReachableNodes(this->start,this->terminal);
    
    nodes->push_back(this->terminal);
    u = nodes->at(0);
    nodes->at(0) = this->start;
    nodes->push_back(u);
    
    this->start->distanceFromStart = 0.0;
    
    for (int ix = 1; ix < nodes->size(); ix++) {
      u = nodes->at(ix);
      u->modifyAllAdjacent(bellmanFordsGambit,0);
    }
    
    vector<EdgeType *> *edges = this->getEdges(nodes);
    
    for (int ix = 0; ix < edges->size(); ix++) {
      uv = edges->at(ix);
      
      if (uv->blocked) {
        continue;
      }
      
      u = uv->getBackward();
      v = uv->getForward();
      if ((u->distanceFromStart + uv->length()) < v->distanceFromStart) {
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
        ret->push_back(u->previousEdge);
      }
      
      u = u->previousEdge->getBackward();
    }
    
    for (uint64_t ix = 0; ix < nodes->size(); ix++) {
      nodes->at(ix)->distanceFromStart = 1.0/0.0;
      nodes->at(ix)->previousEdge = NULL;
    }
    
    shortestPathToTerminal = ret;
  }
  
  
  /* Dijkstra's (For non-negative graphs) */
  template <class NodeType, class EdgeType>
  void Map<NodeType,EdgeType>::dijkstras() {
    Heap<NodeType *,int64_t> *queue = new Heap<NodeType *,int64_t>();
    NodeType *u;
    vector<EdgeType *> *ret;
    vector<NodeType *> *dirtyNodes = new vector<NodeType *>();
    
    this->start->distanceFromStart = 0.0;
    
    this->start->auxIndex = queue->push(this->start,0.0);
    
    while (queue->size()) {
      u = queue->at(0);
      if (u->distanceFromStart == 1.0/0.0) {
        break;
      }
      
      u = queue->pop();
      u->auxIndex = NULL;
      
      dirtyNodes->push_back(u);
      
      u->modifyAllAdjacent(dijkstrasGambit,queue);
      
    }
    
    // Contruct Tree
    
    if (shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
    }
    
    ret = new vector<EdgeType *>();
    
    u = this->terminal;
    
    while ((u->previousEdge != NULL) && (u != this->start)) {
      if (u->previousEdge->getBackward() != NULL) {
        ret->push_back(u->previousEdge);
      }
      
      u = u->previousEdge->getBackward();
    }
    
    shortestPathToTerminal = ret;
    
    // reset initial distances to infinity
    for (uint64_t ix = 0; ix < dirtyNodes->size(); ix++) {
      dirtyNodes->at(ix)->distanceFromStart = 1.0/0.0;
      dirtyNodes->at(ix)->previousEdge = NULL;
    }
    
    dirtyNodes->resize(0);
    
    delete dirtyNodes;
    
  }
  
  template<class NodeType, class EdgeType>
  vector<EdgeType *> *Map<NodeType,EdgeType>::getShortestPath() {
    
    if (shortestPathToTerminal != NULL) {
      delete shortestPathToTerminal;
      shortestPathToTerminal = NULL;
    }
    
    if (this->numNegativeEdges) {
      bellmanFord();
    } else {
      dijkstras();
    }
    
    return shortestPathToTerminal;
    
  }
  
  template <class NodeType, class EdgeType>
  EdgeType *Map<NodeType,EdgeType>::addEdge(NodeType *v, NodeType *u,
                                            int64_t location) {
    EdgeType *tmpEdge = new EdgeType(v,u,location);
    
    if (location < 0.0) {
      numNegativeEdges++;
    }
    
    return tmpEdge;
  }
  
  template <class NodeType, class EdgeType>
  void Map<NodeType,EdgeType>::setStart(NodeType *start) {
    if (this->start != start) {
      this->start=start;
      
      if (shortestPathToTerminal == NULL) {
        return;
      }
      
      while (start != shortestPathToTerminal->back()->getBackward()) {
        if (shortestPathToTerminal->back() == NULL) {
          destroyPath();
        }
        shortestPathToTerminal->resize(shortestPathToTerminal->size()-1);
      }
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
