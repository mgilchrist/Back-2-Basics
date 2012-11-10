//
//  Navigation.h
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 9/26/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#ifndef __Back_2_Basics__Navigation__
#define __Back_2_Basics__Navigation__

#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "Stack.h"
#include "HashTable.h"
#include "Map.h"
#include "Heuristic.h"


namespace Graph {
  
  class Path;
  
  class Coordinate;
  class Vector;
  
  typedef struct AStarStorage {
    Collection::Heap<Coordinate *,double> *open;
    Collection::HashTable<bool,Coordinate *> *openTable;
    Collection::HashTable<double,Coordinate *> *closed;
  } AStarStorage;
  
  class Path : public Via<Coordinate,Path>
  {
  public:
    Path();
    Path(Coordinate *v, Coordinate *u, double length);
  };
  
  
  class Coordinate : public Location<Coordinate,Path>
  {
  public:
    double X;
    double Y;
    double Z;
    
    double *heuritic;
    
    Coordinate();
    Coordinate(double X, double Y, double Z);
  };
  
  class Vector : public Location<Coordinate,Path>
  {
    
  private:
    
    Vector *origin;
    
  public:
    
    double X;
    double Y;
    double Z;
    double deltaX;
    double deltaY;
    double deltaZ;
    
    Vector() {
      origin = NULL;
      X = 0.0;
      Y = 0.0;
      Z = 0.0;
      deltaX = 0.0;
      deltaY = 0.0;
      deltaZ = 0.0;
    }
    
    void updateOrgin(Vector *origin) {
      if (this->origin == NULL) {
        return;
      }
      
      double deltaAlpha;
      double deltaBeta;
      double deltaGamma;
      
      deltaAlpha = tanh(((origin->deltaX)-(deltaX)) / deltaX);
      deltaBeta = tanh(((origin->deltaY)-(deltaY)) / deltaY);
      deltaGamma = tanh(((origin->deltaZ)-(deltaZ)) / deltaZ);
      
      
      X += (X - origin->X) * deltaAlpha;
      Y += (Y - origin->Y) * deltaBeta;
      Z += (Z - origin->Z) * deltaGamma;
      deltaX += (deltaX - origin->deltaX) * deltaAlpha;
      deltaY += (deltaY - origin->deltaY) * deltaBeta;
      deltaZ += (deltaZ - origin->deltaZ) * deltaGamma;
      
    }
    
  };
  
  
  class HeuristicMap : public Map<Coordinate, Path>
  {
  protected:
    const char *name;
    Vector *origin;
    std::vector<double> *costHeuristic;
    
    static uint64_t aStarGambit(LLRB_TreeNode<Path *, uint64_t> *current, void *storage) {
      
      Path *uv = current->data;
      bool tmpBool;
      double tmpDouble;
      Collection::Heap<Coordinate *,double> *open = ((AStarStorage *)storage)->open;
      Collection::HashTable<bool,Coordinate *> *openTable = ((AStarStorage *)storage)->openTable;
      Collection::HashTable<double,Coordinate *> *closed = ((AStarStorage *)storage)->closed;
      
      if (!uv->blocked) {
        Coordinate *u = uv->getBackward();
        Coordinate *v = uv->getForward();
        double cost = u->distanceFromStart + uv->length;
        
        if (cost < v->distanceFromStart) {
          if (!openTable->get(u, &tmpBool) && (tmpBool)) {
            openTable->update(false, v, &tmpBool);
          }
          
          closed->remove(v, &tmpDouble);
        }
        
        if ((openTable->get(u, &tmpBool) || (!tmpBool)) &&
            ((!closed->get(v, &tmpDouble)) && (tmpDouble != 0.0))) {
          
          v->distanceFromStart = cost;
          
          if (!openTable->update(true, v, &tmpBool) && (!tmpBool)) {
            open->remove(*(v->auxIndex));
            v->auxIndex = NULL;
          }
          
          /* TODO */
          v->auxIndex = open->push(v, cost + *v->heuritic);
          v->previousEdge = uv;
          
        }
      }
      
      return current->key;
    }
    
    void aStar();
    
  public:
    HeuristicMap() {
      
    }
    
    static uint64_t calcPathLengthEach(LLRB_TreeNode<Path *, uint64_t> *current, void *reserved) {
      Path *path = current->data;
      
      double deltaX = path->getForward()->X - path->getBackward()->X;
      double deltaY = path->getForward()->Y - path->getBackward()->Y;
      double deltaZ = path->getForward()->Z - path->getBackward()->Z;
      
      path->length = sqrt((deltaX * deltaX) +
                          (deltaY * deltaY) +
                          (deltaZ * deltaZ));
      
      return current->key;
    }
    
    void updateOrgin(Vector *origin) {
      if ((this->origin == NULL) ||
          (!bcmp(this->origin, origin, sizeof(Vector)))) {
        this->origin = origin;
        return;
      }
      
      vector<Coordinate *> *nodes;
      double X, Y, Z;
      double deltaAlpha;
      double deltaBeta;
      double deltaGamma;
      
      deltaAlpha = tanh((origin->deltaX - this->origin->deltaX) / this->origin->deltaX);
      deltaBeta = tanh((origin->deltaY - this->origin->deltaY) / this->origin->deltaY);
      deltaGamma = tanh((origin->deltaZ - this->origin->deltaZ) / this->origin->deltaZ);
      
      
      X = (this->origin->X - origin->X) * deltaAlpha;
      Y = (this->origin->Y - origin->Y) * deltaBeta;
      Z = (this->origin->Z - origin->Z) * deltaGamma;
      
      this->origin->X += X;
      this->origin->Y += Y;
      this->origin->Z += Z;
      
      nodes = this->getReachableNodes(this->start, this->terminal);
      
      for (int ix = 0; ix < nodes->size(); ix ++) {
        nodes->at(ix)->X += X;
        nodes->at(ix)->Y += Y;
        nodes->at(ix)->Z += Z;
      }
      
      for (int ix = 0; ix < nodes->size(); ix ++) {
        nodes->at(ix)->modifyAllAdjacent(HeuristicMap::calcPathLengthEach, 0);
      }
      
    }
    
    vector<Path *> *getShortestPath();
    
    
  };
  
  
  class Navigation : public Map<Coordinate, Path>
  {
    
  private:
    
    
    Collection::HashTable<HeuristicMap *,std::string> *maps;
    Vector *origin;
    
    Coordinate *start;
    Coordinate *terminal;
    
  public:
    
    Navigation() {
      maps = new Collection::HashTable<HeuristicMap *,std::string>();
      origin = new Vector();
      origin->X = 0.0;
      origin->Y = 0.0;
      origin->Z = 0.0;
      
      origin->deltaX = 0.0;
      origin->deltaY = 0.0;
      origin->deltaZ = 1.0;
    }
    
    void moveTo(uint64_t neighbor);
    void changeOrigin(Vector);
    Vector getOriginCooridinate();
    void addMap(HeuristicMap *map);
    void removeMap(const char *);
    
  };
}

#endif /* defined(__Back_2_Basics__Navigation__) */
