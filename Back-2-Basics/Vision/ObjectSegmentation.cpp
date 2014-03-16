//
//  ObjectSegmentation.cpp
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 3/2/14.
//  Copyright (c) 2014 Marcus Gilchrist. All rights reserved.
//

#include "ObjectSegmentation.h"


namespace Segmentation
{

double ObjectSegmentation::computeCredit(uint64_t size)
{
  // TODO
  return (double)size;
}


Segment *ObjectSegmentation::segmentUnion(Segment *region1, Segment *region2)
{
  for (uint64_t ix = 0; ix < region2->edges.size(); ix++)
  {
    Link *currentEdge = region2->edges.at(ix);
    regions.update(currentEdge->getForward(), region1);
    regions.update(currentEdge->getBackward(), region1);
    region1->edges.push_back(currentEdge);
  }
  
  region2->edges.resize(0);
  delete region2;
  
  return region1;
}


void ObjectSegmentation::edgeHeuristic(Link *edge)
{
  Segment *survivor;
  Segment *region1 = regions.get(edge->getBackward());
  Segment *region2 = regions.get(edge->getForward());
  
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

void ObjectSegmentation::segmentation(Graph<Pixel, Link> *g)
{
  vector<Link *> delayQueue;
  vector<Link *> *edges = g->getEdges(g->getReachableNodes(g->getStart(), NULL));
  LLRB_Tree<Link *, double> sorted;
  vector<Segment *> *segments;
  LLRB_TreeNode<Link *, double> *currentNode;
  
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
    Segment *r = segments->at(ix);
    if (r->mark == true)
    {
      continue;
    }
    r->credit = computeCredit(r->edges.size());
    
    r->mark = true;
  }
  
  for (uint64_t ix = 0; ix < segments->size(); ix++)
  {
    Segment *r = segments->at(ix);
    r->mark = false;
  }
  
  for (uint64_t ix = 0; ix < edges->size(); ix++)
  {
    Link *edge = edges->at(ix);
    Segment *region1 = regions.get(edge->getBackward());
    Segment *region2 = regions.get(edge->getForward());
    
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
    Segment *r = segments->at(ix);
    r->mark = true;
  }
  
  edges->resize(0);
  
  for (uint64_t ix = 0; ix < delayQueue.size(); ix++)  //each edge 2 tile.delayQ do
  {
    edgeHeuristic(delayQueue[ix]);
  }
  
}


vector<vector<Pixel *> *> *ObjectSegmentation::getNodeSegments(Graph<Pixel, Link> *g)
{
  vector<vector<Pixel *> *> *nodeRegions = new vector<vector<Pixel *> *>();
  LLRB_Tree<Pixel *, uint64_t> unique;
  std::vector<Segment *> *segments;
  
  segmentation(g);
  
  segments = regions.getElements();
  
  for (uint64_t ix = 0; ix < segments->size(); ix++)
  {
    Segment *r = segments->at(ix);
    
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

vector<vector<Link *> *> *ObjectSegmentation::getEdgeSegments(Graph<Pixel, Link> *g)
{
  std::vector<std::vector<Link *> *> *edgeRegions = new std::vector<std::vector<Link *> *>();
  std::vector<Link *> *edgeR;
  std::vector<Segment *> *segments;
  
  segmentation(g);
  
  segments = regions.getElements();
  
  for (uint64_t ix = 0; ix < segments->size(); ix++)
  {
    edgeR = new std::vector<Link *>();
    Segment *r = segments->at(ix);
    
    for (uint64_t ix = 0; ix < r->edges.size(); ix++)
    {
      edgeR->push_back(r->edges.at(ix));
    }
    
    edgeRegions->push_back(edgeR);
  }
  
  return edgeRegions;
}


}