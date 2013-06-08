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
using std;
#include "Graph.h"
#inlcude "HashTable.h"

namespace Vision {
  
  
  /* Class: ObjectSegmentation */
  class ObjectSegmentation<NodeType, EdgeType>
  {
  private:
    struct Segment {
      vector<EdgeType *> edges;
      double credit;
      bool mark = FALSE;
      bool isValid = FALSE;
    }
    HashTable<Segment *,NodeType *> regions;
    
    Segment *segmentUnion(Segment *region1, Segment *region2);
    
    vector<vector<NodeType *> *> *getEntangledNeuronGroups();
    
    void edgeHeuristic();
    void segmentation(Graph *g);
  
  public:
    vector<NodeType *> *getSegments();
  
  }
  
  template <class NodeType, class EdgeType>
  Segment *ObjectSegmentation::segmentUnion(Segment *region1, Segment *region2)
  {
    for (ix = 0; ix < region2->edges.size(); ix++)
    {
      EdgeType *currentEdge = region2->edges.at(ix);
      regions.update(currentEdge, region1);
      region1->edges.push_back(currentEdge);
    }
    
    region2->edges.resize(0);
    delete region2;
    
    return region1;
  }
  
  template <class NodeType, class EdgeType>
  void ObjectSegmentation::edgeHeuristic(Edge *edge)
  {
    Segment *survivor;
    Segment *region1 = regions.search(edge->getBackward);
    Segment *region2 = regions.search(edge->getForward);
    
    assert(region1 != NULL);
    assert(region2 != NULL);
    
    if (region1 != region2)
    {
      credit = std::min(region1.credit, region2.credit);
      if (credit > edge.weight)
      {
        survivor = segmentUnion(region1, region2);
        survivor.credit = credit - edge.weight;
      }
    }
  }
  
  template <class NodeType, class EdgeType>
  void ObjectSegmentation::segmentation(Graph *g)
  {
    vector<EdgeType *> delayQueue;
    vector<EdgeType *> edges = g->getEdges(g->getReachableNodes(g->start, NULL));
    LLRB_Tree<EdgeType *, double> sorted;
    std::vector<Segment *> *segments;
    
    // sort edges, merging those with weight < minWeight;
    for (uint64_t ix = 0; ix < edges.size(); ix++)
    {
      sorted.insert(edges[ix], edges[ix]->attrib);
    }
    
    edges.resize(0);
    
    while (sorted.size > 0)
    {
      edges.push_back(sorted.removeMin(edges[ix], edges[ix]->attrib));
    }
    
    segments = regions->getElements();
    
    for (uint64_t ix = 0; ix < regions->size(); ix++)
    {
      r = segments->at(ix);
      if (r->mark == TRUE)
      {
        continue;
      }
      r->credit = ComputeCredit(r->nodes->size());
      
      r->mark = TRUE;
    }
    
    for (uint64_t ix = 0; ix < segments->size(); ix++)
    {
      r = segments->at(ix);
      r->mark = FALSE;
    }

    for (uint64_t ix = 0; ix < edges.size(); ix++)
    {
      EdgeType edge = edges[ix];
      region1 = regions.search(edge->getBackward());
      region2 = regions.search(edge->getForward());
      
      if (region1 != region2) {
        region1.mark = TRUE;
        region2.mark = TRUE;
      }
      else if (region1.mark && region2.mark)
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
      r = segments->at(ix);
      r->mark = FALSE;
    }
    
    edges.resize(0);
    
    for (uint64_t ix = 0; ix < delayQueue.size(); ix++)  //each edge 2 tile.delayQ do
    {
      edgeHeuristic(delayQueue[ix]);
    }
    
    edgeHeuristic.resize(0);
    
  }
  
  template <class NodeType, class EdgeType>
  vector<vector<NodeType *> *> *ObjectSegmentation::getNodeSegments(Graph *g)
  {
    vector<vector<NodeType *> *> *nodeRegions = new vector<vector<NodeType *> *>();
    LLRB_Tree<NodeType *, double> unique;
    segmentation(g);
    
    for (uint64_t ix = 0; ix < segments->size(); ix++)
    {
      r = segments->at(ix);
      
      for (uint64_t ix = 0; ix < edges.size(); ix++)
      {
        unique.insert(edges[ix], (uint64_t)edges[ix]);
      }
      
      nodeRegions->push_back(unique->select(NULL, NULL));
      unique->removal(NULL, NULL);
    }

    return nodeRegions;
  }
  
