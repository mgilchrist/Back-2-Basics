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
  
  Neuron::Neuron(LLRB_Tree<Harmony<Neuron> *, uint64_t> *layer, vector<Neuron *> *input, double *inputData, double *expectation) {
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
                               std::vector<Info *> *connectivity) {
    
    Neuron *currentNeuron;
    Neuron **neuronArray[8];
    
    uint64_t maxHiddenWidth = max(input->size(), output->size());
    
    for (uint64_t ix = 0; ix < 8; ix++) {
      neuronArray[ix] = new Neuron*[maxHiddenWidth];
      for (uint64_t jx = 0; jx < maxHiddenWidth; jx++) {
        neuronArray[ix][jx] = NULL;
      }
    }
    
    hiddenInfo = new vector<Info *>();
    
    bias = new Neuron(NULL,NULL,&networkBias,NULL);
    
    for (uint64_t ix = 0; ix < output->size(); ix++) {
      currentNeuron = new Neuron(NULL, NULL, &zero, expectation->at(ix));
      
      Harmony<Neuron> *tmp = new Harmony<Neuron>;
      tmp->logicElement = currentNeuron;
      tmp->expectation = expectation->at(ix);
      tmp->reality = output->at(ix);
      outputs.insert(tmp, (uint64_t)(output->at(ix)));
      neuronArray[7][ix] = currentNeuron;
      
      new Synapse(bias, currentNeuron);
    }
    
    for (uint64_t ix = 0; ix < connectivity->size(); ix++) {
      Info *conn = connectivity->at(ix);
      
      hiddenInfo->push_back(conn);
      
      if (neuronArray[conn->c.inputLayer][conn->c.inputPosition] == NULL) {
        currentNeuron = new Neuron(NULL, NULL, input->at(conn->c.inputPosition), NULL);
        if (conn->c.inputLayer == 0) {
          inputs.insert(currentNeuron, (uint64_t)input->at(conn->c.inputPosition));
        }
        neuronArray[conn->c.inputLayer][conn->c.inputPosition] = currentNeuron;
        new Synapse(bias, currentNeuron);
      }
      
      if (neuronArray[conn->c.layer][conn->c.position] == NULL) {
        currentNeuron = new Neuron(NULL, NULL, &zero, NULL);
        neuronArray[conn->c.layer][conn->c.position] = currentNeuron;
        new Synapse(bias, currentNeuron);
      }
      
      new Synapse(neuronArray[conn->c.inputLayer][conn->c.inputPosition],
                  neuronArray[conn->c.layer][conn->c.position]);
    }
    
    for (uint64_t ix = 0; ix < 8; ix++) {
      delete neuronArray[ix];
    }
    
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
  
  void NeuralNetwork::optimalPrune() {
    vector<Harmony<Neuron> *> *outputLayer;
    
    /* Eliminate synapses that never carry significant charge */
    /* Count output synapses */
    
    outputLayer = this->outputs.select(NULL, NULL);
    
    for (uint64_t kx = 0; kx < outputLayer->size(); kx++) {
      outputLayer->at(kx)->logicElement->forwardEdges.removal(Neuron::optimalPruneEach, outputLayer->at(kx)->logicElement);
    }
    
    for (uint64_t kx = 0; kx < outputLayer->size(); kx++) {
      if (outputLayer->at(kx)->logicElement->forwardEdges.size() == 0) {
        *(outputLayer->at(kx)->logicElement->ptrInput) = -2.328306e10;
      }
    }
    
  }
  
  void NeuralNetwork::probablisticPrune() {
    vector<Harmony<Neuron> *> *outputLayer;
    
    /* Eliminate synapses that never carry stable/significant charge */
    /* Count output synapses */
    
    outputLayer = this->outputs.select(NULL, NULL);
    
    for (uint64_t kx = 0; kx < outputLayer->size(); kx++) {
      outputLayer->at(kx)->logicElement->forwardEdges.removal(Neuron::probablisticPruneEach, outputLayer->at(kx)->logicElement);
    }
    
    for (uint64_t kx = 0; kx < outputLayer->size(); kx++) {
      if (outputLayer->at(kx)->logicElement->forwardEdges.size() == 0) {
        *(outputLayer->at(kx)->logicElement->ptrInput) = -2.328306e10;
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
  
  vector<Info *> *NeuralNetwork::getHiddenInfo() {
    return hiddenInfo;
  }
  
  vector<Harmony<Neuron> *> *NeuralNetwork::getHarmony() {
    return outputs.select(NULL, NULL);
  }
  
  void NeuralNetwork::removeOutput(double *output) {
    outputs.remove(NULL, (uint64_t)output);
  }
  
  uint64_t NeuralNetwork::timeAlive() {
    return experiencedEpochs;
  }
  
}
