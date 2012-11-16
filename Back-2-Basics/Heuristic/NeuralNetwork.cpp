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
  
  static double zero = 0.0;
  
  Synapse::Synapse(Neuron *input, Neuron *neuron) {
    this->u = neuron;
    this->v = input;
    
    input->references++;
    neuron->addEdge(this);
    this->influence = RANDOM_INFLUENCE;
    //this->capacity = (influence < 0) ? -influence : influence;
    lastCorrection = 0.0;
  }
  
  void Synapse::changeInfluence(double correction) {
    this->influence += correction;
    //this->capacity = (this->influence < 0) ? -this->influence : this->influence;
    lastCorrection = correction;
  }
  
  void Synapse::multInfluence(double influence) {
    this->influence *= influence;
    //this->capacity = (this->influence < 0) ? -this->influence : this->influence;
    lastCorrection *= influence;
  }
  
  double Synapse::getInfluence() {
    return influence;
  }
  
  double Synapse::getMomentum() {
    return (INERTIA_DEFAULT * this->lastCorrection);
  }
  
  Neuron::Neuron(LLRB_Tree<Harmony *, uint64_t> *layer, vector<Neuron *> *input, double *inputData, double *expectation) {
    Synapse *synapse;
    
    this->capacity = 0.0;
    
    if (inputData != NULL) {
      this->ptrInput = inputData;
    } else {
      this->ptrInput = new double();
      *(this->ptrInput) = 0.0;
    }
    
    if (expectation == NULL) {
      memory = new double();
    } else {
      memory = expectation;
    }
    
    if (input != NULL) {
      for (int ix = 0; ix < input->size(); ix++) {
        synapse = new Synapse(input->at(ix), this);
      }
    }
    
    if (layer != NULL) {
      layer->modifyAll(Neuron::addInputsEach, this);
    }
    
    iteration = 0;
    delta = 0.0;
    
  }
  
  double Neuron::probeActivation(uint64_t iteration) {
    
    if (this->iteration == iteration) {
      return *memory;
    }
     
    *memory = *ptrInput;
        
    // Get impulse from relavent connected neurons
    modifyAllAdjacent(Neuron::probeActivationEach, this);
    
    totalInputs = *memory;
    
    *memory = (1.0 / (1.0 + exp(double((-1.0) * totalInputs))));
    
    this->iteration = iteration;
    
    return *memory;
  }
  
  /*
  void Neuron::changeInputInfluence() {
    double correction;
    
    correction = LEARNING_RULE_DEFAULT * inputInfluenceDelta * totalInputs;
    correction += inputLastCorrection * inertia;
    inputLastCorrection = correction;
    inputInfluence += correction;
  }
   */
  
  NeuralNetwork::NeuralNetwork() {
  }
  
  NeuralNetwork::NeuralNetwork(std::vector<double *> *input,
                               std::vector<double *> *output,
                               std::vector<double *> *expectation,
                               std::vector<uint64_t> *layers) {
    
    vector<Neuron *> *currentStack, *previousStack;
    Neuron *currentNeuron;
    
    hiddenInfo = layers;
    bias = new Neuron(NULL,NULL,&networkBias,NULL);
    
    previousStack = new vector<Neuron *>();
    
    currentStack = new vector<Neuron *>();
    currentStack->reserve(input->size());
    
    for (int jx = 0; jx < input->size(); jx++) {
      currentNeuron = new Neuron(NULL, NULL, input->at(jx), NULL);
      inputs.insert(currentNeuron, (uint64_t)input->at(jx));
      currentStack->push_back(currentNeuron);
    }
    
    currentStack->push_back(bias);
    
    for (int ix = 0; ix < layers->size(); ix++) {
      previousStack->resize(0);
      delete previousStack;
      previousStack = currentStack;
      currentStack = new vector<Neuron *>();
      currentStack->reserve(layers->at(ix));
      
      for (uint64_t jx = 0; jx < layers->at(ix); jx++) {
        currentNeuron = new Neuron(NULL, previousStack, &zero, NULL);
        currentStack->push_back(currentNeuron);
      }
      currentStack->push_back(bias);
    }
    
    previousStack->resize(0);
    delete previousStack;
    previousStack = currentStack;
    currentStack = new vector<Neuron *>();
    currentStack->reserve(output->size());
    
    for (int ix = 0; ix < output->size(); ix++) {
      currentNeuron = new Neuron(NULL, previousStack, &zero, expectation->at(ix));
      Harmony *tmp = new Harmony;
      tmp->expectation = currentNeuron;
      tmp->reality = output->at(ix);
      outputs.insert(tmp, (uint64_t)(output->at(ix)));
      currentStack->push_back(currentNeuron);
    }
    
    previousStack->resize(0);
    currentStack->resize(0);
    delete previousStack;
    delete currentStack;
  }
  
  void NeuralNetwork::calcExpectation(uint64_t time) {
    
    currentIteration = time;
    
    experiencedEpochs++;
    
    outputs.modifyAll(NeuralNetwork::calcExpectationEach, (void *)currentIteration);
  }
  
  void NeuralNetwork::doCorrection() {
    
    outputs.modifyAll(NeuralNetwork::doCorrectionEach, 0);
    
    outputs.modifyAll(NeuralNetwork::changeInputInfluenceEach, 0);
    
  }
  
  void NeuralNetwork::simplify() {
    vector<Neuron *> *currentLayer;
    Neuron *pCurrentNeuron;
    
    /* Eliminate synapses that never carry significant charge */
    /* Count output synapses */
    
    for (uint64_t kx = 0; kx < this->layers->size()-1; kx++) {
      currentLayer = this->layers->at(kx);
      
      for (int jx = 0; jx < currentLayer->size(); jx++) {
        currentLayer->at(jx)->references = 0;
      }
    }
    
    currentLayer = this->layers->at(this->layers->size()-1);
    
    for (uint64_t jx = 0; jx < currentLayer->size(); jx++) {
      currentLayer->at(jx)->references = 1;
    }
    
    for (uint64_t kx = 1; kx < this->layers->size(); kx++) {
      currentLayer = this->layers->at(this->layers->size()-kx);
      
      for (uint64_t jx = 0; jx < currentLayer->size(); jx++) {
        pCurrentNeuron = currentLayer->at(jx);
        
        /* Get rid of irrelevant synapses */
        pCurrentNeuron->modifyAllAdjacent(Neuron::pruneEdgesEach, 0);
      }
    }
    
  }
  
  
  void NeuralNetwork::merge(NeuralNetwork *network) {
    
    vector<Neuron *> *currentStack, *previousStack, *mergeeLayer;
    LLRB_Tree<Neuron *, uint64_t> *outputTree = new LLRB_Tree<Neuron *, uint64_t>();
    
    Neuron *tmpInput;
    uint64_t outputSize;
    
    network->inputs.modifyAll(NeuralNetwork::addEach, this);
    
    currentStack = this->layers->at(network->layers->size()-1);
    
    while ((tmpInput = currentStack->back()) != NULL) {
      currentStack->resize(currentStack->size()-1);
      outputTree->insert(tmpInput, (uint64_t)tmpInput->ptrInput);
    }
    
    currentStack = network->layers->at(network->layers->size()-1);
    
    while ((tmpInput = currentStack->back()) != NULL) {
      currentStack->resize(currentStack->size()-1);
      outputTree->insert(tmpInput, (uint64_t)tmpInput->ptrInput);
    }
    
    previousStack = this->layers->at(this->layers->size()-1);
    outputSize = previousStack->size();
    
    /* add new network outputs to this one */
    
    mergeeLayer = network->layers->back();
    network->layers->resize(network->layers->size()-1);
    
    while ((tmpInput = mergeeLayer->back()) != NULL) {
      mergeeLayer->resize(mergeeLayer->size()-1);
      previousStack->push_back(tmpInput);
    }
    
  }
  
  NeuralNetwork *split() {
    return NULL;
  }
  
  NeuralNetwork *clone(double errorRate) {
    return NULL;
  }
  
  vector<double *> *NeuralNetwork::getInputs() {
    vector<double *> *ret = new vector<double *>();
    
    inputs.modifyAll(NeuralNetwork::addInputToVectorEach, ret);
    
    return ret;
  }
  
  vector<uint64_t> *NeuralNetwork::getHiddenInfo() {
    return hiddenInfo;
  }
  
  vector<HeuristicHarmony *> *NeuralNetwork::getHarmony() {
    vector<HeuristicHarmony *> *ret = new vector<HeuristicHarmony *>();
    
    outputs.modifyAll(NeuralNetwork::addOutputToVectorEach, ret);
    
    return ret;
  }
  
  void NeuralNetwork::removeOutput(double *output) {
    outputs.remove((uint64_t)output);
  }
  
  uint64_t NeuralNetwork::timeAlive() {
    return experiencedEpochs;
  }
  
}
