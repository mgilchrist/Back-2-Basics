//
//  Optimization.h
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

#ifndef OpenSource_Optimization_h
#define OpenSource_Optimization_h

#include "LLRB_Tree.h"
#include "Heuristic.h"


template <class HeuristicType, class DataType>
class Optimization {
  
private:
  
  DataType *space;
  uint64_t spaceSize;
  
public:
  Optimization();
  Optimization(DataType *, uint64_t);
  
  void add(HeuristicType *) =0;
  HeuristicType *get() =0;
  
};

template <class HeuristicType, class DataType>
Optimization<HeuristicType,DataType>::Optimization() {
  space = NULL;
}

template <class HeuristicType, class DataType>
Optimization<HeuristicType,DataType>::Optimization(DataType *space, uint64_t spaceSize) {
  this->space = space;
  this->spaceSize = spaceSize;
}


#endif
