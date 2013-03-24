//
//  Graph.h
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 8/30/12.
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

#ifndef __Algorithms__Graph__
#define __Algorithms__Graph__

#include <iostream>
using namespace std;

#include "Stack.h"
#include "LLRB_Tree.h"
using namespace Tree;


namespace Graph {
  
  struct Adjacency {
    uint64_t uLayer;
    uint64_t vLayer;
    uint64_t uPosition;
    uint64_t vposition;
  };
  
  template <class NodeType, class EdgeType>
  class Edge;
  template <class NodeType, class EdgeType>
  class Node;
  
  template <class NodeType, class EdgeType>
  class Graph;
  
  
  template <class NodeType, class EdgeType>
  class Edge
  {
    
  protected:
    NodeType    *u;
    NodeType    *v;
    
    double      attrib = 0.0;
    
    void initialize(NodeType *v, NodeType *u);
    void deinitialize();
    
  public:
    
    bool        blocked = false;
    
    Edge() {
      
    }
    
    Edge(NodeType *v, NodeType *u) {
      initialize(v,u);
    }
    
    NodeType *getForward();
    NodeType *getBackward();
    
    friend class Node<NodeType,EdgeType>;
  };
  
  
  
  template <class NodeType, class EdgeType>
  class Node
  {
    
  protected:
    LLRB_Tree<EdgeType *, uint64_t> *forwardEdges = NULL;
    
    void initialize();
    void deinitialize();
    
  public:
    uint64_t    discovered = 0;
    uint64_t    references = 0;
    EdgeType    *previousEdge = NULL;
    
  public:
    
    Node() {
      initialize();
    }
    
    ~Node() {
      deinitialize();
    }
    
    void addEdge(EdgeType *);
    void removeEdge(EdgeType *);
    
    void modifyAllAdjacent(uint64_t (*action)(LLRB_TreeNode<EdgeType *, uint64_t> *, void *), void *);
    
    friend class Edge<NodeType,EdgeType>;
  };
  
  
  template <class NodeType, class EdgeType>
  class Graph
  {
  private:
    
  protected:
    NodeType *start;
    NodeType *terminal;
    
    static uint64_t getEdgeEach(LLRB_TreeNode<EdgeType *, uint64_t> *current, void *vect) {
      ((vector<EdgeType *> *)vect)->push_back(current->data);
      
      return current->key;
    }
    
    static uint64_t pathDiscoveryBFSEach(LLRB_TreeNode<EdgeType *, uint64_t> *current, void *nextL) {
      
      NodeType *v;
      
      if (!current->data->blocked) {
        
        if (!(v = current->data->getForward())->discovered) {
          v->previousEdge = current->data;
          ((vector<NodeType *> *)nextL)->push_back(v);
        }
        
        v->discovered++;
        
      }
      
      return current->key;
    }
    
    static uint64_t pathDiscoveryBFSAllEach(LLRB_TreeNode<EdgeType *, uint64_t> *current, void *nextL) {
      
      NodeType *v;
      
      if (!current->data->blocked) {
        if (!(v = current->data->getForward())->discovered) {
          ((vector<NodeType *> *)nextL)->push(v);
        }
        
        v->discovered++;
      }
      
      
      return current->key;
    }
    
    static uint64_t pathDiscoveryFAPEach(LLRB_TreeNode<EdgeType *, uint64_t> *current, void *nextL) {
      
      NodeType *v;
      
      if (!current->data->blocked) {
        
        if ((v = current->data->getForward())->discovered == false) {
          v->previousEdge = current->data;
          ((vector<NodeType *> *)nextL)->push_back(v);
        }
        v->discovered++;
      }
      
      return current->key;
    }
    
  public:
    
    static vector<EdgeType *> *findAPath(NodeType *start, NodeType *terminal);
    static vector<vector<NodeType *> *> *breadthFirstSearch(NodeType *start, NodeType *terminal);
    static vector<vector<NodeType *> *> *breadthFirstSearchAll(NodeType *start);
    
  public:
    
    Graph();
    
    vector<vector<NodeType *> *> *getLayers();
    
    static vector<NodeType *> *getReachableNodes(NodeType *a, NodeType *b);
    static vector<EdgeType *> *getEdges(vector<NodeType *> *nodes);
    
    virtual vector<EdgeType *> *getPathToTerminal();
    vector<EdgeType *> *minimumSpanningTree();
    vector<EdgeType *> *maximumSpanningTree();
    
