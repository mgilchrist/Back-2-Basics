//
//  Graph.h
//  Algorithms
//
//  Created by Marcus Gilchrist on 8/30/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#ifndef __Algorithms__Graph__
#define __Algorithms__Graph__

#include <iostream>

#include "Stack.h"


namespace Graph {
  
  
  template <class NodeType>
  class Edge;
  template <class NodeType, class EdgeType>
  class Node;
  
  template <class NodeType, class EdgeType>
  class Graph;
  
  
  
  
  template <class NodeType>
  class Edge
  {
    
  protected:
    NodeType    *u;
    NodeType    *v;
    uint64_t    index;
    
  public:
    
    bool        blocked;
    
    Edge();
    Edge(NodeType *v, NodeType *u);
    
    virtual void setIndex(uint64_t index);
    virtual uint64_t getIndex();
    
    virtual NodeType *getForward();
    virtual NodeType *getBackward();
  };
  
  
  
  template <class NodeType, class EdgeType>
  class Node
  {
    
  protected:
    Collection::Stack<EdgeType *> *forwardEdges;
    
  public:
    bool        discovered;
    uint64_t    index;
    EdgeType    *previousEdge;
    
  public:
    
    Node();
    virtual void setIndex(uint64_t);
    virtual uint64_t getIndex();
    virtual uint64_t getNumEdges();
    virtual void addAdjacentEdge(EdgeType *adjacent);
    virtual EdgeType *removeNewestEdge();
    virtual EdgeType *getAdjacentEdge(uint64_t index);
    virtual NodeType *getAdjacentNode(uint64_t index);
  };
  
  
  template <class NodeType, class EdgeType>
  class Graph
  {
  private:
    Collection::Stack<NodeType *> *nodes;
    Collection::Stack<EdgeType *> *edges;
    
  protected:
    NodeType *start;
    NodeType *terminal;
    
  public:
    Collection::Stack<Collection::Stack<NodeType *> *> *layers;
    Collection::Stack<EdgeType *> *pathToTerminal;
    
    virtual Collection::Stack<EdgeType *> *breadthFirstSearch(NodeType *start, NodeType *terminal);
    virtual Collection::Stack<Collection::Stack<NodeType *> *> *breadthFirstSearch(NodeType *start);
    
    virtual void swapNodeIndexes(uint64_t a,uint64_t b);
    
  public:
    
    Graph();
    ~Graph();
    
    virtual void add(NodeType *node);
    virtual void addEdge(EdgeType *edge);
    
    virtual Collection::Stack<Collection::Stack<NodeType *> *> *getLayers();
    
    virtual Collection::Stack<EdgeType *> *getPathToTerminal();
    
    virtual NodeType *nodeAtIndex(uint64_t index);
    virtual EdgeType *edgeAtIndex(uint64_t index);
    virtual uint64_t getNumNodes();
    virtual uint64_t getNumEdges();
    virtual void setStart(NodeType *start);
    virtual void setTerminal(NodeType *terminal);
    virtual NodeType *getStart();
    virtual NodeType *getTerminal();
    
  };
  
  
  
  
  
  //
  //  Graph.cpp
  //  Algorithms
  //
  //  Created by Marcus Gilchrist on 8/30/12.
  //  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
  //
  
  
  
  /* Node */
  
  template <class NodeType>
  Edge<NodeType>::Edge() {
    
  }
  
  template <class NodeType>
  Edge<NodeType>::Edge(NodeType *v, NodeType *u) {
    this->u = u;
    this->v = v;
  }
  
  template <class NodeType>
  void Edge<NodeType>::setIndex(uint64_t index) {
    this->index = index;
  }
  
  template <class NodeType>
  uint64_t Edge<NodeType>::getIndex() {
    return index;
  }
  
  template <class NodeType>
  NodeType *Edge<NodeType>::getForward() {
    return v;
  }
  
