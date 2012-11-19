//
//  Stochastic.h
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

#ifndef OpenSource_Stochastic_h
#define OpenSource_Stochastic_h

#include "LLRB_Tree.h"
using namespace Graph;
#include "Heap.h"
using namespace Collection;
#include "Optimization.h"

namespace Optimization {
  
  
  
  typedef struct TotalCompetition {
    double competition;
    uint64_t numCompetitors;
  } TotalCompetition;
  
  template <class HeuristicType, class DataType>
  class Stoichastic : public Optimization<HeuristicType,DataType> {
    
  private:
    
    LLRB_Tree<TotalCompetition *, uint64_t> *competitionInArea;
    uint64_t iteration = 0;
    
  protected:
    
    static uint64_t doEpochEach(LLRB_TreeNode<HeuristicType *,uint64_t> *current, void *iteration) {
      
      current->data->calcExpectation(*(uint64_t *)iteration);
      
      return current->key;
    }
    
    void doEpoch();
    
  public:
    Stoichastic() {
      competitionInArea = new LLRB_Tree<TotalCompetition *, uint64_t>();
    }
  };
  
  
  template <class HeuristicType, class DataType>
  void Stoichastic<HeuristicType,DataType>::doEpoch() {
    iteration++;
    this->candidates->modifyAll(doEpochEach, &iteration);
  }
  
}

#endif
