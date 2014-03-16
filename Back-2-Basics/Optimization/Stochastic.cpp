//
//  Stochastic.cpp
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 3/2/14.
//  Copyright (c) 2014 Marcus Gilchrist. All rights reserved.
//

#include "Stochastic.h"


namespace Optimization {
  
  static int64_t glbEnergyCnst = 0;
  
void Stoichastic::doEpoch()
{
  iteration++;
  
  this->candidates.modifyAll(doEpochEach, &iteration);
  active.modifyAll(calcConsensus, NULL);
  
  doGeneration();
}


vector<int64_t *> *Stoichastic::pickRandoms(LLRB_Tree<int64_t *, uint64_t> *tree, vector<int64_t *> *originals, uint64_t requests)
{
  
  LLRB_TreeNode<int64_t *, uint64_t> *curr;
  LLRB_Tree<int64_t *, uint64_t> randoms;
  vector<LLRB_TreeNode<int64_t *, uint64_t> *> stack;
  uint64_t tmp;
  uint8_t leftDepth = 0, rightDepth = 0;
  int64_t pivot;
  
  curr = tree->treeRoot;
  
  while (curr != randoms.leftOf(curr)) {
    leftDepth++;
    curr = randoms.leftOf(curr);
  }
  
  curr = tree->treeRoot;
  
  while (curr != randoms.leftOf(curr)) {
    rightDepth++;
    curr = randoms.rightOf(curr);
  }
  
  pivot = (int64_t)leftDepth / ((int64_t)(leftDepth+rightDepth));
  
  if (originals != NULL) {
    for (uint64_t ix = 0; ix < originals->size(); ix++) {
      randoms.insert(originals->at(ix), (uint64_t)originals->at(ix));
    }
    
    originals->resize(0);
    delete originals;
  }
  
  for (uint64_t ix = 0; ix < requests; ix++) {
    curr = tree->treeRoot;
    
    while (curr != randoms.leftOf(curr)) {
      stack.push_back(curr);
      if ((rand() / (int64_t)RAND_MAX) < pivot) {
        curr = randoms.leftOf(curr);
      } else {
        curr = randoms.rightOf(curr);
      }
    }
    
    tmp = random() % (uint64_t)powl(2, stack.size());
    int64_t *entry = stack[log2l(tmp)]->data;
    randoms.insert(entry, (uint64_t)entry);
    stack.resize(0);
  }
  
  return randoms.select(NULL, NULL);
}


vector<Trust<int64_t> *> *Stoichastic::pickRandomTrusts(LLRB_Tree<Trust<int64_t> *, uint64_t> *tree, vector<Trust<int64_t> *> *originals, uint64_t requests) {
  
  LLRB_TreeNode<Trust<int64_t> *, uint64_t> *curr;
  LLRB_Tree<Trust<int64_t> *, uint64_t> randoms;
  vector<LLRB_TreeNode<Trust<int64_t> *, uint64_t> *> stack;
  uint64_t tmp;
  uint8_t leftDepth = 0, rightDepth = 0;
  int64_t pivot;
  
  curr = tree->treeRoot;
  
  while (curr != randoms.leftOf(curr)) {
    leftDepth++;
    curr = randoms.leftOf(curr);
  }
  
  curr = tree->treeRoot;
  
  while (curr != randoms.leftOf(curr)) {
    rightDepth++;
    curr = randoms.rightOf(curr);
  }
  
  pivot = (int64_t)leftDepth / ((int64_t)(leftDepth+rightDepth));
  
  if (originals != NULL) {
    for (uint64_t ix = 0; ix < originals->size(); ix++) {
      randoms.insert(originals->at(ix), (uint64_t)originals->at(ix));
    }
    
    originals->resize(0);
    delete originals;
    
  }
  
  for (uint64_t ix = 0; ix < requests; ix++) {
    curr = tree->treeRoot;
    
    while (curr != randoms.leftOf(curr)) {
      stack.push_back(curr);
      if ((rand() / (int64_t)RAND_MAX) < pivot) {
        curr = randoms.leftOf(curr);
      } else {
        curr = randoms.rightOf(curr);
      }
    }
    
    tmp = random() % (uint64_t)powl(2, stack.size());
    Trust<int64_t> *entry = stack[log2l(tmp)]->data;
    randoms.insert(entry, (uint64_t)entry);
    stack.resize(0);
  }
  
  return randoms.select(NULL, NULL);
}


Heuristic<> *Stoichastic::spawn(uint32_t spawnNum, uint32_t hiddenWidth) {
  vector<int64_t *> *inputEnv;
  LLRB_Tree<int64_t *, uint64_t> *expectation;
  vector<Trust<int64_t> *> *trusts;
  LLRB_Tree<Info *, Info> infoTree;
  vector<Info *> *hiddenInfo = new vector<Info *>();
  Heuristic<> *tmp;
  uint32_t layer, tmpNum;
  uint32_t incVal, inIncVal, inSOffset, sOffset;
  
  if (this->questionCache == NULL) {
    this->questionCache = this->question.select(NULL,NULL);
  }
  
  if (this->answerCache == NULL) {
    this->answerCache = this->answer.select(NULL,NULL);
  }
  
  inputEnv = this->questionCache;
  trusts = this->answerCache;
  
  inIncVal = (inputEnv->size() / (trusts->size() / log2(trusts->size())));
  inSOffset = inIncVal * spawnNum;
  incVal = log2(trusts->size());
  sOffset = incVal * spawnNum;
  
  expectation = new LLRB_Tree<int64_t *, uint64_t>();
  
  for (uint32_t ix = 0; ix < incVal; ix++)
  {
    tmpNum = (sOffset + ix) % trusts->size();
    int64_t *spawnExpect = new int64_t(0);
    expectation->insert(spawnExpect, (uint64_t)(trusts->at(tmpNum)->actual));
    
    if (trusts->at(tmpNum)->prediction == NULL)
    {
      trusts->at(tmpNum)->prediction = new Prediction<int64_t>();
    }
  }
  
  for (uint32_t ix = 0; ix < incVal; ix++)
  {
    layer = 7;
    tmpNum = (sOffset + ix) % trusts->size();
    
    while (layer != 0)
    {
      Info *newConn = new Info;
      newConn->c.layer = layer;
      newConn->c.position = tmpNum;
      layer -= log2((rand() % (1 << layer))) + 1;
      
      assert(layer < 7);
      
      if (layer)
      {
        tmpNum = rand() % hiddenWidth;
      }
      else
      {
        tmpNum = (inSOffset - (inSOffset/2) + (rand() % (inIncVal*2))) % inputEnv->size();
      }
      newConn->c.inputLayer = layer;
      newConn->c.inputPosition = tmpNum;
      
      infoTree.insert(newConn, *newConn);
    }
  }
  
  hiddenInfo = infoTree.select(NULL, NULL);
  
  tmp = this->createNewHeuristic(inputEnv,
                           trusts,
                           expectation,
                           hiddenInfo,
                           hiddenWidth);
  
  this->candidates.insert(tmp, (uint64_t)tmp);
  
  /* Estimate energy use */
  tmp->energy = (int64_t)hiddenInfo->size() * glbEnergyCnst;
  
  hiddenInfo->resize(0);
  
  delete expectation;
  delete hiddenInfo;
  
  return tmp;
}


void Stoichastic::initInternals()
{
  uint64_t numSpawn = (this->answer.size() / log2(this->answer.size())) + 1;
  
  for (uint32_t ix = 0; ix < numSpawn; ix++) {
    spawn(ix, this->spawnHiddenWidth);
  }
  
}
  
}