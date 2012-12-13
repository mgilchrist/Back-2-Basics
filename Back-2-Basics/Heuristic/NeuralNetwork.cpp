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
  
  Axion::Axion(Neuron *input, Neuron *neuron, Info **info) {
    this->u = neuron;
    this->v = input;
    
    input->references++;
    neuron->addEdge(this);
    this->capacity = RANDOM_INFLUENCE;
    mFlux = 0.0;
    
    adjacency = info;
  }
  
  void Axion::changeInfluence(double correction) {
    this->capacity += (correction * glbElecConductivity) + mFlux;
    powerDissipation = pow(correction, 2) * mFlux;
    mFlux = (correction * glbElecConductivity) + (glbMagConductivity * mFlux);
  }
  
  Neuron::Neuron(double *inputData, double *expectation) {
    
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
    
    iteration = 0;
    delta = 0.0;
    
  }
  
  double Neuron::probeActivation(uint64_t iteration) {
    
    if (this->iteration == iteration) {
      return *memory;
    }
    
    *memory = *ptrInput;
    
    if (!this->references) {
      /* killed neuron */
      return 0.0;
    }
    
    // Get impulse from relavent connected neurons
    modifyAllAdjacent(Neuron::probeActivationEach, this);
    
    totalInputs = *memory;
    
    *memory = (1.0 / (1.0 + exp(double((-1.0) * totalInputs))));
    
    this->iteration = iteration;
    
    return *memory;
  }
  
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
    
    bias = new Neuron(&networkBias,NULL);
    
    for (uint64_t ix = 0; ix < output->size(); ix++) {
      currentNeuron = new Neuron(&zero, expectation->at(ix));
      
      Harmony<Neuron> *tmp = new Harmony<Neuron>;
      tmp->logicElement = currentNeuron;
      tmp->expectation = expectation->at(ix);
      tmp->reality = output->at(ix);
      currentNeuron->references = 0;
      outputs.insert(tmp, (uint64_t)(output->at(ix)));
      neuronArray[7][ix] = currentNeuron;
      
      new Axion(bias, currentNeuron, NULL);
    }
    
    for (uint64_t ix = 0; ix < connectivity->size(); ix++) {
      Info *conn = connectivity->at(ix);
      
      if (conn->c.layer == 7) {
        neuronArray[7][conn->c.position]->references = 1;
      }
      
      if (neuronArray[conn->c.inputLayer][conn->c.inputPosition] == NULL) {
        currentNeuron = new Neuron(input->at(conn->c.inputPosition), NULL);
        if (conn->c.inputLayer == 0) {
          inputs.insert(currentNeuron, (uint64_t)input->at(conn->c.inputPosition));
        }
        neuronArray[conn->c.inputLayer][conn->c.inputPosition] = currentNeuron;
        new Axion(bias, currentNeuron, NULL);
      }
      
      if (neuronArray[conn->c.layer][conn->c.position] == NULL) {
        currentNeuron = new Neuron(&zero, NULL);
        neuronArray[conn->c.layer][conn->c.position] = currentNeuron;
        new Axion(bias, currentNeuron, NULL);
      }
      
      hiddenInfo.push_back(conn);
                                       
      new Axion(neuronArray[conn->c.inputLayer][conn->c.inputPosition],
                  neuronArray[conn->c.layer][conn->c.position], &(hiddenInfo[hiddenInfo.size()-1]));
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
      if (outputLayer->at(kx)->logicElement->references != 0) {
        Neuron *tmpNeuron = outputLayer->at(kx)->logicElement;
        tmpNeuron->forwardEdges.deletion(Neuron::optimalPruneEach, tmpNeuron);
        if (!tmpNeuron->forwardEdges.size()) {
          outputLayer->at(kx)->logicElement->references = 0;
        }
      }
    }
    
  }
  
  void NeuralNetwork::probablisticPrune() {
    vector<Harmony<Neuron> *> *outputLayer;
    
    /* Eliminate synapses that never carry stable/significant charge */
    /* Count output synapses */
    
    outputLayer = this->outputs.select(NULL, NULL);
    
    for (uint64_t kx = 0; kx < outputLayer->size(); kx++) {
      outputLayer->at(kx)->logicElement->forwardEdges.deletion(Neuron::probablisticPruneEach, outputLayer->at(kx)->logicElement);
    }
    
  }
  
  vector<double *> *NeuralNetwork::getInputs() {
    vector<double *> *ret = new vector<double *>();
    
    inputs.modifyAll(NeuralNetwork::addInputToVectorEach, ret);
    
    return ret;
  }
  
  vector<Harmony<Neuron> *> *NeuralNetwork::getHarmony() {
    return outputs.select(NULL, NULL);
  }
  
  vector<Info *> *NeuralNetwork::getHiddenInfo() {
    vector<Info *> *ret = new vector<Info *>();
    
    for (uint64_t ix = 0; ix < hiddenInfo.size(); ix++) {
      if (hiddenInfo[ix] != NULL) {
        Info *tmp = new Info;
        Info *thisInfo = hiddenInfo[ix];
        tmp->c.inputLayer = thisInfo->c.inputLayer;
        tmp->c.inputPosition = thisInfo->c.inputPosition;
        tmp->c.layer = thisInfo->c.layer;
        tmp->c.position = thisInfo->c.position;
        ret->push_back(tmp);
      }
    }
    
    return ret;
  }
  
  void NeuralNetwork::removeOutput(double *output) {
    outputs.remove(NULL, (uint64_t)output);
  }
  
  uint64_t NeuralNetwork::timeAlive() {
    return experiencedEpochs;
  }
  
}