  template <class NodeType>
  NodeType *Edge<NodeType>::getBackward() {
    return u;
  }
  
  
  
  /* Node */
  
  template <class NodeType, class EdgeType>
  Node<NodeType,EdgeType>::Node() {
    forwardEdges = new Collection::Stack<EdgeType *>();
    discovered = false;
  }
  
  template <class NodeType, class EdgeType>
  uint64_t Node<NodeType,EdgeType>::getNumEdges() {
    return forwardEdges->getSize();
  }
  
  template <class NodeType, class EdgeType>
  uint64_t Node<NodeType,EdgeType>::getIndex() {
    return index;
  }
  
  template <class NodeType, class EdgeType>
  void Node<NodeType,EdgeType>::setIndex(uint64_t index) {
    this->index = index;
  }
  
  template <class NodeType, class EdgeType>
  void Node<NodeType,EdgeType>::addAdjacentEdge(EdgeType *adjacent) {
    forwardEdges->push(adjacent);
  }
  
  template <class NodeType, class EdgeType>
  EdgeType *Node<NodeType,EdgeType>::removeNewestEdge() {
    return forwardEdges->pop();
  }
  
  template <class NodeType, class EdgeType>
  EdgeType *Node<NodeType,EdgeType>::getAdjacentEdge(uint64_t index) {
    return forwardEdges->atIndex(index);
  }
  
  template <class NodeType, class EdgeType>
  NodeType *Node<NodeType,EdgeType>::getAdjacentNode(uint64_t index) {
    EdgeType *edge = forwardEdges->atIndex(index);
    
    if (edge->getForward() != this) {
      return edge->getForward();
    } else {
      return edge->getBackward();
    }
  }
  
  
  
  /* Graph */
  
  template <class NodeType, class EdgeType>
  Collection::Stack<Collection::Stack<NodeType *> *> *
  Graph<NodeType,EdgeType>::breadthFirstSearch(NodeType *start) {
    
    Collection::Stack<Collection::Stack<NodeType *> *> *baseL;
    Collection::Stack<NodeType *> *L;
    Collection::Stack<NodeType *> *nextL;
    NodeType *u, *v;
    
    for (int ix = 0; ix < this->getNumNodes(); ix++) {
      this->nodeAtIndex(ix)->discovered = false;
    }
    
    start->discovered = true;
    
    baseL = new Collection::Stack<Collection::Stack<NodeType *> *>();
    L = new Collection::Stack<NodeType *>;
    baseL->push(L);
    
    L->push(start);
    
    
    while (L->peek() != NULL) {
      
      // Initialize new layer
      nextL = new Collection::Stack<NodeType *>;
      baseL->push(nextL);
      
      while ((u = L->pop()) != NULL) {
        for (int jx = 0; jx < u->getNumEdges(); jx++) {
          if (u->getAdjacentEdge(jx)->blocked) {
            continue;
          }
          if ((v = u->getAdjacentNode(jx))->discovered == false) {
            v->discovered = true;
            nextL->push(v);
          }
        }
      }
    }
    
    return baseL;
    
  }
  
  
  template <class NodeType, class EdgeType>
  Collection::Stack<EdgeType *> *
  Graph<NodeType,EdgeType>::breadthFirstSearch(NodeType *start,
                                               NodeType *terminal) {
    
    Collection::Stack<Collection::Stack<NodeType *> *> *baseL;
    Collection::Stack<NodeType *> *L;
    Collection::Stack<NodeType *> *nextL;
    NodeType *u, *v;
    Collection::Stack<EdgeType *> *ret;
    
    for (int ix = 0; ix < this->getNumNodes(); ix++) {
      this->nodeAtIndex(ix)->discovered = false;
    }
    
    start->discovered = true;
    
    baseL = new Collection::Stack<Collection::Stack<NodeType *> *>();
    L = new Collection::Stack<NodeType *>;
    baseL->push(L);
    
    L->push(start);
    
    while (L->peek() != NULL) {
      
      // Initialize new layer
      nextL = new Collection::Stack<NodeType *>;
      baseL->push(nextL);
      
      while ((u = L->pop()) != NULL) {
        for (int jx = 0; jx < u->getNumEdges(); jx++) {
          if (u->getAdjacentEdge(jx)->blocked) {
            continue;
          }
          if ((v = u->getAdjacentNode(jx))->discovered == false) {
            v->discovered = true;
            v->previousEdge = u->getAdjacentEdge(jx);
            nextL->push(v);
            if (v == terminal) {
              goto bfs_cleanup;
            }
          }
        }
      }
    }
    
  bfs_cleanup:
    
    for (int ix = 0; ix < baseL->getSize(); ix++) {
      delete baseL->atIndex(ix);
    }
    delete baseL;
    
    ret = new Collection::Stack<EdgeType *>();
    
    v = terminal;
    
    while (v->previousEdge != NULL) {
      if (v->previousEdge->getBackward() != NULL) {
        ret->push(v->previousEdge);
      }
    }
    
    this->pathToTerminal = ret;
    
    return ret;
    
  }
  
