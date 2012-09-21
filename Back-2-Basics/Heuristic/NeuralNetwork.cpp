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


namespace NeuralNetwork
{
  
#define RANDOM_INFLUENCE ((random() > random()) ? 1.0 : -1.0)
#define MAX_LAYERS    7   // 3 for space 7 for oct math and 10 for m-theory
  
  double zeroInfluence = 0.0;
  
  
  Synapse::Synapse(Neuron *neuron, Neuron *input, double influence) {
    this->u = neuron;
    this->v = input;
    
    this->capacity = capacity;
    lastCorrection = 0.0;
  }
  
  void Synapse::changeInfluence(double influence) {
    this->capacity += influence;
    lastCorrection = influence;
  }
  
  double Synapse::getInfluence() {
    return this->capacity;
  }
  
  double Synapse::getMomentum() {
    return (INERTIA_DEFAULT * this->lastCorrection);
  }
  
  Neuron::Neuron(NeuralNetwork *network, Collection::Stack<Neuron *> *input) {
    this->forwardEdges = new Collection::Stack<Synapse *>();
    this->discovered = false;
    this->capacity = 0.0;
    
    Synapse *synapse;
    this->inputData = NULL;
    inputInfluence = 0.0;
    
    for (int ix = 0; ix < input->getSize(); ix++) {
      synapse = new Synapse(this, input->atIndex(ix), RANDOM_INFLUENCE);
      network->addEdge(synapse);
      this->addAdjacentEdge(synapse);
      
    }
    
    this->bias = 0.0;
    iteration = 0;
    delta = 0.0;
    
  }
  
  Neuron::Neuron(NeuralNetwork *network, double *inputData) {
    this->forwardEdges = new Collection::Stack<Synapse *>();
    this->discovered = false;
    this->capacity = 0.0;
    
    this->inputData = inputData;
    inputInfluence = RANDOM_INFLUENCE;
    
    this->bias = 0.0;
    iteration = 0;
    delta = 0.0;
    
  }
  
  double Neuron::probeActivation(uint64_t iteration) {
    
    if (this->iteration == iteration) {
      return memory;
    }
    
    double tmp = this->bias;
    
    if (inputData != NULL) {
      tmp += *inputData * inputInfluence;
    }
    
    // Get impulse from relavent connected neurons
    for (int ix = 0; ix < this->getNumEdges(); ix++) {
      tmp += this->getAdjacentNode(ix)->probeActivation(iteration) *
      this->getAdjacentEdge(ix)->getInfluence();
    }
    
    memory = (1.0 / (1.0 + exp(double((-1.0) * tmp))));
    
    this->iteration = iteration;
    
    this->delta = 0.0;
    this->biasDelta = 0.0;
    
    return memory;
  }
  
  double Neuron::getInputInfluence(uint64_t index) {
    
    if (index > this->getNumEdges()) {
      return inputInfluence;
    }
    return this->getAdjacentEdge(index)->getInfluence();
  }
  
  double Neuron::getInputMomentum(uint64_t index) {
    
    if (index > this->getNumEdges()) {
      return *inputData * inertia;
    }
    
    return this->getAdjacentEdge(index)->getMomentum();
  }
  
  void Neuron::addInputNeuron(NeuralNetwork *network, Neuron *input) {
    
    Synapse *nLink = new Synapse(this, input, RANDOM_INFLUENCE);
    
    network->addEdge(nLink);
    
    this->addAdjacentEdge(nLink);
    
  }
  
