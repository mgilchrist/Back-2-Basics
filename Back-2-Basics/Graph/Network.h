//
//  Network.h
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

#ifndef OpenSource_Network_h
#define OpenSource_Network_h

#include <vector>
#include "Graph.h"

namespace Graph {
  
  class SimplePipe;
  class SimpleHub;
  
  template <class HubType, class PipeType>
  class Pipe : public Edge<HubType,PipeType>
  {
    
  public:
    Pipe<HubType,PipeType>();
    Pipe<HubType,PipeType>(HubType *v, HubType *u, double capacity);
    
    inline double capacity() {
      return this->attrib;
    }
  };
  
  
  template <class NodeType, class EdgeType>
  class Hub : public Node<NodeType,EdgeType>
  {
    
  public:
    double capacity = 0.0;
    
  public:
    
    Hub();
  };
  
  
  class SimplePipe : public Pipe<SimpleHub, SimplePipe> {};
  class SimpleHub : public Hub<SimpleHub, SimplePipe> {};
  
  /* Pipe */
  
  template <class HubType, class PipeType>
  Pipe<HubType,PipeType>::Pipe() {
    
  }
  
  template <class HubType, class PipeType>
  Pipe<HubType,PipeType>::Pipe(HubType *v, HubType *u, double capacity) {
    this->u = u;
    this->v = v;
    
    v->references++;
    u->addEdge((PipeType *)this);
    
    this->attrib = capacity;
  }
  
  
  /* Hub */
  
  template <class NodeType, class EdgeType>
  Hub<NodeType,EdgeType>::Hub() {
  
  }
  
  
  /* Network */
  
  template <class NodeType, class EdgeType>
  class Network : public Graph<NodeType, EdgeType>
  {
    std::vector<double> *flow = NULL;
    
  protected:
    vector<Pipe<NodeType,EdgeType> *> *createAuxNetwork(vector<Pipe<NodeType,EdgeType> *> *);
    void deleteAuxNetwork(vector<Pipe<NodeType,EdgeType> *> *auxEdges);
    double bottleneck(double *, vector<Pipe<NodeType,EdgeType> *> *);
    double *augment(double *, vector<Pipe<NodeType,EdgeType> *> *,vector<Pipe<NodeType,EdgeType> *> *);
    void maxFlow(NodeType *, NodeType *);
    
  public:
    Network();
    ~Network();
    
    virtual std::vector<double> *getMaximumFlow();
  };
  
  template <class NodeType, class EdgeType>
  Network<NodeType,EdgeType>::Network() {
    
  }
  
  template <class NodeType, class EdgeType>
  Network<NodeType,EdgeType>::~Network() {
    if (flow != NULL) {
      delete flow;
    }
  }
  
  
  /* Network Flow */
  template <class NodeType, class EdgeType>
  vector<Pipe<NodeType,EdgeType> *> *Network<NodeType,EdgeType>::createAuxNetwork(vector<Pipe<NodeType,EdgeType> *> *edges) {
    vector<Pipe<NodeType,EdgeType> *> *auxEdges = new vector<Pipe<NodeType,EdgeType> *>();
    Pipe<NodeType,EdgeType> *thisEdge;
    
    auxEdges->resize(edges->size());
    
    for (uint64_t ix = 0; ix < edges->size(); ix++) {
      thisEdge = edges->at(ix);
      
      thisEdge = new Pipe<NodeType,EdgeType>(thisEdge->getBackward(), thisEdge->getForward(),
                              thisEdge->capacity());
      thisEdge->blocked = true;
      
      auxEdges->at(ix) = thisEdge;
    }
    
    return auxEdges;
  }
  
  template <class NodeType, class EdgeType>
  void Network<NodeType,EdgeType>::deleteAuxNetwork(vector<Pipe<NodeType,EdgeType> *> *auxEdges) {
    Pipe<NodeType,EdgeType> *thisEdge;
    
    for (uint64_t ix = 0; ix < auxEdges->size(); ix++) {
      delete auxEdges->at(ix);
    }
    
    delete auxEdges;
  }
  