  template <class NodeType, class EdgeType>
  void Graph<NodeType,EdgeType>::swapNodeIndexes(uint64_t a,uint64_t b) {
    NodeType *tmp;
    
    tmp = nodes->atIndex(a);
    nodes->setIndex(a, nodes->atIndex(b));
    nodes->setIndex(b, tmp);
    
    nodes->atIndex(a)->index = a;
    nodes->atIndex(b)->index = b;
  }
  
  template <class NodeType, class EdgeType>
  Graph<NodeType,EdgeType>::Graph() {
    nodes = new Collection::Stack<NodeType *>();
    edges = new Collection::Stack<EdgeType *>();
  }
  
  template <class NodeType, class EdgeType>
  Graph<NodeType,EdgeType>::~Graph() {
    for (int ix = 0; ix < nodes->getSize(); ix++) {
      delete nodes->atIndex(ix);
    }
    for (int ix = 0; ix < edges->getSize(); ix++) {
      delete edges->atIndex(ix);
    }
    delete nodes;
    delete edges;
  }
  
  template <class NodeType, class EdgeType>
  void Graph<NodeType,EdgeType>::add(NodeType *node) {
    node->index = nodes->getSize();
    nodes->push(node);
  }
  
  template <class NodeType, class EdgeType>
  void Graph<NodeType,EdgeType>::addEdge(EdgeType *edge) {
    edge->setIndex(edges->getSize());
    edges->push(edge);
  }
  
  template <class NodeType, class EdgeType>
  Collection::Stack<Collection::Stack<NodeType *> *> *Graph<NodeType,EdgeType>::getLayers() {
    
    if (layers == NULL) {
      layers = breadthFirstSearch(start);
    }
    
    return layers;
  }
  
  template <class NodeType, class EdgeType>
  Collection::Stack<EdgeType *> *Graph<NodeType,EdgeType>::getPathToTerminal() {
    
    if (pathToTerminal == NULL) {
      breadthFirstSearch(start, terminal);
    }
    
    return pathToTerminal;
  }
  
  template <class NodeType, class EdgeType>
  NodeType *Graph<NodeType,EdgeType>::nodeAtIndex(uint64_t index) {
    return nodes->atIndex(index);
  }
  
  template <class NodeType, class EdgeType>
  EdgeType *Graph<NodeType,EdgeType>::edgeAtIndex(uint64_t index) {
    return edges->atIndex(index);
  }
  
  template <class NodeType, class EdgeType>
  uint64_t Graph<NodeType,EdgeType>::getNumNodes() {
    return nodes->getSize();
  }
  
  template <class NodeType, class EdgeType>
  uint64_t Graph<NodeType,EdgeType>::getNumEdges() {
    return edges->getSize();
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