    void setStart(NodeType *start);
    void setTerminal(NodeType *terminal);
    NodeType *getStart();
    NodeType *getTerminal();
    
  };
  
  
  
  
  
  //
  //  Graph.cpp
  //  Algorithms
  //
  //  Created by Marcus Gilchrist on 8/30/12.
  //  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
  //
  
  
  
  /* Edge */
  
  
  template <class NodeType, class EdgeType>
  void Edge<NodeType,EdgeType>::initialize(NodeType *v, NodeType *u) {
    assert(v != u);
    assert(u != NULL);
    assert(v != NULL);
    
    this->u = u;
    this->v = v;
    
    v->references++;
    u->addEdge((EdgeType *)this);
    
    assert(v->references != 0);
  }
  
  template <class NodeType, class EdgeType>
  void Edge<NodeType,EdgeType>::deinitialize() {
    assert(v != NULL);
    assert(v->references != 0);
    
    v->references--;
    
    if (!(v->references)) {
      delete v;
    }
    u->removeEdge((EdgeType *)this);
  }
  
  template <class NodeType, class EdgeType>
  NodeType *Edge<NodeType,EdgeType>::getForward() {
    return v;
  }
  
  template <class NodeType, class EdgeType>
  NodeType *Edge<NodeType,EdgeType>::getBackward() {
    return u;
  }
  
  
  
  /* Node */

  
  template <class NodeType, class EdgeType>
  void Node<NodeType,EdgeType>::initialize() {
    assert(forwardEdges == NULL);
    
    forwardEdges = new LLRB_Tree<EdgeType *, uint64_t>();
  }
  
  template <class NodeType, class EdgeType>
  void Node<NodeType,EdgeType>::deinitialize() {
    assert(forwardEdges != NULL);
    
    forwardEdges->deletion(NULL,NULL);
    
    forwardEdges = NULL;
  }
  
  template <class NodeType, class EdgeType>
  void Node<NodeType,EdgeType>::addEdge(EdgeType *edge) {
    assert(forwardEdges != NULL);
    
    forwardEdges->insert(edge, (uint64_t)edge);
  }
  
  template <class NodeType, class EdgeType>
  void Node<NodeType,EdgeType>::removeEdge(EdgeType *edge) {
    assert(forwardEdges != NULL);
    
    forwardEdges->remove(edge, (uint64_t)edge);
  }
  
  template <class NodeType, class EdgeType>
  void Node<NodeType,EdgeType>::modifyAllAdjacent(uint64_t (*action)(LLRB_TreeNode<EdgeType *, uint64_t> *, void *), void *object) {
    if (forwardEdges->size()) {
      forwardEdges->modifyAll(action, object);
    }
  }
  
  /* Graph */
  
  template <class NodeType, class EdgeType>
  vector<Adjacency *> *getAdjacencyList(NodeType *a, NodeType *b) {
    
  }
  
  template <class NodeType, class EdgeType>
  vector<NodeType *> *Graph<NodeType,EdgeType>::getReachableNodes(NodeType *a, NodeType *b) {
    vector<vector<NodeType *> *> *layers, *reverseLayers;
    vector<NodeType *> *layer;
    NodeType *node;
    vector<NodeType *> *ret = new vector<NodeType *>();
    uint64_t totalNodes = 0;
    
    layers = breadthFirstSearch(a, b);
    reverseLayers = new vector<vector<NodeType *> *>(layers->size());
    
    while ((!layers->empty()) && ((layer = layers->back()) != NULL)) {
      totalNodes += layer->size();
      reverseLayers->push_back(layer);
      layers->resize(layers->size()-1);
    }
    
    ret->reserve(totalNodes);
    
    while ((!reverseLayers->empty()) && ((layer = reverseLayers->back()) != NULL)) {
      while ((!layer->empty()) && ((node = layer->back()) != NULL)) {
        ret->push_back(node);
        layer->resize(layer->size()-1);
      }
      reverseLayers->pop_back();
    }
    
    delete layers;
    delete reverseLayers;
    
    return ret;
  }
  
  template <class NodeType, class EdgeType>
  vector<EdgeType *> *Graph<NodeType,EdgeType>::getEdges(vector<NodeType *> *nodes) {
    vector<EdgeType *> *ret = new vector<EdgeType *>();
    
    for (uint64_t ix = 0; ix < nodes->size(); ix++) {
      nodes->at(ix)->modifyAllAdjacent(Graph::getEdgeEach, ret);
    }
    
    return ret;
  }
  
