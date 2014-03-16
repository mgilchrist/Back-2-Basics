//
//  Genetic.h
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

#ifndef OpenSource_Genetic_h
#define OpenSource_Genetic_h

#include "Stochastic.h"
#include "Heuristic.h"
#include "NeuralNetwork.h"

namespace Optimization {
  
  static int64_t glbAreaCapacity = 4;
  
  static uint64_t currentTime = -1;
  static const int64_t glbToughness = 0.0;
  static const uint64_t ofAge = 1;
  static const uint64_t maxAge = 100000;
  
  
  class Genetic : public Stoichastic
  {
    
  protected:
    
    static bool checkSurvivalEach(LLRB_TreeNode<Heuristic<> *,uint64_t> *current, void *world) {
      Heuristic<> *candidate = current->data;
      LLRB_Tree<Trust<int64_t> *, uint64_t> *cia = &(((Genetic *)world)->active);
      vector<Harmony<> *> *harmony = candidate->getHarmony();
      
      
      if (!candidate->registered)
      {
        if (candidate->experiencedEpochs > (pow(candidate->hiddenWidth, 2) * maxAge))
        {
          return true;
        }
        return false;
      }
      
      for (uint64_t ix = 0; ix < harmony->size(); ix++) {
        Trust<int64_t> *localComp = cia->search((uint64_t)harmony->at(ix)->reality);
        
        if (harmony->at(ix) != NULL)
        {
          if (harmony->at(ix)->logicElement == NULL)
          {
            Trust<int64_t> *localComp = cia->search((uint64_t)harmony->at(ix)->reality);
            localComp->prediction->predictions.remove(harmony->at(ix)->expectation, (uint64_t)harmony->at(ix)->expectation);
            delete harmony->at(ix)->expectation;
            harmony->at(ix)->expectation = NULL;
            
            if (!localComp->prediction->predictions.size())
            {
              cia->remove(localComp, (uint64_t)harmony->at(ix)->reality);
              delete localComp->prediction;
              localComp->prediction = NULL;
            }
            
            delete harmony->at(ix);
            harmony->at(ix) = NULL;
            continue;
          }
        }
        else
        {
          continue;
        }
        
        if (localComp == NULL)
        {
          continue;
        }
        uint64_t envSize = localComp->prediction->predictions.size();
        int64_t toughness = (localComp->prediction->confidence * envSize) - glbAreaCapacity;
        
        if (toughness < 0.0)
        {
          toughness = 0.0;
        }
        
        candidate->persistance -= (toughness+glbToughness);
      }
      
      if (candidate->persistance < 0.0)
      {
        for (uint64_t ix = 0; ix < harmony->size(); ix++)
        {
          Trust<int64_t> *localComp = cia->search((uint64_t)harmony->at(ix)->reality);
          
          if (localComp == NULL)
          {
            continue;
          }

          if (localComp->prediction->predictions.search((uint64_t)harmony->at(ix)->expectation))
          {
            localComp->prediction->predictions.remove(harmony->at(ix)->expectation, (uint64_t)harmony->at(ix)->expectation);
            delete harmony->at(ix)->expectation;
            harmony->at(ix)->expectation = NULL;
            //delete harmony->at(ix)->logicElement;
            //harmony->at(ix)->logicElement = NULL;
          }
          
          if (!localComp->prediction->predictions.size())
          {
            cia->remove(localComp, (uint64_t)harmony->at(ix)->reality);
            delete localComp->prediction;
            localComp->prediction = NULL;
          }
        }
        
        candidate->deinitialize();
        return true;
      }
      
      return false;
    }
    
    static uint64_t calcFitnessEach(LLRB_TreeNode<Heuristic<> *,uint64_t> *current, void *world)
    {
      Heuristic<> *candidate = current->data;
      vector<Harmony<> *> *harmony = candidate->getHarmony();
      int64_t fitness = 0.0, err;
      uint64_t harmonySize = 0;
      
      candidate->calcExpectation(currentTime);
      
      for (uint64_t ix = 0; ix < harmony->size(); ix++)
      {
        if ((harmony->at(ix) != NULL) && (harmony->at(ix)->logicElement != NULL))
        {
          int64_t output = *(harmony->at(ix)->reality);
          int64_t expectation = *(harmony->at(ix)->expectation);
          err = (output - expectation);
          err = pow(err,2);
          fitness += err;
          harmonySize++;
        }
      }
      
      if ((candidate->experiencedEpochs > (pow(candidate->hiddenWidth, 2) * ofAge)) && (!candidate->registered))
      {
        for (uint64_t ix = 0; ix < harmony->size(); ix++)
        {
          Trust<int64_t> *localComp = ((Genetic *)world)->answer.search((uint64_t)harmony->at(ix)->reality);
          if (localComp->prediction == NULL)
          {
            continue;
          }
          localComp->prediction->predictions.insert(harmony->at(ix)->expectation, (uint64_t)harmony->at(ix)->expectation);
          ((Genetic *)world)->active.insert(localComp, (uint64_t)harmony->at(ix)->reality);
        }
        candidate->registered = true;
      }
      
      fitness /= harmonySize;
      fitness = sqrt(fitness);
      
      candidate->persistance += (int64_t)harmonySize *
                                ((pow(harmonySize, 1.618) / pow(((Genetic *)world)->answer.size(), 1.618))) *
                                (1.0 - fitness);
      
      return current->key;
    }
    
    static uint64_t meditateEach(LLRB_TreeNode<Heuristic<> *,uint64_t> *current, void *world)
    {
      Heuristic<> *candidate = current->data;
      
      candidate->doCorrection();
      
      return current->key;
    }
    
    static void mutate(vector<Info *> *vect, uint64_t outputWidth,
                       uint64_t inputWidth, uint64_t hiddenWidth, int64_t error_rate);
    
    vector<int64_t *> *crossover(vector<int64_t *> *dad, vector<int64_t *> *mom);
    
    void add();
    void evaluateFitness();
    void checkSurvival();
    void doMeditation();
    void doGeneration();
    Heuristic<> *reproduce(Heuristic<> *father, Heuristic<> *mother);
    
  public:
    Genetic();
    
    ~Genetic()
    {
      
    }
    
    
  };
  
  class Genetic_NN : public Genetic
  {
    
    Heuristic<> *createNewHeuristic(vector<int64_t *> *input,
                                    vector<Trust<int64_t> *> *output,
                                    LLRB_Tree<int64_t *, uint64_t> *expectation,
                                    vector<Info *> *layers,
                                    uint32_t maxHiddenWidth)
    {
      return (Heuristic<> *)new NeuralNetwork::NeuralNetwork
      (
         input,
         output,
         expectation,
         layers,
         maxHiddenWidth
      );
    }
  };

  
}

#endif
