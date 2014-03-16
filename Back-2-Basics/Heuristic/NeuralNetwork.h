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

#include "config.h"
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
  class Axon;

#define RANDOM_INFLUENCE ((rand() > rand()) ? 1.0 : -1.0)
#define PPM 1e-6
#define LEARNING_RULE_DEFAULT   1082
#define OK  0
  
  static const int64_t glbElecConductivity = 1082;
  static const int64_t glbMagConductivity = 8928; // (1/30)^-30 = .8928; (1/90)^-90 = .95123

  
  enum SIG_FNCT_TYPE
  {
    NORMAL,
    PULSE
  };
  
  const int64_t irrelevant = 10000 * PPM;
  static unsigned long currentIteration = -1;
  
  class Axon : public Pipe<Neuron,Axon>
  {
  private:
#if (DEBUG == 1)
    bool initialized = false;
#endif
    
  protected:
    int64_t mFlux = 0;
    int64_t powerDissipation = 0;
    Info *adjacency = NULL;
    
    void initialize(Neuron *neuron, Neuron *input, Info *tree);
    
  public:
    
    Axon();
    
    ~Axon()
    {
      if (adjacency != NULL)
      {
        //delete adjacency;
        adjacency = NULL;
      }
    }
    
    Axon(Neuron *neuron, Neuron *input, Info *tree)
    {
      initialize(neuron, input, tree);
    }
    
    void changeInfluence(int64_t capacity);
    
    friend Neuron;
  };
  
  class Neuron : public Hub<Neuron,Axon>, public LogicOperator
  {
    
  private:
    NeuralNetwork *nnetwork;
    
#if (DEBUG == 1)
    bool initialized = false;
#endif
    
  protected:
    
    uint64_t iteration = 0;
    const int64_t inertia = glbMagConductivity;
    
    SIG_FNCT_TYPE sigmoidFunctionType = NORMAL;

    
    static bool optimalPruneEach(LLRB_TreeNode<Axon *, uint64_t> *current, void *neuron)
    {
      Neuron *input = current->data->getForward();
      int64_t tVal = sqrt(pow(current->data->capacity(),2) + pow(current->data->mFlux,2));
      bool ret = false;
      
      if (tVal < irrelevant) /*|| (current->data->powerDissipation > 0.01)) */
      {
        ret = true;
      }
      
      if (input->discovered < input->references)
      {
        input->discovered++;
      }
        
      if (input->discovered == input->references)
      {
        input->forwardEdges->deletion(Neuron::optimalPruneEach, input);
        input->discovered = 0;
      }
      
      return ret;
    }
    
    static bool probablisticPruneEach(LLRB_TreeNode<Axon *, uint64_t> *current, void *neuron)
    {
      
      Neuron *input = current->data->getForward();
      int64_t tVal = sqrt(pow(current->data->capacity(),2) + pow(current->data->mFlux,2));
      int64_t cutoff = pow(rand() / RAND_MAX, 2);
      bool ret = false;
      
      if ((tVal < cutoff) || (current->data->mFlux > 0.7))
      {
        ret = true;
      }
      
      if (input->discovered < input->references)
      {
        input->discovered++;
      }
      
      if (input->discovered == input->references)
      {
        input->forwardEdges->deletion(Neuron::probablisticPruneEach, input);
        input->discovered = 0;
      }
      
      return ret;
    }
    
    static uint64_t probeActivationEach(LLRB_TreeNode<Axon *, uint64_t> *current, void *neuron)
    {
      Axon *synapse = current->data;
      Neuron *input = synapse->getForward();
      
      if ((synapse != NULL) && (input != NULL))
      {
        *(((Neuron *)neuron)->memory) += input->probeActivation(currentIteration) * synapse->capacity();
      }
      
      return current->key;
    }
    
    static uint64_t calcDeltaEach(LLRB_TreeNode<Axon *, uint64_t> *current, void *neuron)
    {
      Axon *synapse = current->data;
      Neuron *input = current->data->getForward();
      
      if (input->discovered < input->references)
      {
        input->delta += synapse->capacity() * ((Neuron *)neuron)->delta;
        input->discovered++;
      }
      
      if (input->discovered == input->references)
      {
        int64_t nCurrentOutput = *(input->memory);
        
        input->delta = (nCurrentOutput * (1.0 - nCurrentOutput) * input->delta);
        
        /* TODO don't go deeper if error is small */
        if (pow(input->delta, 2) > irrelevant)
        {
          input->modifyAllAdjacent(calcDeltaEach, input);
        }
      }
      
      return current->key;
    }
    
    static uint64_t changeInputInfluenceEach(LLRB_TreeNode<Axon *, uint64_t> *current, void *neuron)
    {
      int64_t correction;
      Axon *synapse = current->data;
      Neuron *input = synapse->getForward();
      Neuron *pCurrentNeuron = ((Neuron *)neuron);
      
      if (pCurrentNeuron->discovered)
      {
        correction = (pCurrentNeuron->delta * *(pCurrentNeuron->memory));
        
        synapse->changeInfluence(correction);
        
        input->modifyAllAdjacent(Neuron::changeInputInfluenceEach, input);
        
        input->discovered = 0;
        input->delta = 0.0;
      }
      
      return current->key;
    }
    
    void deinitialize();
    void initialize(int64_t *, int64_t *, NeuralNetwork *);
    
  public:
    
    int64_t delta = 0.0;
    int64_t *memory = NULL;
    int64_t *ptrInput = NULL;
    int64_t totalInputs = 0.0;
    bool privateMemory, privateInput;
    
    Neuron()
    {
      
    }
    
    ~Neuron()
    {
      deinitialize();
    }
    
    Neuron(int64_t *inputData, int64_t *expectation, NeuralNetwork *neuralNetwork)
    {
      initialize(inputData, expectation, neuralNetwork);
    }
    
    int64_t probeActivation(uint64_t iteration);
    
    friend class NeuralNetwork;
    friend class Axon;
  };
  
  
  class NeuralNetwork : public Network<Neuron,Axon> , public Heuristic<Neuron>
  {
    
  private:
#if (DEBUG == 1)
    bool initialized = false;
#endif
    vector<Neuron *> inputs;
    vector<Harmony<Neuron> *> outputs;
    Neuron *bias;
    vector<Info *> hiddenInfo;
    vector<vector<Neuron *> *> *layers;
    bool layersDetermined = false;
    int64_t networkBias = -1.0;
    //int64_t learningRule = LEARNING_RULE_DEFAULT;
    vector<Axon *> terminalEdges;
    
  protected:
    
    uint64_t numberOfNeurons = 0;
    
    static void changeInputInfluenceEach(Harmony<Neuron> *current)
    {
      Neuron *pCurrentNeuron = (Neuron *)(current->logicElement);
      
      if (pCurrentNeuron->discovered)
      {
        pCurrentNeuron->modifyAllAdjacent(Neuron::changeInputInfluenceEach, pCurrentNeuron);
        
        pCurrentNeuron->discovered = 0;
        pCurrentNeuron->delta = 0.0;
      }
    }
    
    static uint64_t calcExpectationEach(LLRB_TreeNode<Harmony<Neuron> *, uint64_t> *current, void *iteration)
    {
      current->data->logicElement->probeActivation((uint64_t)iteration);
      return current->key;
    }
    
    static void doCorrectionEach(Harmony<Neuron> *current)
    {
      Neuron *pCurrentNeuron = (Neuron *)(current->logicElement);
      int64_t reality = *(current->reality);
      int64_t expectation = *(pCurrentNeuron->memory);
      
      /* TODO don't propagate small errors or no data */
      if (reality > irrelevant) {
        pCurrentNeuron->delta = (expectation *
                                 (1.0 - expectation) *
                                 (reality - expectation));
        
        if (pow(pCurrentNeuron->delta, 2) > irrelevant)
        {
          pCurrentNeuron->modifyAllAdjacent(Neuron::calcDeltaEach, pCurrentNeuron);
        }
        
        pCurrentNeuron->discovered = 1;
      }
    }
    
    static uint64_t addInputToVectorEach(LLRB_TreeNode<Neuron *, uint64_t> *current, void *vect)
    {
      ((vector<int64_t *> *)vect)->push_back(current->data->ptrInput);
      
      return current->key;
    }
    
    void deinitialize();
    
    void initialize(vector<int64_t *> *input,
                    vector<Trust<int64_t> *> *output,
                    LLRB_Tree<int64_t *, uint64_t> *expectation,
                    vector<Info *> *layers,
                    uint32_t maxHiddenWidth);
    
  public:
    
    
    NeuralNetwork()
    {
      
    }
    
    ~NeuralNetwork()
    {
      deinitialize();
    }
    
    NeuralNetwork(vector<int64_t *> *input,
                  vector<Trust<int64_t> *> *output,
                  LLRB_Tree<int64_t *, uint64_t> *expectation,
                  vector<Info *> *layers,
                  uint32_t maxHiddenWidth)
    {
      initialize(input, output, expectation, layers, maxHiddenWidth);
    }
    
    void calcExpectation(uint64_t);
    void doCorrection();
    
    vector<int64_t *> *getInputs();
    vector<Info *> *getHiddenInfo();
    vector<Harmony<Neuron> *> *getHarmony();
        
    uint64_t timeAlive();
    
    void optimalPrune();
    void probablisticPrune();
    
    friend class Neuron;
    
  };
  
}


#endif