  template <class NodeType, class EdgeType>
  double Network<NodeType,EdgeType>::bottleneck(double *flow, vector<Pipe<NodeType,EdgeType> *> *path) {
    double bottleneck = path->at(0)->capacity() - flow[0];
    
    for (int ix = 1; ix < path->size(); ix++) {
      if (bottleneck > (path->at(ix)->capacity() - flow[ix])) {
        bottleneck = (path->at(ix)->capacity() - flow[ix]);
      }
    }
    
    return bottleneck;
  }
  
  template <class NodeType, class EdgeType>
  double *Network<NodeType,EdgeType>::augment(double *flow,
                                              vector<Pipe<NodeType,EdgeType> *> *path,
                                              vector<Pipe<NodeType,EdgeType> *> *edges) {
    
    double *fPrime = new double[path->size()];
    double b = bottleneck(flow, path);
    
    for (int ix = 0; ix < path->size(); ix++) {
      bool isAug = (((path->at(ix)) >= edges->at(0)) && ((path->at(ix)) < edges->at(edges->size()))) ? false : true;
      if (isAug) {
        fPrime[ix] = flow[ix] - b;
      } else {
        fPrime[ix] = flow[ix] + b;
      }
    }
    
    return fPrime;
  }
  
  template<class NodeType, class EdgeType>
  void Network<NodeType,EdgeType>::maxFlow(NodeType *s, NodeType *t) {
    
    vector<Pipe<NodeType,EdgeType> *> *path;
    std::vector<double> *flow;
    double *subFlow, *subFlowPrime;
    std::vector<NodeType *> *nodes = this->getReachableNodes(s,t);
    
    std::vector<Pipe<NodeType,EdgeType> *> *edges = (vector<Pipe<NodeType,EdgeType> *> *)this->getEdges(nodes);
    
    std::vector<Pipe<NodeType,EdgeType> *> *auxEdges = createAuxNetwork(edges);
    
    flow = new std::vector<double>();
    
    flow->resize(nodes->size(),0.0);
    
    while ((path = (vector<Pipe<NodeType,EdgeType> *> *)this->findAPath(s, t)) != NULL) {
      subFlow = new double[path->size()];
      
      for (int ix = 0; ix < path->size(); ix++) {
        uint64_t index = ((((path->at(ix)) >= edges->at(0)) && ((path->at(ix)) < edges->at(edges->size()))) ?
                          (((path->at(ix))-edges->at(0))/sizeof(EdgeType *)) :
                          (((path->at(ix))-auxEdges->at(0)))/sizeof(EdgeType *));
        subFlow[ix] = flow->at(index);
      }
      
      subFlowPrime = this->augment(subFlow, path, edges);
      
      for (int ix = 0; ix < path->size(); ix++) {
        uint64_t index = ((((path->at(ix)) >= edges->at(0)) && ((path->at(ix)) < edges->at(edges->size()))) ?
                          (((path->at(ix))-edges->at(0))/sizeof(EdgeType *)) :
                          (((path->at(ix))-auxEdges->at(0)))/sizeof(EdgeType *));
        flow->at(index) = subFlowPrime[ix];
      }
      
      for (int ix = 0; ix < flow->size(); ix++) {
        if (flow->at(ix) == edges->at(ix)->capacity()) {
          edges->at(ix)->blocked = true;
        } else {
          edges->at(ix)->blocked = false;
        }
        if (flow->at(ix) == 0.0) {
          auxEdges->at(ix)->blocked = true;
        } else {
          auxEdges->at(ix)->blocked = false;
        }
      }
      
      delete subFlow;
      delete subFlowPrime;
    }
    
    delete nodes;
    delete edges;
    
    this->flow = flow;
  }
  
  template<class NodeType, class EdgeType>
  std::vector<double> *Network<NodeType,EdgeType>::getMaximumFlow() {
    
    if (this->flow == NULL) {
      maxFlow(this->getStart(), this->getTerminal());
    }
    
    return this->flow;
  }
  
}


#endif
