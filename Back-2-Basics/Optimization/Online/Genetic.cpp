//
//  Genetic.cpp
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 3/2/14.
//  Copyright (c) 2014 Marcus Gilchrist. All rights reserved.
//

#include "Genetic.h"

namespace Optimization {
  
  static int64_t glbEnergyCnst = 0;
  
void Genetic::doGeneration()
{
  vector<Heuristic<> *> *mates;
  
  evaluateFitness();
  checkSurvival();
  
  mates = this->candidates.select(NULL, NULL);
  
  for (uint64_t ix = 0; ix < mates->size(); ix++)
  {
    Heuristic<> *dad = mates->at(ix);
    Heuristic<> *mom = mates->at(rand() % mates->size());
    
    if ((dad->experiencedEpochs < dad->hiddenWidth) || (mom->experiencedEpochs < dad->hiddenWidth))
    {
      continue;
    }
    
    int64_t repoRate = 1.0 / (200.0 + dad->energy + mom->energy);
    int64_t rVal = rand() / (int64_t)RAND_MAX;
    
    repoRate *= this->error_rate;
    
    if (rVal < repoRate)
    {
      dad->optimalPrune();
      
      if (mom != dad)
      {
        mom->optimalPrune();
      }
      
      Heuristic<> *child = reproduce(dad, mom);
      
      this->candidates.insert(child, (uint64_t)child);
    }
  }
  
  //if ((rand() / (int64_t)RAND_MAX) < this->error_rate) {
  //this->spawn();
  //}
  
  doMeditation();
}


  
void Genetic::evaluateFitness()
{
  this->candidates.modifyAll(calcFitnessEach, this);
}


void Genetic::checkSurvival()
{
  this->candidates.removal(checkSurvivalEach, this);
}


void Genetic::doMeditation()
{
  this->candidates.modifyAll(meditateEach, 0);
}


vector<int64_t *> *Genetic::crossover(vector<int64_t *> *dad, vector<int64_t *> *mom)
{
  LLRB_Tree<int64_t *, uint64_t> treeRet;
  uint64_t tmp, crossover, mCrossover;
  uint64_t retSize = max(dad->size(), mom->size());
  uint64_t treeSizeStart;
  
  if (dad == mom)
  {
    return new vector<int64_t *>(dad->begin(), dad->end());
  }
  
  tmp = min((uint64_t)(random() % retSize), (uint64_t)dad->size());
  mCrossover = min((uint64_t)(retSize-tmp), (uint64_t)mom->size());
  crossover = retSize-mCrossover;
  
  /* Each generation should progress, so the size should be greater of
   * two rents
   */
  for (uint64_t mCurrent = 1; mCurrent <= mCrossover; mCurrent++)
  {
    treeRet.insert(mom->at(mom->size()-mCurrent), (uint64_t)mom->at(mom->size()-mCurrent));
  }
  
  treeSizeStart = treeRet.size();
  
  for (uint64_t dCurrent = 0; dCurrent < dad->size(); dCurrent++)
  {
    treeRet.insert(dad->at(dCurrent), (uint64_t)dad->at(dCurrent));
    
    if ((treeRet.size()-treeSizeStart) >= crossover)
    {
      break;
    }
  }
  
  return treeRet.select(NULL,NULL);
}


void Genetic::mutate(vector<Info *> *vect,
                     uint64_t outputWidth,
                     uint64_t inputWidth,
                     uint64_t hiddenWidth,
                     int64_t error_rate)
{
  uint64_t mutations = (int64_t)vect->size() * error_rate;
  uint32_t pos;
  Info *entry;
  uint64_t randVal = 0;
  
  for (uint64_t ix = 0; ix < mutations; ix++)
  {
    if (!randVal)
    {
      randVal = random();
    }
    entry = vect->at(rand()%vect->size());
    
    if (randVal & 0x01) {;
      pos = entry->c.position ^ (1 << (rand() % 32));
      
      if (entry->c.layer == 7)
      {
        entry->c.position = pos % outputWidth;
      }
      else
      {
        entry->c.position = pos % hiddenWidth;
      }
    }
    else
    {
      pos = entry->c.inputPosition ^ (1 << (rand() % 32));
      
      if (entry->c.inputLayer)
      {
        entry->c.inputPosition = pos % hiddenWidth;
      }
      else
      {
        entry->c.inputPosition = pos % inputWidth;
      }
    }
    assert(entry->c.inputLayer < 7);
    assert(entry->c.layer <= 7);
    randVal >>= 1;
  }
}


Heuristic<> *Genetic::reproduce(Heuristic<> *dad, Heuristic<> *mom)
{
  vector<int64_t *> *childInputs;
  vector<Trust<int64_t> *> *childTrusts;
  LLRB_Tree<int64_t *, uint64_t> childExpectations;
  vector<Info *> *childHiddenInfo, *tmpInfo;
  LLRB_Tree<Info *, Info> infoTree;
  Heuristic<> *tmpHeuristic;
  int64_t mutation_rate = this->error_rate;
  
  if (this->questionCache == NULL)
  {
    this->questionCache = this->question.select(NULL,NULL);
  }
  
  if (this->answerCache == NULL)
  {
    this->answerCache = this->answer.select(NULL,NULL);
  }
  
  childInputs = this->questionCache;
  childTrusts = this->answerCache;
  
  tmpInfo = dad->getHiddenInfo();
  
  for (uint64_t ix = 0; ix < tmpInfo->size(); ix++)
  {
    Info *thisInfo = tmpInfo->at(ix);
    infoTree.insert(thisInfo, *thisInfo);
  }
  
  tmpInfo = mom->getHiddenInfo();
  mutate(tmpInfo, childTrusts->size(), childInputs->size(),
         mom->hiddenWidth, mutation_rate);
  
  for (uint64_t ix = 0; ix < tmpInfo->size(); ix++)
  {
    Info *thisInfo = tmpInfo->at(ix);
    
    if (thisInfo->c.layer != 7)
    {
      thisInfo->c.position = thisInfo->c.position + dad->hiddenWidth;
    }
    
    if (thisInfo->c.inputLayer != 0)
    {
      thisInfo->c.inputPosition = thisInfo->c.inputPosition + dad->hiddenWidth;
    }
    
    infoTree.insert(thisInfo, *thisInfo);
  }
  
  childHiddenInfo = infoTree.select(NULL, NULL);
  
  for (uint64_t ix = 0; ix < childHiddenInfo->size(); ix++)
  {
    if (childHiddenInfo->at(ix)->c.layer == 7)
    {
      int64_t *reality = childTrusts->at(childHiddenInfo->at(ix)->c.position)->actual;
      int64_t *childExpect = new int64_t(0);
      childExpectations.insert(childExpect, (uint64_t)reality);
      
      if (childTrusts->at(ix)->prediction == NULL)
      {
        childTrusts->at(ix)->prediction = new Prediction<int64_t>();
      }
    }
  }
  
  tmpHeuristic = this->createNewHeuristic
  (
   childInputs,
   childTrusts,
   &childExpectations,
   childHiddenInfo,
   dad->hiddenWidth + mom->hiddenWidth
  );
  
  tmpHeuristic->persistance = dad->persistance + mom->persistance;
  tmpHeuristic->energy *= childHiddenInfo->size() * glbEnergyCnst;
  
  childHiddenInfo->resize(0);
  
  delete childHiddenInfo;
  
  return tmpHeuristic;
}


Genetic::Genetic()
{
  
}

}