  template <class NodeType, class EdgeType>
  vector<vector<EdgeType *> *> *ObjectSegmentation::getEdgeSegments(Graph *g)
  {
    std::vector<std::vector<NodeType *> *> *edgeRegions = new ;
    LLRB_Tree<NodeType *, double> *edgeR;
    segmentation(g);
    
    for (uint64_t ix = 0; ix < segments->size(); ix++)
    {
      edgeR = LLRB_Tree<NodeType *, double>();
      r = segments->at(ix);
      
      for (uint64_t ix = 0; ix < edges.size(); ix++)
      {
        edgeR.push_back(edges[ix]);
      }
      
      nodeRegions->push_back(edgeR);
    }
    
    return edgeRegions;
  }

#if 0
  template <class NodeType, class EdgeType>
  ObjectSegmentation::relabeling()
  {
    std::vector<Segment *> *segments;
    double min, max;
    uint64_t numRegions = 0;
    std::vector<Nodetype *> *nodes = g->getReachableNodes(g->start, NULL)
    
    // compress regions
    for (uint64_t ix = 0; ix < segments->size(); ix++) { //each r in regions
      r = segments->at(ix);
      if ((r->size < max) && (r->size >= min)) {
        r->isValid = TRUE;
      } else {
        r->isValid = FALSE;
      }
    }
  
    for (uint64_t ix = 0; ix < nodes->size(); ix++) //each pixel do
    { 
      if (IsRepresentative(pixel) and regions.search(pixel).isValid)
      {
        numRegions++;
      }
    }
  
    for (uint64_t ix = 0; ix < nodes->size(); ix++) //each pixel do
    {
      if (IsRepresentative(pixel) && Find(pixel).isValid)
      {
        parents [pixel] = tile.startIndex;
        tile.startIndex = tile.startIndex + 1;
      }
    }
  }
#endif

  vector<vector<Neuron *> *> *ObjectSegmentation::getEntangledNeuronGroups() {
    
    LLRB_Tree<vector<Neuron *>, uint64_t> rootsFound;
    vector<Neuron *> *nodes = this->getReachableNodes(this->start,this->terminal);
    vector<Axion *> *edgesMaxList = maximumSpanningTree();
    double mean = 0.0, stdDev = 0.0, cutoff;
    
    /* Determine edge cutoff limit (one standard deviation below average) */
    
    for (uint64_t ix = 0; ix < edgesMaxList->size(); ix++) {
      mean += edgesMaxList->at(ix)->attrib;
    }
    
    mean /= edgesMaxList->size();
    
    for (uint64_t ix = 0; ix < edgesMaxList->size(); ix++) {
      stdDev += pow(mean - edgesMaxList->at(ix)->attrib, 2);
    }
    
    stdDev /= edgesMaxList->size();
    stdDev = sqrt(stdDev);
    
    cutoff = mean - stdDev;
    
    for (uint64_t ix = 0; ix < edgesMaxList->size(); ix++) {
      if (edgesMaxList->at(ix)->length() >= cutoff) {
        edgesMaxList->at(ix)->getForward()->previousEdge = edgesMaxList->at(ix);
      }
    }
    
    /* For each node find its root parent and store base on root */
    
    for (uint64_t ix = 0; ix < nodes->size(); ix++) {
      vector<NodeType *> *group;
      NodeType *current = nodes->at(ix);
      NodeType *previous = nodes->at(ix);
      EdgeType *memo;
      
      /* get root node */
      
      while (current->previousEdge != NULL) {
        memo = current->previousEdge;
        current = current->previousEdge->getBackward();
      }
      
      /* assign previous edges as edge connected to root to skip work */
      
      current = nodes->at(ix);
      while (current->previousEdge != NULL) {
        current->previousEdge = memo;
        current = current->previousEdge->getBackward();
      }
      
      if ((group = rootsFound.search((uint64_t)current)) == NULL) {
        group = new vector<NodeType *>();
        rootsFound.insert(group, (uint64_t)current);
      }
      
      group->push_back(nodes->at(ix));
    }
    
    /* Cleanup */
    
    for (uint64_t ix = 0; ix < edgesMaxList->size(); ix++) {
      edgesMaxList->at(ix)->getForward()->previousEdge = NULL;
    }
    
    nodes->resize(0);
    delete nodes;
    
    edgesMaxList->resize(0);
    delete edgesMaxList;
    
    return rootsFound.select(NULL, NULL);
  }
  
}

#endif
