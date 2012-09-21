//
//  Network.h
//  OpenSource
//
//  Created by Marcus Gilchrist on 9/16/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#ifndef OpenSource_Network_h
#define OpenSource_Network_h

#include "Graph.h"

namespace Graph {
  
  
  
  template <class NodeType, class EdgeType>
  class Hub;
  
  
  template <class HubType>
  class Pipe : public Edge<HubType>
  {
  public:
    double      capacity;
    
  public:
    Pipe<HubType>();
    Pipe<HubType>(HubType *v, HubType *u, double capacity);
  };
  
  
  template <class NodeType, class EdgeType>
  class Hub : public Node<NodeType,EdgeType>
  {
    
  public:
    double        capacity;
    
  public:
    
    Hub();
  };
  
  
  /* Pipe */
  
  template <class HubType>
  Pipe<HubType>::Pipe() {
    
  }
  
  template <class HubType>
  Pipe<HubType>::Pipe(HubType *v, HubType *u, double capacity) {
    this->u = u;
    this->v = v;
    capacity = capacity;
  }
  
  
  /* Hub */
  
  template <class NodeType, class EdgeType>
  Hub<NodeType,EdgeType>::Hub() {
    this->forwardEdges = new Collection::Stack<EdgeType *>();
    this->discovered = false;
    capacity = 0.0;
  }
  
  
  /* Network */
  
  template <class NodeType, class EdgeType>
  class Network : public Graph<NodeType, EdgeType>
  {
    Collection::Array<double> *flow;
    
  protected:
    Network *createAuxNetwork();
    void deleteAuxNetwork(Network<NodeType, EdgeType> *auxGraph);
    double bottleneck(double *flow, Collection::Stack<EdgeType *> *path);
    double *augment(double *flow, Collection::Stack<EdgeType *> *path);
    void maxFlow(NodeType *s, NodeType *t);
    
  public:
    Network();
    ~Network();
    
    virtual Collection::Array<double> *getMaximumFlow();
  };
  
  template <class NodeType, class EdgeType>
  Network<NodeType,EdgeType>::Network() {
    Graph<NodeType,EdgeType>();
    flow = NULL;
  }
  
  template <class NodeType, class EdgeType>
  Network<NodeType,EdgeType>::~Network() {
    if (flow != NULL) {
      delete flow;
    }
  }
  
  
  /* Network Flow */
  template <class NodeType, class EdgeType>
  Network<NodeType,EdgeType> *Network<NodeType,EdgeType>::createAuxNetwork() {
    Network<NodeType,EdgeType> *auxGraph = new Network<NodeType,EdgeType>();
    EdgeType *thisEdge;
    
    for (int ix = 0; ix < this->getNumNodes(); ix++) {
      auxGraph->add(this->nodeAtIndex(ix));
    }
    
    for (int ix = 0; ix < this->getNumEdges(); ix++) {
      thisEdge = this->edgeAtIndex(ix);
      auxGraph->addEdge(thisEdge);
      
      thisEdge = new EdgeType(thisEdge->getBackward(), thisEdge->getForward(),
                              thisEdge->capacity);
      
      thisEdge->getBackward()->addAdjacentEdge(thisEdge);
      thisEdge->blocked = true;
      
    }
    return auxGraph;
  }
  
  template <class NodeType, class EdgeType>
  void Network<NodeType,EdgeType>::deleteAuxNetwork(Network<NodeType, EdgeType> *auxGraph) {
    EdgeType *thisEdge;
    
    for (int ix = 1; ix < auxGraph->getNumEdges(); ix+=2) {
      thisEdge = this->edgeAtIndex(ix);
      
      thisEdge = new EdgeType(thisEdge->getBackward(), thisEdge->getForward(),
                              thisEdge->capacity);
      
      thisEdge->getBackward()->removeNewestEdge();
      delete thisEdge;
      
    }
    
    delete auxGraph;
  }
  
  template <class NodeType, class EdgeType>
  double Network<NodeType,EdgeType>::bottleneck(double *flow, Collection::Stack<EdgeType *> *path) {
    double bottleneck = path->atIndex(0)->capacity - flow[0];
    
    for (int ix = 1; ix < path->getSize(); ix++) {
      if (bottleneck > (path->atIndex(ix)->capacity - flow[ix])) {
        bottleneck = (path->atIndex(ix)->capacity - flow[ix]);
      }
    }
    
    return bottleneck;
  }
  
  template <class NodeType, class EdgeType>
  double *Network<NodeType,EdgeType>::augment(double *flow,
                                              Collection::Stack<EdgeType *> *path) {
    
    double *fPrime = new double[path->getSize()];
    double b = bottleneck(flow, path);
    
    for (int ix = 0; ix < path->getSize(); ix++) {
      if ((path->atIndex(ix)->getIndex()) % 2) {
        fPrime[ix] = flow[ix] - b;
      } else {
        fPrime[ix] = flow[ix] + b;
      }
    }
    
    return fPrime;
  }
  
  template<class NodeType, class EdgeType>
  void Network<NodeType,EdgeType>::maxFlow(NodeType *s, NodeType *t) {
    
    Collection::Stack<EdgeType *> *path;
    Collection::Array<double> *flow;
    double *subFlow, *subFlowPrime;
    
    Network<NodeType,EdgeType> *auxGraph = createAuxNetwork();
    
    flow = new Collection::Array<double>(this->getNumEdges());
    
    if (this->pathToTerminal != NULL) {
      delete this->pathToTerminal;
      this->pathToTerminal = NULL;
    }
    
    for (int ix = 0; ix < this->getNumEdges(); ix++) {
      flow[ix] = 0.0;
    }
    
    while ((path = auxGraph->breadthFirstSearch(s, t)) != NULL) {
      subFlow = new double[path->getSize()];
      
      for (int ix = 0; ix < path->getSize(); ix++) {
        subFlow[ix] = flow->atIndex((path->atIndex(ix)->getIndex())/2);
      }
      
      subFlowPrime = this->augment(subFlow, path);
      
      for (int ix = 0; ix < path->getSize(); ix++) {
        flow[(path->atIndex(ix)->getIndex())/2] = subFlowPrime[ix];
      }
      
      for (int ix = 0; ix < flow->getSize(); ix++) {
        if (flow->atIndex(ix) == auxGraph->edgeAtIndex(ix*2)->capacity) {
          auxGraph->edgeAtIndex(ix*2)->blocked = true;
        } else {
          auxGraph->edgeAtIndex(ix*2)->blocked = false;
        }
        if (flow->atIndex(ix) == 0.0) {
          auxGraph->edgeAtIndex((ix*2)+1)->blocked = true;
        } else {
          auxGraph->edgeAtIndex((ix*2)+1)->blocked = false;
        }
      }
      
      delete subFlow;
      delete subFlowPrime;
      delete this->pathToTerminal;
      this->pathToTerminal = NULL;
    }
    
    deleteAuxNetwork(auxGraph);
    
    this->flow = flow;
  }
  
  template<class NodeType, class EdgeType>
  Collection::Array<double> *Network<NodeType,EdgeType>::getMaximumFlow() {
    
    if (this->flow == NULL) {
      maxFlow(this->getStart(), this->getTerminal());
    }
    
    return this->flow;
  }
  
}


#endif
