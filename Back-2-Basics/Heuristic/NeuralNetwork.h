//
//  NeuralNetwork.h
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 8/28/12.
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

#ifndef __Algorithms__NeuralNetwork__
#define __Algorithms__NeuralNetwork__

#include "Heuristic.h"
#include "Network.h"
#include "Stack.h"
#include <vector>
#include "LLRB_Tree.h"
using namespace Graph;


namespace NeuralNetwork
{
  
  
  class NeuralNetwork;
  class Neuron;
  class Axion;

#define RANDOM_INFLUENCE ((rand() > rand()) ? 1.0 : -1.0)
#define PPM 1e-6
#define LEARNING_RULE_DEFAULT   0.1082
#define OK  0
  
  static const double glbElecConductivity = 0.1082;
  static const double glbMagConductivity = 0.8928; // (1/30)^-30 = .8928; (1/90)^-90 = .95123

  
  enum SIG_FNCT_TYPE {
    NORMAL,
    PULSE
  };
  
  const double irrelevant = 10000.0 * PPM;
  static unsigned long currentIteration = -1;
  
  class Axion : public Pipe<Neuron,Axion> {
    
  protected:
    double mFlux = 0.0;
    double powerDissipation = 0.0;
    Info *adjacency = NULL;
    
  public:
    
    Axion(Neuron *neuron, Neuron *input, Info *tree);
    
    ~Axion() {
      if (adjacency != NULL) {
        //delete adjacency;
        adjacency = NULL;
      }
    }
    
    void changeInfluence(double capacity);
    
    friend Neuron;
  };
  
  class Neuron : public Hub<Neuron,Axion> {
    
  protected:
    
    uint64_t iteration = 0;
    const double inertia = glbMagConductivity;
    
    SIG_FNCT_TYPE sigmoidFunctionType = NORMAL;

    
    static bool optimalPruneEach(LLRB_TreeNode<Axion *, uint64_t> *current, void *neuron) {
      
      Neuron *input = current->data->getForward();
      double tVal = sqrt(pow(current->data->capacity,2) + pow(current->data->mFlux,2));
      bool ret = false;
      
      if (tVal < irrelevant) /*|| (current->data->powerDissipation > 0.01)) */ {
        ret = true;
      }
      
      if (input->discovered < input->references) {
        input->discovered++;
      }
        
      if (input->discovered == input->references) {
        input->forwardEdges.deletion(Neuron::optimalPruneEach, input);
        input->discovered = 0;
      }
      

      return ret;
    }
    
    static bool probablisticPruneEach(LLRB_TreeNode<Axion *, uint64_t> *current, void *neuron) {
      
      Neuron *input = current->data->getForward();
      double tVal = sqrt(pow(current->data->capacity,2) + pow(current->data->mFlux,2));
      double cutoff = pow(rand() / RAND_MAX, 2);
      bool ret = false;
      
      if ((tVal < cutoff) || (current->data->mFlux > 0.7))  {
        ret = true;
      }
      
      if (input->discovered < input->references) {
        input->discovered++;
      }
      
      if (input->discovered == input->references) {
        input->forwardEdges.deletion(Neuron::probablisticPruneEach, input);
        input->discovered = 0;
      }
      
      return ret;
    }
    
    static uint64_t probeActivationEach(LLRB_TreeNode<Axion *, uint64_t> *current, void *neuron) {
      
      Axion *synapse = current->data;
      Neuron *input = synapse->getForward();
      
      if ((synapse != NULL) && (input != NULL)) {
        *(((Neuron *)neuron)->memory) += input->probeActivation(currentIteration) * synapse->capacity;
      }
      return current->key;
    }
    
    static uint64_t calcDeltaEach(LLRB_TreeNode<Axion *, uint64_t> *current, void *neuron) {
      Axion *synapse = current->data;
      Neuron *input = current->data->getForward();
      
      if (input->discovered < input->references) {
        input->delta += synapse->capacity * ((Neuron *)neuron)->delta;
        input->discovered++;
      }
      
      if (input->discovered == input->references) {
        double nCurrentOutput = *(input->memory);
        
        input->delta = (nCurrentOutput * (1.0 - nCurrentOutput) * input->delta);
        
        /* TODO don't go deeper if error is small */
        if (pow(input->delta, 2) > irrelevant) {
          input->modifyAllAdjacent(calcDeltaEach, input);
        }
      }
      
      return current->key;
    }
    
