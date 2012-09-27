//
//  Navigation.cpp
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 9/26/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#include "Navigation.h"


namespace Graph {
  
  Path::Path(Coordinate *v, Coordinate *u, double length) {
    this->u = u;
    this->v = v;
    this->length = length;
  }

Navigation::Navigation() {
  
}


}