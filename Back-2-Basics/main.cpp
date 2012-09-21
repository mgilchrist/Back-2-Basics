//
//  main.cpp
//  OpenSource
//
//  Created by Marcus Gilchrist on 9/12/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#include <iostream>
using namespace std;


#include "Array.h"
#include "Graph.h"
#include "NeuralNetwork.h"
#include "Genetic.h"
#include "Metaheuristic.h"

#define INPUT_SIZE    32
#define OUTPUT_SIZE   32
#define ITERATIONS    (1024)


int main(int argc, const char * argv[])
{
  return 0;
}


int testHeap() {
  return 0;
}

int testStack() {
  return 0;
}

int testLinkedList() {
  return 0;
}

int testArrayList() {
  return 0;
}

int testNeuralNetwork() {
  return 0;
}

int testMetaheuristic() {
  
  Collection::Array<double> *input = new Collection::Array<double>(INPUT_SIZE);
  double *reality = new double[OUTPUT_SIZE];
  double expectation;
  uint64_t iterations = 0;
  
  Metaheuristic<Genetic,NeuralNetwork::NeuralNetwork,double> *masterMind = new Metaheuristic<Genetic,NeuralNetwork::NeuralNetwork, double>(input);
  
  cout << "Training\n";
  do {
    for (int ix = 0; ix < 1 /*OUTPUT_SIZE*/; ix++) {
      for (int jx = 0; jx < input->getSize(); jx++) {
        input->setIndex(jx, 0.1 * (random()%10));
      }
      expectation = masterMind->getConsensus();
      reality[ix] = 1.0;
      /*if ((input->atIndex(ix%INPUT_SIZE)) +
       (input->atIndex((ix+1)%INPUT_SIZE)) +
       (input->atIndex((ix+2)%INPUT_SIZE)) > 2.5) {
       reality[ix] = 1.0;
       } else if ((input->atIndex(ix%INPUT_SIZE)) +
       (input->atIndex((ix+1)%INPUT_SIZE)) +
       (input->atIndex((ix+2)%INPUT_SIZE)) < 0.5) {
       reality[ix] = 0.0;
       }*/
      masterMind->postResult(reality[ix]);
      cout << "Expecting ";
      cout << expectation;
      cout << ", Reality: ";
      cout << reality[ix];
      cout << "\n";
    }
    iterations++;
  } while (iterations < ITERATIONS);
  
  delete input;
  delete masterMind;
  
  return 0;
}
