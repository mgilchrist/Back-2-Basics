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
using namespace Tree;
#include "Heap.h"
using namespace Collection;
#include "Optimization.h"

namespace Optimization {
  
  /*  typedef struct TotalCompetition {
   int64_t competition = 0.0;
   uint64_t numCompetitors = 0;
   } TotalCompetition;
   */
  
  class Stoichastic : public Optimization {
    
  private:
    
    uint64_t iteration = 0;
    
  protected:
    
    //  LLRB_Tree<TotalCompetition *, uint64_t> *competitionInArea;
    LLRB_Tree<Trust<int64_t> *, uint64_t> active;
    
    static uint64_t calcConsensus(LLRB_TreeNode<Trust<int64_t> *,uint64_t> *current, void *reserved) {
      
      vector<int64_t *> *expectations = current->data->prediction->predictions.select(NULL, NULL);
      int64_t sum = 0.0, average = 0.0, diff = 0.0;
      uint64_t expectationCnt = 0;
      
      for (uint64_t ix = 0; ix < expectations->size(); ix++) {
        int64_t expect = *(expectations->at(ix));
        if (expect) {
          sum += expect;
          expectationCnt++;
        }
      }
      
      if (expectationCnt) {
        average = sum / (int64_t)expectationCnt;
      }
      
      for (uint64_t ix = 0; ix < expectations->size(); ix++) {
        int64_t expect = *(expectations->at(ix));
        if (expect) {
          int64_t tmp = expect-average;
          diff += pow(tmp,2);
        }
      }
      
      current->data->prediction->expectation = average;
      current->data->prediction->confidence = 0.0;
      
      if (expectationCnt == 1)
      {
        current->data->prediction->confidence = 0.99;
      }
      else
      {
        int64_t variance = sqrt(diff / int64_t(expectationCnt-1));
        current->data->prediction->confidence = 1.0 / (1.0 + (variance / sqrt(expectationCnt)));
      }
      
      return current->key;
    }
    
    static uint64_t doEpochEach(LLRB_TreeNode<Heuristic<> *,uint64_t> *current, void *iteration)
    {
      current->data->calcExpectation(*(uint64_t *)iteration);
      
      return current->key;
    }
    
    void doEpoch();
    virtual void doGeneration() =0;
    
    vector<int64_t *> *pickRandoms(LLRB_Tree<int64_t *, uint64_t> *, vector<int64_t *> *originals, uint64_t);
    vector<Trust<int64_t> *> *pickRandomTrusts(LLRB_Tree<Trust<int64_t> *, uint64_t> *, vector<Trust<int64_t> *> *originals, uint64_t);
    Heuristic<> *spawn(uint32_t, uint32_t);
    
  public:
    Stoichastic()
    {
      
    }
    
    ~Stoichastic()
    {
      
    }
    
    void initInternals();
  };
  
  
  
  
}

#endif
