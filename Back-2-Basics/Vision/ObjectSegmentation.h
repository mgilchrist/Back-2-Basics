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
using namespace Graph;
#include "HashTable.h"
using namespace Collection;
#include "Network.h"

namespace Segmentation {
  
  class Link;
  class Pixel;
  
  class Link : public Edge<Pixel,Link>
  {
    
  };
  
  class Pixel : public Node<Pixel,Link>
  {
    
  };
  
  struct Segment
  {
    vector<Link *> edges;
    double credit;
    bool mark = false;
    bool isValid = false;
  };
  
  /* Class: ObjectSegmentation */
  class ObjectSegmentation
  {
  private:
    
    HashTable<Segment *,Pixel *> regions;
    
    vector<vector<Link *> *> *getEntangledNeuronGroups();
    
    void edgeHeuristic(Link *edge);
    void segmentation(Graph<Pixel, Link> *g);
    double computeCredit(uint64_t);
    
    Segment *segmentUnion(Segment *region1, Segment *region2);
  
  public:
    vector<vector<Link *> *> *getEdgeSegments(Graph<Pixel, Link> *g);
    vector<vector<Pixel *> *> *getNodeSegments(Graph<Pixel, Link> *g);
    
    ObjectSegmentation() {
      
    }
  
  };
  
}

#endif
