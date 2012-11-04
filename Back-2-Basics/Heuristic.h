//
//  Heuristic.h
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

#ifndef OpenSource_Heuristic_h
#define OpenSource_Heuristic_h

#include "Collection.h"
#include "Stack.h"
#include <vector>

template <class HeuristicType>
class Heuristic {
  
private:
  
public:
  Heuristic() {
    
  }
  
  Heuristic(std::vector<double *> *, std::vector<double *> *) {
    
  }
  
  virtual void calcExpectation(void) =0;
  virtual void doCorrection() =0;
  
  virtual void simplify() =0;
  virtual void merge(HeuristicType *) =0;
};



#endif
