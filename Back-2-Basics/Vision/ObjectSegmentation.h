//
//  ObjectSegmentation.h
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 2/24/13.
//  Copyright (c) 2013 Marcus Gilchrist. All rights reserved.
//

#ifndef Back_2_Basics_ObjectSegmentation_h
#define Back_2_Basics_ObjectSegmentation_h

#include <algorithm>    // std::min
#include <vector>
using namespace std;
#include "Graph.h"
#include "HashTable.h"

namespace Segmentation {
  
  template <class EdgeType>
  struct Segment
  {
    vector<EdgeType *> edges;
    double credit;
    bool mark = false;
    bool isValid = false;
  };
  
  /* Class: ObjectSegmentation */
  template <class NodeType, class EdgeType>
  class ObjectSegmentation
  {
  private:
    
    HashTable<Segment<EdgeType> *,NodeType *> regions;
    
    vector<vector<NodeType *> *> *getEntangledNeuronGroups();
    
    void edgeHeuristic(EdgeType *edge);
    void segmentation(Graph<NodeType, EdgeType> *g);
    double computeCredit(uint64_t);
    
    Segment<EdgeType> *segmentUnion(Segment<EdgeType> *region1, Segment<EdgeType> *region2);
  
  public:
    vector<vector<EdgeType *> *> *getEdgeSegments(Graph<NodeType, EdgeType> *g);
    vector<vector<NodeType *> *> *getNodeSegments(Graph<NodeType, EdgeType> *g);
    
    ObjectSegmentation() {
      
    }
  
  };
  
  template<class NodeType, class EdgeType>
  double ObjectSegmentation<NodeType,EdgeType>::computeCredit(uint64_t size)
  {
    // TODO
    return (double)size;
  }
  
  template <class NodeType, class EdgeType>
  Segment<EdgeType> *ObjectSegmentation<NodeType,EdgeType>::segmentUnion(Segment<EdgeType> *region1, Segment<EdgeType> *region2)
  {
    for (uint64_t ix = 0; ix < region2->edges.size(); ix++)
    {
      EdgeType *currentEdge = region2->edges.at(ix);
      regions.update(currentEdge->getForward(), region1);
      regions.update(currentEdge->getBackward(), region1);
      region1->edges.push_back(currentEdge);
    }
    
    region2->edges.resize(0);
    delete region2;
    
    return region1;
  }
  
  template <class NodeType, class EdgeType>
  void ObjectSegmentation<NodeType,EdgeType>::edgeHeuristic(EdgeType *edge)
  {
    Segment<EdgeType> *survivor;
    Segment<EdgeType> *region1 = regions.get(edge->getBackward());
    Segment<EdgeType> *region2 = regions.get(edge->getForward());
    
    assert(region1 != NULL);
    assert(region2 != NULL);
    
    if (region1 != region2)
    {
      double credit = std::min(region1->credit, region2->credit);
      if (credit > edge->getAttribute())
      {
        survivor = segmentUnion(region1, region2);
        survivor->credit = credit - edge->getAttribute();
      }
    }
  }
  
  template <class NodeType, class EdgeType>
  void ObjectSegmentation<NodeType,EdgeType>::segmentation(Graph<NodeType, EdgeType> *g)
  {
    vector<EdgeType *> delayQueue;
    vector<EdgeType *> *edges = g->getEdges(g->getReachableNodes(g->getStart(), NULL));
    LLRB_Tree<EdgeType *, double> sorted;
    vector<Segment<EdgeType> *> *segments;
    LLRB_TreeNode<EdgeType *, double> *currentNode;
    
    // sort edges, merging those with weight < minWeight;
    for (uint64_t ix = 0; ix < edges->size(); ix++)
    {
      sorted.insert(edges->at(ix), edges->at(ix)->getAttribute());
    }
    
    edges->resize(0);
    
    currentNode = sorted.min(sorted.treeRoot);
    
    while (sorted.next(currentNode) != NULL)
    {
      edges->push_back(currentNode->data);
      currentNode = sorted.next(currentNode);
    }
    
    sorted.removal(NULL, NULL);
    
    segments = regions.getElements();
    
    for (uint64_t ix = 0; ix < regions.getSize(); ix++)
    {
      Segment<EdgeType> *r = segments->at(ix);
      if (r->mark == true)
      {
        continue;
      }
      r->credit = computeCredit(r->edges.size());
      
      r->mark = true;
    }
    
    for (uint64_t ix = 0; ix < segments->size(); ix++)
    {
      Segment<EdgeType> *r = segments->at(ix);
      r->mark = false;
    }

    for (uint64_t ix = 0; ix < edges->size(); ix++)
    {
      EdgeType *edge = edges->at(ix);
      Segment<EdgeType> *region1 = regions.get(edge->getBackward());
      Segment<EdgeType> *region2 = regions.get(edge->getForward());
      
      if (region1 != region2)
      {
        region1->mark = true;
        region2->mark = true;
      }
      else if (region1->mark && region2->mark)
      {
        delayQueue.push_back(edge);
      }
      else
      {
        edgeHeuristic(edge);
      }
    }
    
    for (uint64_t ix = 0; ix < segments->size(); ix++)
    {
      Segment<EdgeType> *r = segments->at(ix);
      r->mark = true;
    }
    
    edges->resize(0);
    
    for (uint64_t ix = 0; ix < delayQueue.size(); ix++)  //each edge 2 tile.delayQ do
    {
      edgeHeuristic(delayQueue[ix]);
    }
    
  }
  
  template <class NodeType, class EdgeType>
  vector<vector<NodeType *> *> *ObjectSegmentation<NodeType,EdgeType>::getNodeSegments(Graph<NodeType, EdgeType> *g)
  {
    vector<vector<NodeType *> *> *nodeRegions = new vector<vector<NodeType *> *>();
    LLRB_Tree<NodeType *, uint64_t> unique;
    std::vector<Segment<EdgeType> *> *segments;

    segmentation(g);
    
    segments = regions.getElements();
    
    for (uint64_t ix = 0; ix < segments->size(); ix++)
    {
      Segment<EdgeType> *r = segments->at(ix);
      
      for (uint64_t ix = 0; ix < r->edges.size(); ix++)
      {
        unique.insert(r->edges.at(ix)->getForward(), (uint64_t)r->edges.at(ix)->getForward());
        unique.insert(r->edges.at(ix)->getBackward(), (uint64_t)r->edges.at(ix)->getBackward());
      }
      
      nodeRegions->push_back(unique.select(NULL, NULL));
      unique.removal(NULL, NULL);
    }

    return nodeRegions;
  }
  
  template <class NodeType, class EdgeType>
  vector<vector<EdgeType *> *> *ObjectSegmentation<NodeType,EdgeType>::getEdgeSegments(Graph<NodeType, EdgeType> *g)
  {
    std::vector<std::vector<NodeType *> *> *edgeRegions = new std::vector<std::vector<NodeType *> *>();
    std::vector<NodeType *> *edgeR;
    std::vector<Segment<EdgeType> *> *segments;

    segmentation(g);
    
    segments = regions->getElements();
    
    for (uint64_t ix = 0; ix < segments->size(); ix++)
    {
      edgeR = LLRB_Tree<NodeType *, double>();
      Segment<EdgeType> *r = segments->at(ix);
      
      for (uint64_t ix = 0; ix < r->edges.size(); ix++)
      {
        edgeR->push_back(r->edges->at(ix));
      }
      
      edgeRegions->push_back(edgeR);
    }
    
    return edgeRegions;
  }


}

#endif
