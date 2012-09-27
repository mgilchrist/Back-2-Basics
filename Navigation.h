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
  
public:
  
  Navigation();
  
};
}

#endif /* defined(__Back_2_Basics__Navigation__) */
