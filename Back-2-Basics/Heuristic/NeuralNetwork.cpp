//
//  NeuralNetwork.cpp
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 9/21/12.
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


#include <iostream>
using namespace std;

#include "NeuralNetwork.h"

//#define NEURALNETWORK_DEBUG

namespace NeuralNetwork
{
  
#define MAX_LAYERS    7   // 3 for space 7 for oct math and 10 for m-theory
  
  static const double zeroInfluence = 0.0;
  static double glbBias = 1.0;
  
  Synapse::Synapse(Neuron *input, Neuron *neuron, double influence) {
    this->u = neuron;
    this->v = input;
    
    input->references++;
    neuron->addEdge(this);
    influence = influence;
    //this->capacity = (influence < 0) ? -influence : influence;
    lastCorrection = 0.0;
  }
  
  void Synapse::changeInfluence(double influence) {
    this->influence += influence;
    //this->capacity = (this->influence < 0) ? -this->influence : this->influence;
    lastCorrection = influence;
  }
  
  void Synapse::multInfluence(double influence) {
    this->influence *= influence;
    //this->capacity = (this->influence < 0) ? -this->influence : this->influence;
    lastCorrection *= influence;
  }
  
  double Synapse::getInfluence() {
    return this->influence;
  }
  
  double Synapse::getMomentum() {
    return (INERTIA_DEFAULT * this->lastCorrection);
  }
  
  Neuron::Neuron(LLRB_Tree<Harmony *, uint64_t> *layer, double *inputData, double *expectation) {
    this->capacity = 0.0;
    
    this->inputBias = inputData;
    inputInfluence = RANDOM_INFLUENCE;
    memory = new double(0.0);
    
    layer->modifyAll(Neuron::addInputsEach, this);
    
    iteration = 0;
    delta = 0.0;
    inputInfluenceDelta = 0.0;
    
  }
  
  Neuron::Neuron(Collection::Stack<Neuron *> *input, double *inputData, double *expectation) {
    this->capacity = 0.0;
    
    Synapse *synapse;
    inputBias = inputData;
    inputInfluence = RANDOM_INFLUENCE;
    
    if (expectation == NULL) {
      memory = new double;
    } else {
      memory = expectation;
    }
    
    for (int ix = 0; ix < input->getSize(); ix++) {
      synapse = new Synapse(input->atIndex(ix), this, RANDOM_INFLUENCE);
      
    }
    
    iteration = 0;
    delta = 0.0;
    inputInfluenceDelta = 0.0;
    
  }
  
  Neuron::Neuron(double *inputData, double *expectation) {
    this->capacity = 0.0;
    
    this->inputBias = inputData;
    inputInfluence = RANDOM_INFLUENCE;
    inputLastCorrection = 0.0;
    
    if (expectation == NULL) {
      memory = new double;
    } else {
      memory = expectation;
    }
    
    iteration = 0;
    delta = 0.0;
    inputInfluenceDelta = 0.0;
    
  }
  
  double Neuron::probeActivation(uint64_t iteration) {
    
    if (this->iteration == iteration) {
      return *memory;
    }
    
    *memory = *inputBias * inputInfluence;
    
    // Get impulse from relavent connected neurons
    modifyAllAdjacent(NeuralNetwork::probeActivationEach, this);
    
    *memory = (1.0 / (1.0 + exp(double((-1.0) * *memory))));
    
    this->iteration = iteration;
    
    this->delta = 0.0;
    this->inputInfluenceDelta = 0.0;
    
    return *memory;
  }
  /*
   double Neuron::getInputInfluence(uint64_t index) {
   
   if (!index) {
   return inputInfluence;
   }
   return this->getAdjacentEdge(index-1)->getInfluence();
   }
   
   double Neuron::getInputMomentum(uint64_t index) {
   
   if (!index) {
   return inputLastCorrection * inertia;
   }
   
   return this->getAdjacentEdge(index-1)->getMomentum();
   }
   */
  
  void Neuron::addInputNeuron(NeuralNetwork *network, Neuron *input) {
    
    new Synapse(input, this, RANDOM_INFLUENCE);
    
  }
  
  void Neuron::changeInputInfluence() {
    double correction;
    
    correction = LEARNING_RULE_DEFAULT * this->inputInfluenceDelta;
    correction += this->inputLastCorrection * this->inertia;
    inputLastCorrection = correction;
    inputInfluence += this->inputInfluenceDelta;
  }
  
  NeuralNetwork::NeuralNetwork(std::vector<double *> *input,
                               std::vector<double *> *output,
                               std::vector<double *> *expectation,
                               std::vector<uint64_t> *layers) {
    
    Collection::Stack<Neuron *> *currentStack, *previousStack;
    Neuron *currentNeuron;
    
    
    this->layers = new Collection::Stack<Collection::Stack<Neuron *> *>(layers->size()+2);
    
    
    currentStack = new Collection::Stack<Neuron *>(input->size());
    
    for (int jx = 0; jx < input->size(); jx++) {
      currentNeuron = new Neuron(input->at(jx), NULL);
      inputs.insert(currentNeuron, (uint64_t)input->at(jx));
      currentStack->push(currentNeuron);
    }
    
    this->layers->push(currentStack);
    
    for (int ix = 0; ix < layers->size(); ix++) {
      previousStack = currentStack;
      currentStack = new Collection::Stack<Neuron *>(layers->at(ix));
      
      for (uint64_t jx = 0; jx < layers->at(ix); jx++) {
        currentNeuron = new Neuron(previousStack,&glbBias, NULL);
        currentStack->push(currentNeuron);
      }
      this->layers->push(currentStack);
    }
    
    previousStack = currentStack;
    currentStack = new Collection::Stack<Neuron *>(output->size());
    
    for (int ix = 0; ix < output->size(); ix++) {
      currentNeuron = new Neuron(previousStack, &glbBias, expectation->at(ix));
      Harmony *tmp = new Harmony;
      tmp->expectation = currentNeuron;
      tmp->reality = output->at(ix);
      outputs.insert(tmp, (uint64_t)(output->at(ix)));
      currentStack->push(currentNeuron);
    }
    
    this->layers->push(currentStack);
    
    currentIteration = 0;
  }
  