    static uint64_t changeInputInfluenceEach(LLRB_TreeNode<Axion *, uint64_t> *current, void *neuron) {
      double correction;
      Axion *synapse = current->data;
      Neuron *input = synapse->getForward();
      Neuron *pCurrentNeuron = ((Neuron *)neuron);
      
      if (pCurrentNeuron->discovered) {
        correction = (pCurrentNeuron->delta * *(pCurrentNeuron->memory));
        
        synapse->changeInfluence(correction);
        
        input->modifyAllAdjacent(Neuron::changeInputInfluenceEach, input);
        
        input->discovered = 0;
        input->delta = 0.0;
      }
      
      return current->key;
    }
    
  public:
    
    double delta = 0.0;
    double *memory = NULL;
    double *ptrInput = NULL;
    double totalInputs = 0.0;
    
    Neuron(double *, double *);
    
    double probeActivation(uint64_t iteration);
    
    friend class NeuralNetwork;
    friend class Axion;
  };
  
  
  class NeuralNetwork : public Network<Neuron,Axion> , public Heuristic<NeuralNetwork, Neuron, double>
  {
    
  private:
    
    vector<Neuron *> inputs;
    vector<Harmony<Neuron> *> outputs;
    Neuron *bias;
    vector<Info *> hiddenInfo;
    vector<vector<Neuron *> *> *layers;
    bool layersDetermined = false;
    double networkBias = -1.0;
    //double learningRule = LEARNING_RULE_DEFAULT;
    vector<Axion *> terminalEdges;
    
  protected:
    
    static void changeInputInfluenceEach(Harmony<Neuron> *current) {
      Neuron *pCurrentNeuron = (Neuron *)(current->logicElement);
      
      if (pCurrentNeuron->discovered) {
        pCurrentNeuron->modifyAllAdjacent(Neuron::changeInputInfluenceEach, pCurrentNeuron);
        
        pCurrentNeuron->discovered = 0;
        pCurrentNeuron->delta = 0.0;
      }
    }
    
    static uint64_t calcExpectationEach(LLRB_TreeNode<Harmony<Neuron> *, uint64_t> *current, void *iteration) {
      current->data->logicElement->probeActivation((uint64_t)iteration);
      return current->key;
    }
    
    static void doCorrectionEach(Harmony<Neuron> *current) {
      Neuron *pCurrentNeuron = (Neuron *)(current->logicElement);
      double reality = *(current->reality);
      double expectation = *(pCurrentNeuron->memory);
      
      /* TODO don't propagate small errors or no data */
      if (reality > irrelevant) {
        pCurrentNeuron->delta = (expectation *
                                 (1.0 - expectation) *
                                 (reality - expectation));
        
        if (pow(pCurrentNeuron->delta, 2) > irrelevant) {
          pCurrentNeuron->modifyAllAdjacent(Neuron::calcDeltaEach, pCurrentNeuron);
        }
        
        pCurrentNeuron->discovered = 1;
      }
    }
    
    static uint64_t addInputToVectorEach(LLRB_TreeNode<Neuron *, uint64_t> *current, void *vect) {
      
      ((vector<double *> *)vect)->push_back(current->data->ptrInput);
      
      return current->key;
    }
    
  public:
    
    NeuralNetwork();
    NeuralNetwork(std::vector<double *> *input,
                  std::vector<double *> *output,
                  std::vector<double *> *expectation,
                  std::vector<Info *> *layers);
    
    void calcExpectation(uint64_t);
    void doCorrection();
    
    vector<double *> *getInputs();
    vector<Info *> *getHiddenInfo();
    vector<Harmony<Neuron> *> *getHarmony();
        
    uint64_t timeAlive();
    
    void optimalPrune();
    void probablisticPrune();
    
  };
  
}


#endif
