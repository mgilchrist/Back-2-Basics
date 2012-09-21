//
//  Genetic.h
//  OpenSource
//
//  Created by Marcus Gilchrist on 9/16/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#ifndef OpenSource_Genetic_h
#define OpenSource_Genetic_h

#include "Stochastic.h"

class Genetic : public Stoichastic<Genetic> {
  
private:
  
  double envToughness;
  uint64_t reproductionAgeMin;
  uint64_t reproductionAgeMax;
  
  void determineSurvival();
  void reproduce();
  void getNextGeneration();
  
public:
  Genetic();
  
  
};


void Genetic::determineSurvival() {
  
}

void Genetic::reproduce() {
  
}

void Genetic::getNextGeneration() {
  
}

Genetic::Genetic() {
  
}


#endif
