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
  
#define LEARNING_RULE_DEFAULT   0.01
#define OK  0
#define INERTIA_DEFAULT 0.8928 // (1/30)^-30 = .8928
  
#define RANDOM_INFLUENCE ((rand() > rand()) ? 1.0 : 1.0)
  
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
    
  protected:
    string dbgName;
    double influence;
    double lastCorrection;
    
  public:
    
    Synapse(Neuron *neuron, Neuron *input);
    
    void changeInfluence(double influence);
    void multInfluence(double influence);
    
    double getInfluence();
    double getMomentum();
    
    friend Neuron;
  };
  
  class Neuron : public Hub<Neuron,Synapse> {
    
  protected:
    
    uint64_t iteration = 0;
    const double inertia = INERTIA_DEFAULT;
    
    SIG_FNCT_TYPE sigmoidFunctionType = NORMAL;
    
    static uint64_t addInputsEach(LLRB_TreeNode<Harmony *, uint64_t> *current, void *neuron) {
      new Synapse((Neuron *)neuron, current->data->expectation);
      
      return current->key;
    }
    
    static uint64_t pruneEdgesEach(LLRB_TreeNode<Synapse *, uint64_t> *current, void *neuron) {
      
      if (current->data->capacity < irrelevant) {
        ((Neuron *)neuron)->removeEdge(current->data);
      }
      
      return current->key;
    }
    
    static uint64_t probeActivationEach(LLRB_TreeNode<Synapse *, uint64_t> *current, void *neuron) {
      
      Synapse *synapse = current->data;
      Neuron *input = synapse->getForward();
      
      if ((synapse != NULL) && (input != NULL)) {
        *(((Neuron *)neuron)->memory) += input->probeActivation(currentIteration) * synapse->getInfluence();
      }
      return current->key;
    }
    
    static uint64_t calcDeltaEach(LLRB_TreeNode<Synapse *, uint64_t> *current, void *neuron) {
      Synapse *synapse = current->data;
      Neuron *input = current->data->getForward();
      
      if (input->discovered < input->references) {
        input->delta += synapse->getInfluence() * ((Neuron *)neuron)->delta;
        input->discovered++;
      }
      
      if (input->discovered == input->references) {
        double nCurrentOutput = *(input->memory);
        
        input->delta = (nCurrentOutput * (1.0 - nCurrentOutput) * input->delta);
        
        input->modifyAllAdjacent(calcDeltaEach, input);
      }
      
      return current->key;
    }
    
    static uint64_t changeInputInfluenceEach(LLRB_TreeNode<Synapse *, uint64_t> *current, void *neuron) {
      double correction;
      Synapse *synapse = current->data;
      Neuron *input = synapse->getForward();
      Neuron *pCurrentNeuron = ((Neuron *)neuron);
      
      if (pCurrentNeuron->discovered) {
        correction = LEARNING_RULE_DEFAULT * pCurrentNeuron->delta * *(pCurrentNeuron->memory); // TODO
        correction += synapse->getMomentum();
        
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
    double *inputBias = NULL;
    double totalInputs = 0.0;
    
    Neuron(LLRB_Tree<Harmony *, uint64_t> *, vector<Neuron *> *, double *, double *);
    
    double probeActivation(uint64_t iteration);
    void addInputNeuron(NeuralNetwork *,Neuron *);
    
    friend class NeuralNetwork;
    friend class Synapse;
  };
  
  
  class NeuralNetwork : public Network<Neuron,Synapse> , public Heuristic<NeuralNetwork>
  {
  private:
    
    LLRB_Tree<Neuron *, uint64_t> inputs;
    LLRB_Tree<Harmony *, uint64_t> outputs;
    vector<vector<Neuron *> *> *layers;
    bool layersDetermined = false;
    double networkBias = 1.0;
    double learningRule = LEARNING_RULE_DEFAULT;
    vector<Synapse *> terminalEdges;
    
  protected:
    
    double *glbBias;
    
    static uint64_t changeInputInfluenceEach(LLRB_TreeNode<Harmony *, uint64_t> *current, void *nnetwork) {
      Neuron *pCurrentNeuron = current->data->expectation;
      
      if (pCurrentNeuron->discovered) {
        pCurrentNeuron->modifyAllAdjacent(Neuron::changeInputInfluenceEach, pCurrentNeuron);
        
        pCurrentNeuron->discovered = 0;
        pCurrentNeuron->delta = 0.0;
      }
      
      return current->key;
    }
    
    static uint64_t calcExpectationEach(LLRB_TreeNode<Harmony *, uint64_t> *current, void *iteration) {
      current->data->expectation->probeActivation((uint64_t)iteration);
      return current->key;
    }
    
    static uint64_t doCorrectionEach(LLRB_TreeNode<Harmony *, uint64_t> *current, void *reserved) {
      Neuron *pCurrentNeuron = current->data->expectation;
      double reality = *(current->data->reality);
      double expectation = *(pCurrentNeuron->memory);
      
      pCurrentNeuron->delta = (expectation *
                               (1.0 - expectation) *
                               (reality - expectation));
      
      pCurrentNeuron->modifyAllAdjacent(Neuron::calcDeltaEach, pCurrentNeuron);
      pCurrentNeuron->discovered = 1;
      
      return current->key;
    }
    
    static uint64_t addEach(LLRB_TreeNode<Neuron *, uint64_t> *current, void *network) {
      
      ((NeuralNetwork *)network)->inputs.insert(current->data, current->key);
      
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
    
    virtual void simplify();
    virtual void merge(NeuralNetwork *);
    
  };
  
}


#endif
