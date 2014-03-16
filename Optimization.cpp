//
//  Optimization.cpp
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 3/2/14.
//  Copyright (c) 2014 Marcus Gilchrist. All rights reserved.
//

#include "Optimization.h"


namespace Optimization {


Optimization::Optimization() {
  
}


void Optimization::addInput(int64_t *input) {
  question.insert(input, (uint64_t)input);
  
  spawnHiddenWidth = log2(max(question.size(),answer.size()));
  
  if (questionCache != NULL) {
    questionCache->resize(0);
    delete questionCache;
    questionCache = NULL;
  }
}


void Optimization::addOutput(int64_t *output) {
  Trust<int64_t> *trust = new Trust<int64_t>;
  
  trust->actual = output;
  
  answer.insert(trust, (uint64_t)output);
  
  spawnHiddenWidth = log2(max(question.size(),answer.size()));
  
  if (answerCache != NULL) {
    answerCache->resize(0);
    delete answerCache;
    answerCache = NULL;
  }
  
}


void Optimization::addInput(vector<int64_t *> *input) {
  for (uint64_t ix = 0; ix < input->size(); ix++) {
    question.insert(input->at(ix), (uint64_t)(input->at(ix)));
  }
  
  spawnHiddenWidth = log2(max(question.size(),answer.size()));
  
  if (questionCache != NULL) {
    questionCache->resize(0);
    delete questionCache;
    questionCache = NULL;
  }
  
}


void Optimization::addOutput(vector<int64_t *> *output) {
  Trust<int64_t> *trust;
  
  for (uint64_t ix = 0; ix < output->size(); ix++) {
    trust = new Trust<int64_t>;
    
    trust->actual = output->at(ix);
    
    answer.insert(trust, (uint64_t)output->at(ix));
  }
  
  spawnHiddenWidth = log2(max(question.size(), answer.size()));
  
  if (answerCache != NULL) {
    answerCache->resize(0);
    delete answerCache;
    answerCache = NULL;
  }
}


void Optimization::optimizeAnwser() {
  doEpoch();
}

}