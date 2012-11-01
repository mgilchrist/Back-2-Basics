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
  
  
  class Synapse : public Graph::Pipe<Neuron> {
    
  private:
    double influence;
    double lastCorrection;
    
  public:
    
    Synapse(Neuron *neuron, Neuron *input, double influence);
    
    void changeInfluence(double influence);
    
    double getInfluence();
    double getMomentum();
  };
  
  class Neuron : public Graph::Hub<Neuron,Synapse> {
    
  protected:
    
    double *inputData;
    double inputInfluence = 0.0;
    double inputLastCorrection = 0.0;
    
    double memory = 0.0;
    double bias;
    uint64_t iteration = 0;
    double inertia = INERTIA_DEFAULT;
    
    SIG_FNCT_TYPE sigmoidFunctionType = NORMAL;
    
  public:
    
    double delta;
    double biasDelta;
    uint64_t outputCount = 0;
    
    Neuron(NeuralNetwork *,Collection::Stack<Neuron *> *);
    Neuron(NeuralNetwork *,double *);
    double getBias() {return bias;}
    void changeBias(double bias) {this->bias += bias;}
    double probeActivation(uint64_t iteration);
    double getInputInfluence(uint64_t);
    double getInputMomentum(uint64_t);
    void addInputNeuron(NeuralNetwork *,Neuron *);
    void changeInputInfluence(uint64_t);
    void colapseInputNeuron(uint64_t);
  };
  
  
  class NeuralNetwork : public Graph::Network<Neuron,Synapse> , public Heuristic<NeuralNetwork>
  {
  private:
    
    unsigned long currentIteration;
    Collection::Stack<Neuron *> inputs;
    Collection::Stack<Collection::Stack<Neuron *> *> *layers;
    bool layersDetermined = false;
    double glbBias = 0.5;
    double learningRule = LEARNING_RULE_DEFAULT;
    vector<double> *expectation;
    
  public:
    NeuralNetwork();
    NeuralNetwork(std::vector<double *> *,std::vector<uint64_t> *);
    NeuralNetwork *clone();
    
    void calculateExpectation(void);
    vector<double> *getExpectation(void);
    void doCorrection(double *,double);
    
    virtual void simplify();
    virtual void merge(NeuralNetwork *);
    
  };
  
}


#endif
