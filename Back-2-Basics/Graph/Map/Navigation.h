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

#include "Stack.h"
#include "Map.h"
#include "Heuristic.h"


namespace Graph {
  
  
  class Path;
  
  class Coordinate;
  
  class Path : public Via<Coordinate>
  {
  public:
    Path();
    Path(Coordinate *v, Coordinate *u, double length);
  };
  
  
  class Coordinate : public Location<Coordinate,Path>
  {
    
  private:
    
    Coordinate *origin;
    
    double x;
    double y;
    double z;
    
  public:
    
    Coordinate();
    
  };
  
  
  class Navigation : public Map<Coordinate,Path>
  {
    
  private:
    void aStar();
    Collection::Array<double> *costHeuristic;
    
  public:
    
    Navigation();
    
    void moveTo(uint64_t neighbor);
    void changeOrigin();
    void getOriginCooridinate(double *,double *,double *);
    void addSubMap(Map<Coordinate,Path> *map, Coordinate *origin, Heuristic *);
    void removeSubMap(uint64_t);
    Collection::Stack<Path *> *getShortestPath();

    
    
  };
}

#endif /* defined(__Back_2_Basics__Navigation__) */