  template <class NodeType, class EdgeType>
  vector<vector<NodeType *> *> *
  Graph<NodeType,EdgeType>::breadthFirstSearch(NodeType *start,
                                               NodeType *terminal) {
    
    vector<vector<NodeType *> *> *baseL;
    vector<NodeType *> *L, *first;
    vector<NodeType *> *nextL;
    NodeType *u;
    uint64_t terminalReachCnt = 0, lastTerminalLayer = 0, layerCnt = 0;
    
    start->discovered = 1;
    terminal->discovered = 1;
    
    baseL = new vector<vector<NodeType *> *>();
    L = new vector<NodeType *>();
    first = L;
    L->push_back(start);
    
    while (!(L->empty())) {
      
      // Initialize new layer
      nextL = new vector<NodeType *>;
      baseL->push_back(nextL);
      layerCnt++;
      
      for (uint64_t ix = 0; ix < L->size(); ix++) {
        u = L->at(ix);
        u->modifyAllAdjacent(Graph::pathDiscoveryBFSEach, nextL);
        
        if (terminalReachCnt != terminal->discovered) {
          lastTerminalLayer = layerCnt;
          terminalReachCnt = terminal->discovered;
          if (terminal->discovered > terminal->references) {
            goto bfs_cleanup;
          }
        }
      }
      L = nextL;
    }
    
  bfs_cleanup:
    
    first->resize(0);
    delete first;
    
    for (int ix = 0; ix < baseL->size(); ix++) {
      for (int jx = 0; jx < baseL->at(ix)->size(); jx++) {
        baseL->at(ix)->at(jx)->discovered = 0;
      }
    }
    
    while (layerCnt > lastTerminalLayer) {
      layerCnt--;
      baseL->back()->resize(0);
      baseL->pop_back();
    }
    
    return baseL;
    
  }
  
  template <class NodeType, class EdgeType>
  vector<vector<NodeType *> *> *
  Graph<NodeType,EdgeType>::breadthFirstSearchAll(NodeType *start) {
    
    vector<vector<NodeType *> *> *baseL;
    vector<NodeType *> *L;
    vector<NodeType *> *nextL;
    NodeType *u;
    
    start->discovered = 1;
    
    baseL = new vector<vector<NodeType *> *>();
    L = new vector<NodeType *>();
    
    L->push(start);
    
    
    while (!L->empty()) {
      
      // Initialize new layer
      nextL = new vector<NodeType *>;
      baseL->push(nextL);
      
      for (uint64_t ix = 0; ix < L->size(); ix++) {
        u = L->at(ix);
        u->modifyAllAdjacent(Graph::pathDiscoveryBFSAllEach, nextL);
      }
      
      L = nextL;
      
    }
    
    
    for (int ix = 0; ix < baseL->size(); ix++) {
      for (int jx = 0; jx < baseL->atIndex(ix)->size(); jx++) {
        baseL->atIndex(ix)->atIndex(jx)->discovered = 0;
      }
    }
    
    return baseL;
    
  }
  
  
  template <class NodeType, class EdgeType>
  vector<EdgeType *> *
  Graph<NodeType,EdgeType>::findAPath(NodeType *start,
                                      NodeType *terminal) {
    
    vector<vector<NodeType *> *> *baseL;
    vector<NodeType *> *L;
    vector<NodeType *> *nextL;
    NodeType *u, *v;
    vector<EdgeType *> *ret;
    
    start->discovered = 1;
    
    baseL = new vector<vector<NodeType *> *>();
    L = new vector<NodeType *>();
    baseL->push_back(L);
    
    L->push_back(start);
    
    while (!L->empty()) {
      
      // Initialize new layer
      nextL = new vector<NodeType *>;
      baseL->push_back(nextL);
      
      while ((!L->empty()) && ((u = L->back()) != NULL)) {
        u->modifyAllAdjacent(Graph::pathDiscoveryFAPEach, nextL);
        
        L->resize(L->size()-1);
        
        if (terminal->discovered) {
          goto findAPath_cleanup;
        }
      }
      
    L = nextL;
    }
    
  findAPath_cleanup:
    
    delete baseL;
    
    ret = new vector<EdgeType *>();
    
    v = terminal;
    
    while (v->previousEdge != NULL) {
      if (v->previousEdge->getBackward() != NULL) {
        ret->push_back(v->previousEdge);
      }
      v = v->previousEdge->getBackward();
    }
    
    return ret;
    
  }
  