  void NeuralNetwork::calcExpectation() {
    
    currentIteration++;
    
    this->outputs.modifyAll(NeuralNetwork::calcExpectationEach, (void *)currentIteration);
  }
  
  void NeuralNetwork::doCorrection() {
    
    vector<vector<Neuron *> *> *layers;
    vector<Neuron *> *currentLayer;
    Neuron *pCurrentNeuron;
    double nCurrentOutput;
    
    
    start = new Neuron(&outputs,&glbBias,NULL);
    terminal = new Neuron(&glbBias,NULL);
    
    terminalEdges.reserve(inputs.size());
    inputs.modifyAll(NeuralNetwork::addTerminal, this);
    
    
    layers = getLayers();
    
    delete start;
    terminalEdges.clear();
    
    layersDetermined = true;
    
    outputs.modifyAll(NeuralNetwork::doCorrectionEach, 0);
    
    for (uint64_t ix = 2; ix <= layers->size(); ix++) {
      currentLayer = layers->at(layers->size()-ix);
      for (uint64_t jx = 0; jx < currentLayer->size(); jx++) {
        pCurrentNeuron = currentLayer->at(jx);
        nCurrentOutput = *pCurrentNeuron->memory;
        
        pCurrentNeuron->delta = (nCurrentOutput *
                                 (1.0 - nCurrentOutput) *
                                 pCurrentNeuron->delta);
      }
    }
    
    for (uint64_t ix = 0; ix < layers->size(); ix++) {
      currentLayer = layers->at(ix);
      
      for (uint64_t jx = 0; jx < currentLayer->size(); jx++) {
        pCurrentNeuron = currentLayer->at(jx);
        pCurrentNeuron->changeInputInfluence();
        pCurrentNeuron->modifyAllAdjacent(Neuron::changeInputInfluenceEach, 0);
      }
    }
  }
  
  void NeuralNetwork::simplify() {
    Collection::Array<Neuron *> *currentLayer;
    Neuron *pCurrentNeuron;
    terminal = new Neuron((double *)NULL,NULL);
    
    /* Eliminate synapses that never carry significant charge */
    /* Count output synapses */
    
    for (uint64_t kx = 0; kx < this->layers->getSize()-1; kx++) {
      currentLayer = this->layers->atIndex(kx);
      
      for (int jx = 0; jx < currentLayer->getSize(); jx++) {
        currentLayer->atIndex(jx)->outputCount = 0;
      }
    }
    
    currentLayer = this->layers->atIndex(this->layers->getSize()-1);
    
    for (uint64_t jx = 0; jx < currentLayer->getSize(); jx++) {
      currentLayer->atIndex(jx)->outputCount = 1;
    }
    
    for (uint64_t kx = 1; kx < this->layers->getSize(); kx++) {
      currentLayer = this->layers->atIndex(this->layers->getSize()-kx);
      
      for (uint64_t jx = 0; jx < currentLayer->getSize(); jx++) {
        pCurrentNeuron = currentLayer->atIndex(jx);
        
        /* Get rid of irrelevant synapses */
        pCurrentNeuron->modifyAllAdjacent(Neuron::pruneEdgesEach, 0);
      }
    }
    
  }
  
  
  void NeuralNetwork::merge(NeuralNetwork *network) {
    
    Collection::Stack<Neuron *> *currentStack, *previousStack, *mergeeLayer;
    LLRB_Tree<Neuron *, uint64_t> *outputTree = new LLRB_Tree<Neuron *, uint64_t>();
    
    Neuron *tmpInput;
    uint64_t outputSize;
    
    network->inputs.modifyAll(NeuralNetwork::addEach, this);
    
    currentStack = this->layers->atIndex(network->layers->getSize()-1);
    
    while ((tmpInput = currentStack->pop()) != NULL) {
      outputTree->insert(tmpInput, (uint64_t)tmpInput->inputBias);
    }
    
    currentStack = network->layers->atIndex(network->layers->getSize()-1);
    
    while ((tmpInput = currentStack->pop()) != NULL) {
      outputTree->insert(tmpInput, (uint64_t)tmpInput->inputBias);
    }
    
    previousStack = this->layers->atIndex(this->layers->getSize()-1);
    outputSize = previousStack->getSize();
    
    /* add new network outputs to this one */
    
    mergeeLayer = network->layers->pop();
    
    while ((tmpInput = mergeeLayer->pop()) != NULL) {
      previousStack->push(tmpInput);
    }
    
    /*currentStack = new Collection::Stack<Neuron *>(this->layers->at(this->layers->size()-1));
     
     for (uint64_t jx = 0; jx < layers->at(layers->size()-1); jx++) {
     currentNeuron = new Neuron(this, previousStack);
     start = currentNeuron;  // Output Neuron
     this->add(currentNeuron);
     currentStack->push(currentNeuron);
     }*/
    
    /*
     this->layers->push(currentStack);
     expectation = new std::vector<double>();
     expectation->resize(layers->at(layers->size()-1));
     */
    
  }
  
  NeuralNetwork *split() {
    return NULL;
  }
  
  NeuralNetwork *clone(double errorRate) {
    return NULL;
  }
  
}
