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

#define LOGIC_TYPE NeuralNetwork::Neuron


namespace Optimization {
  
  class Optimization {
    
  private:
    typedef struct RandSection {
      uint64_t begin;
      uint64_t requests;
      int64_t probability;
    } URandom;
    
  protected:
    
    virtual void doEpoch() =0;
    uint32_t spawnHiddenWidth;
    
    vector<int64_t *> *questionCache = NULL;
    vector<Trust<int64_t> *> *answerCache = NULL;
    
  public:
    
    int64_t error_rate = 0;
    
    LLRB_Tree<int64_t *, uint64_t> question;
    LLRB_Tree<Trust<int64_t> *, uint64_t> answer;
    
    
  public:
    
    LLRB_Tree<Heuristic<> *, uint64_t> candidates;
    
    Optimization();
    
    ~Optimization() {
      
    }
    
    void addInput(int64_t *);
    void addOutput(int64_t *);
    
    void addInput(vector<int64_t *> *);
    void addOutput(vector<int64_t *> *);
    
    virtual void initInternals() =0;
    virtual Heuristic<> *createNewHeuristic(vector<int64_t *> *input,
                                          vector<Trust<int64_t> *> *output,
                                          LLRB_Tree<int64_t *, uint64_t> *expectation,
                                          vector<Info *> *layers,
                                          uint32_t maxHiddenWidth) =0;
    void optimizeAnwser();
    
  };
  
  
  
  
}

#endif
