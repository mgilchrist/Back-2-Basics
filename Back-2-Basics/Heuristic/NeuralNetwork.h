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
  
#define LEARNING_RULE_DEFAULT   1.0
#define OK  0
#define DATA_STRUCT_INIT_SIZE   16
#define INERTIA_DEFAULT 0.1
  
  
  enum SIG_FNCT_TYPE {
    NORMAL,
    PULSE
  };
  
  
  class Synapse : public Pipe<Neuron> {
    
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
    
    
    double inputInfluence = 0.0;
    double inputLastCorrection = 0.0;
    
    double *memory;
    uint64_t iteration = 0;
    const double inertia = INERTIA_DEFAULT;
    
    SIG_FNCT_TYPE sigmoidFunctionType = NORMAL;
    
  public:
    
    double delta;
    double inputInfluenceDelta;
    uint64_t outputCount = 0;
    double *inputBias;
    
    Neuron(NeuralNetwork *,Collection::Stack<Neuron *> *);
    Neuron(NeuralNetwork *,Collection::Stack<Neuron *> *,double *);
    Neuron(NeuralNetwork *,double *);
    double getMemory() {return *memory;}
    double probeActivation(uint64_t iteration);
    double getInputInfluence(uint64_t);
    double getInputMomentum(uint64_t);
    void addInputNeuron(NeuralNetwork *,Neuron *);
    void changeInputInfluence(uint64_t);
    void colapseInputNeuron(uint64_t);
  };
  
  
  class NeuralNetwork : public Network<Neuron,Synapse> , public Heuristic<NeuralNetwork>
  {
  private:
    
    unsigned long currentIteration;
    LLRB_Tree<Neuron *, uint64_t> inputs;
    Collection::Stack<Collection::Stack<Neuron *> *> *layers;
    bool layersDetermined = false;
    double glbBias = 0.5;
    double learningRule = LEARNING_RULE_DEFAULT;
    //vector<double *> *inputData, *expectationData, *outputData;
    
    LLRB_Tree<pair<Neuron *, double *> *, uint64_t> output;
    
    static uint64_t getExpectationEach(LLRB_TreeNode<std::pair<Neuron *, double *> *, uint64_t> *current, void *expectation) {
      
      return current->key;
    }
    
    static uint64_t calcExpectationEach(LLRB_TreeNode<std::pair<Neuron *, double *> *, uint64_t> *current, void *iteration) {
      current->data->first->probeActivation((uint64_t)iteration);
      return current->key;
    }
    
    static uint64_t doCorrectionEach(LLRB_TreeNode<pair<Neuron *, double *> *, uint64_t> *current, void *reserved) {
      Neuron *pCurrentNeuron = current->data->first;
      double reality = *(current->data->second);
      double expectation = pCurrentNeuron->getMemory();
      
      pCurrentNeuron->delta = (expectation *
                               (1.0 - expectation) *
                               (reality - expectation));
      
      pCurrentNeuron->inputInfluenceDelta += ((pCurrentNeuron->getInputInfluence(0) *
                                               pCurrentNeuron->delta) *
                                              (pCurrentNeuron->getMemory() *
                                               (1.0 - pCurrentNeuron->getMemory())));
      
      for (uint64_t ix = 0; ix < pCurrentNeuron->getNumEdges(); ix++) {
        pCurrentNeuron->getAdjacentNode(ix)->delta += (pCurrentNeuron->getInputInfluence(ix+1) *
                                                       pCurrentNeuron->delta);
      }
      
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
    
    static uint64_t addEach(LLRB_TreeNode<Neuron *, uint64_t> *current, void *network) {
      
      ((NeuralNetwork *)network)->inputs.insert(current->data, current->key);
      
      return current->key;
    }
    
    virtual void simplify();
    virtual void merge(NeuralNetwork *);
    
  };
  
}


#endif
