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
  
#define RANDOM_INFLUENCE ((random() > random()) ? 1.0 : -1.0)
#define MAX_LAYERS    7   // 3 for space 7 for oct math and 10 for m-theory
  
  static const double zeroInfluence = 0.0;
  
  
  Synapse::Synapse(Neuron *neuron, Neuron *input, double influence) {
    this->u = neuron;
    this->v = input;
    this->influence = influence;
    this->capacity = (influence < 0) ? -influence : influence;
    lastCorrection = 0.0;
  }
  
  void Synapse::changeInfluence(double influence) {
    this->influence += influence;
    this->capacity = (this->influence < 0) ? -this->influence : this->influence;
    lastCorrection = influence;
  }
  
  double Synapse::getInfluence() {
    return this->influence;
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
      network->addEdgeObj(synapse);
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
    
    network->addEdgeObj(nLink);
    
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
  
  NeuralNetwork::NeuralNetwork(std::vector<double *> *input, std::vector<uint64_t> *layers) {
    
    Collection::Stack<Neuron *> *currentStack, *previousStack;
    Neuron *currentNeuron;
    
    
    this->layers = new Collection::Stack<Collection::Stack<Neuron *> *>(layers->size());
    
    
    currentStack = new Collection::Stack<Neuron *>(input->size());
    
#ifdef NEURALNETWORK_DEBUG
    cout << "Creating New Input Layer:";
    cout << inLayer[0];
    cout << "\n";
#endif
    
    for (int jx = 0; jx < input->size(); jx++) {
      //cout << "Creating Input Neuron\n";
      currentNeuron = new Neuron(this, input->at(jx));
      inputs.push(currentNeuron);
      this->add(currentNeuron);
      currentStack->push(currentNeuron);
      //cout << "  Inputs: ";
      //cout << currentNeuron->getNumEdges();
      //cout << "\n";
    }
    
    this->layers->push(currentStack);
    
    for (int ix = 0; ix < layers->size()-1; ix++) {
#ifdef NEURALNETWORK_DEBUG
      cout << "Creating New Hidden Layer:";
      cout << inLayer[ix];
      cout << "\n";
#endif
      previousStack = currentStack;
      currentStack = new Collection::Stack<Neuron *>(layers->at(ix));
      
      for (int jx = 0; jx < layers->at(ix); jx++) {
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
#ifdef NEURALNETWORK_DEBUG
    cout << "Creating Output Neuron\n";
#endif
    previousStack = currentStack;
    currentStack = new Collection::Stack<Neuron *>(layers->at(layers->size()-1));
    
    for (int jx = 0; jx < layers->at(layers->size()-1); jx++) {
      currentNeuron = new Neuron(this, previousStack);
      start = currentNeuron;  // Output Neuron
      this->add(currentNeuron);
      currentStack->push(currentNeuron);
    }
    
#ifdef NEURALNETWORK_DEBUG
    cout << "  Inputs: ";
    cout << currentNeuron->getNumEdges();
    cout << "\n";
#endif
    
    this->layers->push(currentStack);
    expectation = new std::vector<double>();
    expectation->resize(layers->at(layers->size()-1));
    
    currentIteration = 0;
  }
  
  void NeuralNetwork::calculateExpectation() {
    
    currentIteration++;
    
    Collection::Stack<Neuron *> *outputLayer = this->layers->atIndex(this->layers->getSize()-1);
    
    /* Get response */
    for (uint64_t ix = 0; ix < expectation->size(); ix++) {
      expectation->at(ix) = outputLayer->atIndex(ix)->probeActivation(currentIteration);
      
    }
  }
  
  std::vector<double> *NeuralNetwork::getExpectation() {
    return expectation;
  }
  
  void NeuralNetwork::doCorrection(double *reality, double sensation) {
    
    Collection::Stack<Neuron *> *currentLayer;
    Neuron *pCurrentNeuron;
    double nCurrentOutput;
    
    getLayers();
    
    if ((this->layers == NULL) || (this->layers->peek() == NULL)) {
      return;
    }
    
    currentLayer = this->layers->atIndex(this->layers->getSize()-1);
    
    for (int jx = 0; jx < currentLayer->getSize(); jx++) {
      pCurrentNeuron = currentLayer->atIndex(jx);
      pCurrentNeuron->delta = (expectation->at(jx) *
                               (1.0 - expectation->at(jx)) *
                               (reality[jx] - expectation->at(jx)));
      pCurrentNeuron->biasDelta = 0; //(pCurrentNeuron->delta * (sensation * pCurrentNeuron->getNumEdges()));
      
      for (int ix = 0; ix < pCurrentNeuron->getNumEdges(); ix++) {
        pCurrentNeuron->getAdjacentNode(ix)->delta += (pCurrentNeuron->getInputInfluence(ix) *
                                                       pCurrentNeuron->delta);
        pCurrentNeuron->getAdjacentNode(ix)->biasDelta += (pCurrentNeuron->getInputInfluence(ix) *
                                                           pCurrentNeuron->biasDelta);
      }
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
  
  void NeuralNetwork::prune() {
    Collection::Array<Neuron *> *currentLayer;
    Neuron *pCurrentNeuron;
    terminal = new Neuron(this, (double *)NULL);
    std::vector<double> *flow;
    
    for (int ix = 0; ix < this->inputs.getSize(); ix++) {
      this->inputs.atIndex(ix)->addInputNeuron(this, terminal);
    }
    
    flow = this->getMaximumFlow();
    
    for (uint64_t ix = 1; ix < this->layers->getSize(); ix++) {
      currentLayer = this->layers->atIndex(this->layers->getSize()-ix);
      for (int jx = 0; jx < currentLayer->getSize(); jx++) {
        pCurrentNeuron = currentLayer->atIndex(jx);
        for (int kx = 0; kx < pCurrentNeuron->getNumEdges(); kx++) {
          if (flow->at(pCurrentNeuron->getAdjacentEdge(kx)->getIndex()) == 0) {
            pCurrentNeuron->getAdjacentEdge(kx)->blocked = true;
          } else {
            pCurrentNeuron->getAdjacentEdge(kx)->blocked = false;
          }
        }
        
        pCurrentNeuron->cleanUp();
      }
    }
    
    
  }
  
  void NeuralNetwork::merge(NeuralNetwork *network) {
    
    
  }
  
  NeuralNetwork *clone(double errorRate) {
    return NULL;
  }
  
}
