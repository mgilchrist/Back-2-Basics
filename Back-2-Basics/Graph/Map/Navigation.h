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
    Heap<Coordinate *,int64_t> *open;
    HashTable<bool *,Coordinate *> *openTable;
    HashTable<uint64_t *,Coordinate *> *closed;
  } AStarStorage;
  
  class Path : public Via<Coordinate,Path>
  {
  public:
    Path();
    Path(Coordinate *v, Coordinate *u, int64_t length);
  };
  
  
  class Coordinate : public Location<Coordinate,Path>
  {
  public:
    int64_t X;
    int64_t Y;
    int64_t Z;
    
    int64_t *heuritic;
    
    Coordinate();
    Coordinate(int64_t X, int64_t Y, int64_t Z);
  };
  
  class Vector : public Location<Coordinate,Path>
  {
    
  private:
    
    Vector *origin;
    
  public:
    
    int64_t X;
    int64_t Y;
    int64_t Z;
    int64_t deltaX;
    int64_t deltaY;
    int64_t deltaZ;
    
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
      
      int64_t deltaAlpha;
      int64_t deltaBeta;
      int64_t deltaGamma;
      
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
    std::vector<int64_t> *costHeuristic;
    
    static uint64_t aStarGambit(LLRB_TreeNode<Path *, uint64_t> *current, void *storage) {
      
      Path *uv = current->data;
      bool tmpTrue, tmpFalse;
      bool *available;
      Heap<Coordinate *,int64_t> *open = ((AStarStorage *)storage)->open;
      HashTable<bool *,Coordinate *> *openTable = ((AStarStorage *)storage)->openTable;
      HashTable<uint64_t *,Coordinate *> *closed = ((AStarStorage *)storage)->closed;
      
      if (!uv->blocked) {
        Coordinate *u = uv->getBackward();
        Coordinate *v = uv->getForward();
        int64_t cost = u->distanceFromStart + uv->length();
        
        if (cost < v->distanceFromStart) {
          available = openTable->get(u);
          
          if (available != NULL) {
            openTable->update(v, &tmpFalse);
          }
          
          closed->remove(v);
        }
        
        if (!(openTable->get(u)) && (!closed->get(v))) {
          
          v->distanceFromStart = cost;
          
          if (!openTable->get(v)) {
            openTable->update(v, &tmpTrue);
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
      
      int64_t deltaX = path->getForward()->X - path->getBackward()->X;
      int64_t deltaY = path->getForward()->Y - path->getBackward()->Y;
      int64_t deltaZ = path->getForward()->Z - path->getBackward()->Z;
      
      path->setLength( sqrt((deltaX * deltaX) +
                          (deltaY * deltaY) +
                          (deltaZ * deltaZ)) );
      
      return current->key;
    }
    
    void updateOrgin(Vector *origin) {
      if ((this->origin == NULL) ||
          (!bcmp(this->origin, origin, sizeof(Vector)))) {
        this->origin = origin;
        return;
      }
      
      vector<Coordinate *> *nodes;
      int64_t X, Y, Z;
      int64_t deltaAlpha;
      int64_t deltaBeta;
      int64_t deltaGamma;
      
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
    
    
    HashTable<HeuristicMap *,std::string> *maps;
    Vector *origin;
    
    Coordinate *start;
    Coordinate *terminal;
    
  public:
    
    Navigation() {
      maps = new HashTable<HeuristicMap *,std::string>();
      origin = new Vector();
      origin->X = 0.0;
      origin->Y = 0.0;
      origin->Z = 0.0;
      
      origin->deltaX = 0.0;
      origin->deltaY = 0.0;
      origin->deltaZ = 1.0;
    }
    
    static int64_t calcDistance(int64_t X, int64_t Y, int64_t Z, int64_t tX, int64_t tY, int64_t tZ) {
      return sqrt(pow(X-tX,2)+pow(Y-tY,2)+pow(Z-tZ,2));
    }
    
    void moveTo(uint64_t neighbor);
    void changeOrigin(Vector);
    Vector getOriginCooridinate();
    void addMap(HeuristicMap *map);
    void removeMap(const char *);
    
  };
}

#endif /* defined(__Back_2_Basics__Navigation__) */