  template <class NodeType, class EdgeType>
  Graph<NodeType,EdgeType>::Graph() {
    
  }
  
  template <class NodeType, class EdgeType>
  vector<vector<NodeType *> *> *Graph<NodeType,EdgeType>::getLayers() {
    
    return breadthFirstSearch(start, terminal);
  }
  
  template <class NodeType, class EdgeType>
  vector<EdgeType *> *Graph<NodeType,EdgeType>::getPathToTerminal() {
    
    return findAPath(start, terminal);
  }
  
  
  template <class NodeType, class EdgeType>
  vector<EdgeType *> *Graph<NodeType,EdgeType>::minimumSpanningTree() {
    vector<NodeType *> *nodes = this->getReachableNodes(this->start,this->terminal);
    vector<EdgeType *> *ret = this->getEdges(nodes);
    LLRB_Tree<EdgeType *, double> edges = new LLRB_Tree<EdgeType *, double>(false);
    
    for (uint64_t ix = 0; ix < ret->size(); ix++) {
      edges.insert(ret->at(ix), (ret->at(ix))->attrib);
    }
    
    ret->resize(0);
    
    while ((ret->size() < nodes->size()) && edges.size() ) {
      EdgeType *edge = edges.min(edges.treeRoot)->data;
      NodeType *u = edge->getBackward();
      NodeType *v = edge->getForward();
      
      if ((u->previousEdge->getBackward() != v->previousEdge->getBackward()) ||
          (v->previousEdge->getBackward() == NULL)) {
        NodeType *tmp = v->previousEdge->getBackward();
        for (uint64_t ix = 0; ix < nodes->size(); ix++) {
          if (nodes->at(ix) == tmp ) {
            nodes->at(ix) = u->previousEdge->getBackward();
          }
        }
        
        ret->push_back(edge);
      }
      
      edges.removeMin();
    }
    
    /* Cleanup */
    for (uint64_t ix = 0; ix < nodes->size(); ix++) {
      nodes->at(ix)->previousEdge = NULL;
    }
    
    nodes->resize(0);
    delete nodes;
    
    return ret;
    
  }
  
  template <class NodeType, class EdgeType>
  vector<EdgeType *> *Graph<NodeType,EdgeType>::maximumSpanningTree() {
    vector<NodeType *> *nodes = this->getReachableNodes(this->start,this->terminal);
    vector<EdgeType *> *ret = this->getEdges(nodes);
    LLRB_Tree<EdgeType *, double> edges = new LLRB_Tree<EdgeType *, double>(false);
    
    for (uint64_t ix = 0; ix < ret->size(); ix++) {
      edges.insert(ret->at(ix), -(ret->at(ix))->attrib); /* multiply by -1 to get maximum */
    }
    
    ret->resize(0);
    
    while ((ret->size() < nodes->size()) && edges.size() ) {
      EdgeType *edge = edges.min(edges.treeRoot)->data;
      NodeType *u = edge->getBackward();
      NodeType *v = edge->getForward();
      
      if ((u->previousEdge->getBackward() != v->previousEdge->getBackward()) ||
          (v->previousEdge->getBackward() == NULL)) {
        NodeType *tmp = v->previousEdge->getBackward();
        for (uint64_t ix = 0; ix < nodes->size(); ix++) {
          if (nodes->at(ix) == tmp ) {
            nodes->at(ix) = u->previousEdge->getBackward();
          }
        }
        
        ret->push_back(edge);
      }
      
      edges.removeMin();
    }
    
    /* Cleanup */
    for (uint64_t ix = 0; ix < nodes->size(); ix++) {
      nodes->at(ix)->previousEdge = NULL;
    }
    
    nodes->resize(0);
    delete nodes;
    
    return ret;
    
  }

  
  template <class NodeType, class EdgeType>
  void Graph<NodeType,EdgeType>::setStart(NodeType *start) {
    this->start=start;
  }
  
  template <class NodeType, class EdgeType>
  void Graph<NodeType,EdgeType>::setTerminal(NodeType *terminal) {
    this->terminal=terminal;
  }
  
  template <class NodeType, class EdgeType>
  NodeType *Graph<NodeType,EdgeType>::getStart() {
    return start;
  }
  
  template <class NodeType, class EdgeType>
  NodeType *Graph<NodeType,EdgeType>::getTerminal() {
    return terminal;
  }
  
  
  
}



#endif /* defined(__Algorithms__Graph__) */