  void Neuron::changeInputInfluence(uint64_t index) {
    double correction;
    
    correction = LEARNING_RULE_DEFAULT * this->memory * this->delta;
    correction += this->getAdjacentEdge(index)->getMomentum();
    
    bias += LEARNING_RULE_DEFAULT * this->memory * this->biasDelta;
    
    if (index > this->getNumEdges()) {
      inputLastCorrection = correction;
      inputInfluence += this->delta;
    } else {
      this->getAdjacentEdge(index)->changeInfluence(correction);
    }
  }
  
  
  NeuralNetwork::NeuralNetwork(Collection::Array<double> *input) {
    uint64_t layers;
    uint64_t *inLayer;
    Collection::Stack<Neuron *> *currentStack, *previousStack;
    Neuron *currentNeuron;
    
    
    layers = (rand() % MAX_LAYERS) + 1;
    inLayer = new uint64_t[layers];
    
    for (int jx = 0; jx < layers; jx++) {
      inLayer[jx] = (rand() % (input->getSize())) + 1;
    }
    
    
    this->layers = new Collection::Stack<Collection::Stack<Neuron *> *>(layers);
    
    
    currentStack = new Collection::Stack<Neuron *>(inLayer[0]);
    
    cout << "Creating New Input Layer:";
    cout << inLayer[0];
    cout << "\n";
    
    for (int jx = 0; jx < inLayer[0]; jx++) {
      //cout << "Creating Input Neuron\n";
      currentNeuron = new Neuron(this, input->ptrToIndex(jx));
      inputs.push(currentNeuron);
      this->add(currentNeuron);
      currentStack->push(currentNeuron);
      //cout << "  Inputs: ";
      //cout << currentNeuron->getNumEdges();
      //cout << "\n";
    }
    
    this->layers->push(currentStack);
    
    for (int ix = 1; ix < layers; ix++) {
      cout << "Creating New Hidden Layer:";
      cout << inLayer[ix];
      cout << "\n";
      previousStack = currentStack;
      currentStack = new Collection::Stack<Neuron *>(inLayer[ix]);
      
      for (int jx = 0; jx < inLayer[ix]; jx++) {
        //cout << "Creating Hidden Neuron\n";
        currentNeuron = new Neuron(this, previousStack);
        this->add(currentNeuron);
        currentStack->push(currentNeuron);
        //cout << "  Inputs: ";
        //cout << currentNeuron->getNumEdges();
        //cout << "\n";
      }
      this->layers->push(currentStack);
    }
    
    cout << "Creating Output Neuron\n";
    previousStack = currentStack;
    currentStack = new Collection::Stack<Neuron *>(1);
    
    currentNeuron = new Neuron(this, previousStack);
    start = currentNeuron;  // Output Neuron
    this->add(currentNeuron);
    currentStack->push(currentNeuron);
    
    cout << "  Inputs: ";
    cout << currentNeuron->getNumEdges();
    cout << "\n";
    
    this->layers->push(currentStack);
    
    currentIteration = 0;
  }
  
  void NeuralNetwork::calculateExpectation() {
    
    currentIteration++;
    
    /* Get response */
    expectation = start->probeActivation(currentIteration);
    
  }
  
  double NeuralNetwork::getExpectation() {
    return expectation;
  }
  
  void NeuralNetwork::doCorrection(double reality, double sensation) {
    
    Collection::Array<Neuron *> *currentLayer;
    Neuron *pCurrentNeuron;
    double nCurrentOutput;
    
    getLayers();
    
    if ((this->layers == NULL) || (this->layers->peek() == NULL)) {
      return;
    }
    
    currentLayer = this->layers->atIndex(this->layers->getSize()-1);
    
    pCurrentNeuron = currentLayer->atIndex(0);
    pCurrentNeuron->delta = (expectation *
                             (1.0 - expectation) *
                             (reality - expectation));
    pCurrentNeuron->biasDelta = -(((expectation - 0.5) * 2.0) *
                                  (sensation * pCurrentNeuron->getNumEdges()));
    
    for (int ix = 0; ix < pCurrentNeuron->getNumEdges(); ix++) {
      pCurrentNeuron->getAdjacentNode(ix)->delta += (pCurrentNeuron->getInputInfluence(ix) *
                                                     pCurrentNeuron->delta);
      pCurrentNeuron->getAdjacentNode(ix)->biasDelta += (pCurrentNeuron->getInputInfluence(ix) *
                                                         pCurrentNeuron->biasDelta);
    }
    
    for (uint64_t ix = 2; ix < this->layers->getSize(); ix++) {
      currentLayer = this->layers->atIndex(this->layers->getSize()-ix);
      for (int jx = 0; jx < currentLayer->getSize(); jx++) {
        nCurrentOutput = pCurrentNeuron->probeActivation(currentIteration);
        pCurrentNeuron = currentLayer->atIndex(jx);
        pCurrentNeuron->delta = (nCurrentOutput *
                                 (1.0 - nCurrentOutput) *
                                 pCurrentNeuron->delta);
        pCurrentNeuron->biasDelta = (((nCurrentOutput - 0.5) * 2.0) * pCurrentNeuron->biasDelta);
      }
    }
    
    for (uint64_t ix = 1; ix < this->layers->getSize(); ix++) {
      currentLayer = this->layers->atIndex(this->layers->getSize()-ix);
      for (int jx = 0; jx < currentLayer->getSize(); jx++) {
        for (int kx = 0; kx < currentLayer->atIndex(jx)->getNumEdges(); kx++) {
          currentLayer->atIndex(jx)->changeInputInfluence(kx);
        }
      }
    }
  }
  
  
  NeuralNetwork *clone(double errorRate) {
    return NULL;
  }
  
}
