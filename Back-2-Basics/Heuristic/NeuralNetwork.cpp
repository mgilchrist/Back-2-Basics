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
  
  Axion::Axion() {
    
  }
  
  void Axion::initialize(Neuron *input, Neuron *neuron, Info *info)
  {
    Pipe::initialize(input, neuron, RANDOM_INFLUENCE);
    
    mFlux = 0.0;
    
    adjacency = info;
  }
  
  void Axion::changeInfluence(double correction)
  {
    this->setCapacity(this->capacity() + (correction * glbElecConductivity) + mFlux);
    powerDissipation = pow(correction, 2) * mFlux;
    mFlux = (correction * glbElecConductivity) + (glbMagConductivity * mFlux);
  }
  
  void Neuron::initialize(double *inputData, double *expectation, NeuralNetwork *neuralNetwork)
  {
    Hub::initialize();
    
    nnetwork = neuralNetwork;
    
    if (inputData != NULL)
    {
      this->ptrInput = inputData;
    }
    else
    {
      this->ptrInput = new double();
      *(this->ptrInput) = 0.0;
    }
    
    if (expectation == NULL)
    {
      memory = new double();
    }
    else
    {
      memory = expectation;
    }
    
    iteration = 0;
    delta = 0.0;
    
    neuralNetwork->numberOfNeurons++;
    nnetwork = neuralNetwork;
  }
  
  void Neuron::deinitialize()
  {
    nnetwork->numberOfNeurons--;
  }
  
  double Neuron::probeActivation(uint64_t iteration)
  {
    if (this->iteration == iteration)
    {
      return *memory;
    }
    
    *memory = *ptrInput;
    
    if (!this->references)
    {
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
  
  void NeuralNetwork::deinitialize()
  {
    for (uint64_t ix = 0; ix < outputs.size(); ix++)
    {
      if (outputs[ix] != NULL)
      {
        delete outputs[ix];
        outputs[ix] = NULL;
      }
    }
    
    assert(!numberOfNeurons);
  }
  
  void NeuralNetwork::initialize(std::vector<double *> *input,
                               std::vector<Trust<double> *> *output,
                               LLRB_Tree<double *, uint64_t> *expectation,
                               std::vector<Info *> *connectivity,
                               uint64_t maxHiddenWidth)
  {
    
    Neuron *currentNeuron;
    Neuron **neuronArray[8];
    Axion *tmpAxion;
    
    this->hiddenWidth = maxHiddenWidth;
    
    for (uint64_t ix = 1; ix < 7; ix++)
    {
      neuronArray[ix] = new Neuron*[maxHiddenWidth];
      for (uint64_t jx = 0; jx < maxHiddenWidth; jx++)
      {
        neuronArray[ix][jx] = NULL;
      }
    }
    
    neuronArray[0] = new Neuron*[input->size()];
    
    for (uint64_t jx = 0; jx < input->size(); jx++)
    {
      neuronArray[0][jx] = NULL;
    }
    
    neuronArray[7] = new Neuron*[output->size()];
    
    for (uint64_t jx = 0; jx < output->size(); jx++)
    {
      neuronArray[7][jx] = NULL;
    }
    
    bias = new Neuron(&networkBias,NULL,this);
    
    
    for (int64_t ix = connectivity->size()-1; ix >= 0; ix--)
    {
      Info *conn = connectivity->at(ix);
      uint8_t inLayer = INFO_LAYER(conn->c.inputPosition);
      uint8_t outLayer = INFO_LAYER(conn->c.position);
      uint32_t inPosition = INFO_POSITION(conn->c.inputPosition);
      uint32_t outPosition = INFO_POSITION(conn->c.position);
      
      if (outLayer == 7)
      {
        uint32_t pos = outPosition;
        double *reality = output->at(pos)->actual;
        double *expect = expectation->search((uint64_t)reality);
        currentNeuron = new Neuron(&zero, expect, this);
        
        Harmony<Neuron> *tmp = new Harmony<Neuron>;
        tmp->logicElement = currentNeuron;
        tmp->expectation = expect;
        tmp->reality = reality;
        outputs.push_back(tmp);
        neuronArray[7][pos] = currentNeuron;
        
        tmpAxion = new Axion(bias, currentNeuron, NULL);
        
        neuronArray[7][pos]->references = 1;
      }
      
      if (neuronArray[outLayer][outPosition] == NULL)
      {
        /* there's no output neuron */
        continue;
      }
      
      if (neuronArray[inLayer][inPosition] == NULL)
      {
        if (inLayer == 0)
        {
          currentNeuron = new Neuron(input->at(inPosition), NULL, this);
          this->inputs.push_back(currentNeuron);
        }
        else
        {
          currentNeuron = new Neuron(&zero, NULL, this);
        }
        neuronArray[inLayer][inPosition] = currentNeuron;
        tmpAxion = new Axion(bias, currentNeuron, NULL);
      }
                                       
      tmpAxion = new Axion(neuronArray[inLayer][inPosition],
                  neuronArray[outLayer][outPosition], conn);
      
      hiddenInfo.push_back(conn);
    }
    
    for (uint64_t ix = 0; ix < 8; ix++)
    {
      delete neuronArray[ix];
    }
    
  }
  
  void NeuralNetwork::calcExpectation(uint64_t time)
  {
    currentIteration = time;
    experiencedEpochs++;
    
    for (uint64_t ix = 0; ix < outputs.size(); ix++)
    {
      if ((outputs[ix] != NULL) && (outputs[ix]->logicElement != NULL))
      {
        outputs[ix]->logicElement->probeActivation((uint64_t)currentIteration);
      }
    }
  }
  
  void NeuralNetwork::doCorrection()
  {
    
    for (uint64_t ix = 0; ix < outputs.size(); ix++)
    {
      if ((outputs[ix] != NULL) && (outputs[ix]->logicElement != NULL))
      {
        doCorrectionEach(outputs[ix]);
      }
    }
    
    for (uint64_t ix = 0; ix < outputs.size(); ix++)
    {
      if ((outputs[ix] != NULL) && (outputs[ix]->logicElement != NULL))
      {
        changeInputInfluenceEach(outputs[ix]);
      }
    }
    
  }
  
  void NeuralNetwork::optimalPrune()
  {
    
    /* Eliminate synapses that never carry significant charge */
    /* Count output synapses */
    
    for (uint64_t kx = 0; kx < outputs.size(); kx++)
    {
      if ((outputs[kx] != NULL) && (outputs[kx]->logicElement != NULL))
      {
        Neuron *tmpNeuron = outputs[kx]->logicElement;
        tmpNeuron->forwardEdges->deletion(Neuron::optimalPruneEach, tmpNeuron);
        if ((!tmpNeuron->forwardEdges->size()) && (tmpNeuron->ptrInput == &zero))
        {
          delete outputs[kx]->logicElement;
          numberOfNeurons--;
          outputs[kx]->logicElement = NULL;
        }
      }
    }
    
  }
  
  void NeuralNetwork::probablisticPrune()
  {
    /* Eliminate synapses that never carry stable/significant charge */
    /* Count output synapses */
    
    for (uint64_t kx = 0; kx < outputs.size(); kx++)
    {
      outputs[kx]->logicElement->forwardEdges->deletion(Neuron::probablisticPruneEach, outputs[kx]->logicElement);
    }
    
  }
  
  vector<double *> *NeuralNetwork::getInputs()
  {
    vector<double *> *ret = new vector<double *>();
    
    for (uint64_t ix = 0; ix < inputs.size(); ix++)
    {
      ret->push_back(inputs[ix]->ptrInput);
    }
    
    return ret;
  }
  
  vector<Harmony<Neuron> *> *NeuralNetwork::getHarmony()
  {
    vector<Harmony<Neuron> *> *ret = new vector<Harmony<Neuron> *>();
    
    for (uint64_t ix = 0; ix < outputs.size(); ix++)
    {
      if ((outputs[ix] != NULL) &&
          (outputs[ix]->logicElement != NULL))
      {
        ret->push_back(outputs[ix]);
      }
    }
    
    return ret;
  }
  
  vector<Info *> *NeuralNetwork::getHiddenInfo()
  {
    vector<Info *> *ret = new vector<Info *>();
    
    for (uint64_t ix = 0; ix < hiddenInfo.size(); ix++)
    {
      if (hiddenInfo[ix] != NULL)
      {
        Info *tmp = new Info;
        Info *thisInfo = hiddenInfo[ix];
        tmp->c.inputPosition = thisInfo->c.inputPosition;
        tmp->c.position = thisInfo->c.position;
        ret->push_back(tmp);
      }
    }
    
    return ret;
  }
  
  uint64_t NeuralNetwork::timeAlive()
  {
    return experiencedEpochs;
  }
  
}
