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
  class Synapse;
  
#define LEARNING_RULE_DEFAULT   0.3
#define OK  0
#define DATA_STRUCT_INIT_SIZE   16
#define INERTIA_DEFAULT 0.1
  
#define RANDOM_INFLUENCE ((rand() > rand()) ? 1.0 : -1.0)
  
  enum SIG_FNCT_TYPE {
    NORMAL,
    PULSE
  };
  
  typedef struct Harmony {
    Neuron *expectation;
    double *reality;
  } Harmony;
  
  const double irrelevant = 10e-14;
  static unsigned long currentIteration;
  
  class Synapse : public Pipe<Neuron,Synapse> {
    
  private:
    double influence;
    double lastCorrection;
    
  public:
    
    Synapse(Neuron *neuron, Neuron *input, double influence);
    
    void changeInfluence(double influence);
    void multInfluence(double influence);
    
    double getInfluence();
    double getMomentum();
  };
  
  class Neuron : public Hub<Neuron,Synapse> {
    
  protected:
    
    
    double inputLastCorrection = 0.0;
    
    uint64_t iteration = 0;
    const double inertia = INERTIA_DEFAULT;
    
    SIG_FNCT_TYPE sigmoidFunctionType = NORMAL;
    
    static uint64_t addInputsEach(LLRB_TreeNode<Harmony *, uint64_t> *current, void *neuron) {
      new Synapse((Neuron *)neuron, current->data->expectation, RANDOM_INFLUENCE);
      
      return current->key;
    }
    
  public:
    
    double delta;
    double *memory;
    double inputInfluenceDelta;
    double inputInfluence = 0.0;
    uint64_t outputCount = 0;
    double *inputBias;
    
    static uint64_t pruneEdgesEach(LLRB_TreeNode<Synapse *, uint64_t> *current, void *neuron) {
      
      if (current->data->capacity < irrelevant) {
        ((Neuron *)neuron)->removeEdge(current->data);
      }

      return current->key;
    }
    
    Neuron(LLRB_Tree<Harmony *, uint64_t> *, double *, double *);
    Neuron(Collection::Stack<Neuron *> *,double *, double *);
    Neuron(double *, double *);
    double probeActivation(uint64_t iteration);
    
    static uint64_t changeInputInfluenceEach(LLRB_TreeNode<Synapse *, uint64_t> *current, void *neuron) {
      double correction;
      Synapse *synapse = current->data;
      Neuron *pCurrentNeuron =synapse->getForward();
      
      correction = LEARNING_RULE_DEFAULT * pCurrentNeuron->delta;
      correction += synapse->getMomentum();
      
      synapse->changeInfluence(correction);
      
      return current->key;
    }
    void addInputNeuron(NeuralNetwork *,Neuron *);
    void changeInputInfluence();
  };
  
  
  class NeuralNetwork : public Network<Neuron,Synapse> , public Heuristic<NeuralNetwork>
  {
  private:
    
    LLRB_Tree<Neuron *, uint64_t> inputs;
    LLRB_Tree<Harmony *, uint64_t> outputs;
    Collection::Stack<Collection::Stack<Neuron *> *> *layers;
    bool layersDetermined = false;
    double networkBias = 1.0;
    double learningRule = LEARNING_RULE_DEFAULT;
    vector<Synapse *> terminalEdges;
    
  protected:
    
    static uint64_t addTerminal(LLRB_TreeNode<Neuron *, uint64_t> *current, void *nnetwork) {
      Synapse *edge = new Synapse(((NeuralNetwork *)nnetwork)->terminal, current->data, 0.0);
      
      ((NeuralNetwork *)nnetwork)->terminalEdges.push_back(edge);
      
      current->data->addEdge(edge);
      
      return current->key;
    }
    
    static uint64_t removeTerminal(LLRB_TreeNode<Neuron *, uint64_t> *current, void *neuron) {
      
      
      return current->key;
    }
    
    static uint64_t calcExpectationEach(LLRB_TreeNode<Harmony *, uint64_t> *current, void *iteration) {
      current->data->expectation->probeActivation((uint64_t)iteration);
      return current->key;
    }
    
    static uint64_t calcDeltaEach(LLRB_TreeNode<Synapse *, uint64_t> *current, void *neuron) {
      current->data->getForward()->delta += current->data->getInfluence() * ((Neuron *)neuron)->delta;
      
      return current->key;
    }
    
    static uint64_t doCorrectionEach(LLRB_TreeNode<Harmony *, uint64_t> *current, void *reserved) {
      Neuron *pCurrentNeuron = current->data->expectation;
      double reality = *(current->data->reality);
      double expectation = *pCurrentNeuron->memory;
      
      pCurrentNeuron->delta = (expectation *
                               (1.0 - expectation) *
                               (reality - expectation));
      
      pCurrentNeuron->inputInfluenceDelta += ((pCurrentNeuron->inputInfluence *
                                               pCurrentNeuron->delta) *
                                              (*pCurrentNeuron->memory *
                                               (1.0 - *pCurrentNeuron->memory)));
      
      pCurrentNeuron->modifyAllAdjacent(calcDeltaEach, pCurrentNeuron);

      return current->key;
    }
    
  public:
    NeuralNetwork();
    NeuralNetwork(std::vector<double *> *input,
                  std::vector<double *> *output,
                  std::vector<double *> *expectation,
                  std::vector<uint64_t> *layers);
    NeuralNetwork *clone();
    
    void calcExpectation(void);
    void doCorrection();
    
    static uint64_t probeActivationEach(LLRB_TreeNode<Synapse *, uint64_t> *current, void *output) {
      *(((Neuron *)output)->memory) += current->data->getForward()->probeActivation(currentIteration) * current->data->getInfluence();
      
      return current->key;
    }
    
    static uint64_t addEach(LLRB_TreeNode<Neuron *, uint64_t> *current, void *network) {
      
      ((NeuralNetwork *)network)->inputs.insert(current->data, current->key);
      
      return current->key;
    }
    
    virtual void simplify();
    virtual void merge(NeuralNetwork *);
    
  };
  
}


#endif
