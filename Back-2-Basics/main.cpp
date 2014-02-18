//
//  main.cpp
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 9/12/12.
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
#include <fstream>

#include "Array.h"
using namespace Collection;
#include "Heap.h"
#include "HashTable.h"
#include "SecureHashTable.h"
#include "Graph.h"
using namespace Graph;
#include "LLRB_Tree.h"
#include "Navigation.h"
#include "Network.h"
#include "NeuralNetwork.h"
#include "Genetic.h"
#include "Metaheuristic.h"
#include "ObjectSegmentation.h"


#define SUITE_NAME  "test"

#define BINARY_FILE ".bin"
#define TEXT_FILE ".txt"

const uint64_t glbInputSize = 0x1000;
const uint64_t glbOutputSize = 0x1000;
const uint64_t glbIterations = 0x10000;
const uint64_t glbTestSize = 0x100000;
const uint64_t glbSlowTestSize = 0x10000;


typedef struct testEntry
{
  const char  *name;
  int (*funct) (char **args, int nArgs, ifstream *inputStream, ofstream *outputData, ofstream *log);
  uint64_t    defaultBufferSize;
  uint8_t     nArgs;
  uint8_t     argSize[16];
} TEST_ENTRY;

typedef struct maxtrixConvInfo
{
  uint64_t matrixIncrement;
  uint64_t matrixOffset;
  uint64_t kernelIncrement;
  uint64_t kernelOffset;
} MAXTRIX_CONV_INFO;



int testGenetic(char **args, int nArgs, ifstream *inputStream, ofstream *outputData, ofstream *log);
void rKernelWork(vector<double> *matrix, uint64_t matrixOffset, std::vector<uint64_t> *mDimensions,
                               vector<pair<double, double>> *kernel, int64_t kernelStartOffset, uint64_t kernelOffset, std::vector<uint64_t> *kDimensions,
                               std::vector<uint64_t> *kCenter,
                               uint8_t dimension, vector<double> *ret);
void rArrayedMatrixTraverse(vector<double> *matrix, uint64_t matrixOffset, std::vector<uint64_t> *mDimensions,
                            vector<pair<double, double>> *kernel, std::vector<uint64_t> *kDimensions,
                            std::vector<uint64_t> *kCenter,
                            uint8_t dimension, vector<double> *ret);



TEST_ENTRY tests[256] =
{
  //{"genetic", testGenetic, 960*600, 0, {}},
  {"end", NULL, 0, 0, {}},
};


#define openFileStreams(suiteName, testName)  \
  do {  \
    char *iFile = new char [strlen( suiteName )+2 + strlen ( testName )+1 + 2 + strlen ( BINARY_FILE )+1];  \
    char *vFile = new char [strlen( suiteName )+2 + strlen ( testName )+1 + 2 + strlen ( BINARY_FILE )+1];  \
    char *oFile = new char [strlen( suiteName )+2 + strlen ( testName )+1 + 2 + strlen ( BINARY_FILE )+1];  \
    char *lFile = new char [strlen( suiteName )+2 + strlen ( testName )+1 + strlen ( TEXT_FILE )+1];        \
    strcpy (iFile, suiteName);    \
    strcat (iFile, "_");          \
    strcat (iFile, testName);     \
    strcat (iFile, "_i");         \
    strcat (iFile, BINARY_FILE);  \
    strcpy (iFile, suiteName);    \
    strcat (iFile, "_");          \
    strcat (iFile, testName);     \
    strcat (iFile, "_v");         \
    strcat (iFile, BINARY_FILE);  \
    strcpy (oFile, suiteName);    \
    strcat (oFile, "_");          \
    strcat (oFile, testName);     \
    strcat (oFile, "_o");         \
    strcat (oFile, BINARY_FILE);  \
    strcpy (lFile, suiteName);    \
    strcat (lFile, testName);     \
    strcat (lFile, TEXT_FILE);    \
    ifstream inputStream(iFile, ios::in|ios::binary|ios::ate);  \
    ifstream verifyStream(vFile, ios::in|ios::binary|ios::ate); \
    ofstream outputData(oFile, ios::out|ios::binary|ios::ate);  \
    ofstream log(lFile);  \
    delete iFile; \
    delete vFile; \
    delete oFile; \
    delete lFile; \
  } while ((0))

#define closeFileStreams()  \
  do {  \
    inputStream.close();  \
    outputData.close(); \
    log.close();  \
  } while ((0))


MAXTRIX_CONV_INFO *rMatrixConvolution(vector<double> *matrix, uint64_t matrixOffset, std::vector<uint64_t> *mDimensions,
                                             vector<pair<double, double>> *kernel, std::vector<uint64_t> *kDimensions,
                                             uint8_t dimension, vector<double> *ret);


TEST_ENTRY *lookupTest(const char *testName)
{
  return NULL;
}


int64_t addPattern(int64_t seed, uint64_t iterations)
{
  return seed + iterations;
}

int64_t randomPattern(int64_t seed, uint64_t iterations)
{
  return seed + (random() & 0x00FF);
}

int rModifyAndPrintMatrix(vector<int64_t> *array, vector<uint64_t> *dim, uint64_t offset,
                    uint8_t dimensions, int64_t (*pattern)(int64_t seed, uint64_t iter), uint64_t iter)
{
  cout << "{";
  
  if (dimensions > 1)
  {
    for (uint64_t ix = 0; ix < dim->at(dimensions - 1); ix++)
    {
      rModifyAndPrintMatrix(array, dim, offset + (ix * dim->at(dimensions - 2)), dimensions-1, pattern, iter);
    }
  }
  else
  {
    cout << ".";
    for (uint64_t ix = 0; ix < dim->at(0); ix++)
    {
      uint64_t entry = pattern(array->at(ix + offset), iter);
      cout << entry;
      cout << ".";
      array->at(ix + offset) = entry;
    }
  }
  
  cout << "}";
  
  return 0;
}

int64_t getRandomInBetween(int64_t first, int64_t second)
{
  uint64_t diff;
  int64_t ret;
  
  if (first == second)
  {
    return first;
  }
  
  if (first > second)
  {
    diff = first - second;
    ret = second + (random() % diff);
  }
  else
  {
    diff = second - first;
    ret = first + (random() % diff);
  }
  
  return ret;
}

void rKernelWork(vector<int64_t> *matrix, uint64_t matrixOffset, vector<uint64_t> *mDimensions,
                               uint64_t retOffset,
                               vector<pair<int64_t, int64_t>> *kernel, uint64_t kernelOffset, vector<uint64_t> *kDimensions, vector<uint64_t> *kCenter,
                               uint8_t dimension, vector<int64_t> *ret)
{
  kernelOffset *= kDimensions->at(dimension-1);
  
  if (dimension > 1)
  {
    for (uint64_t kLoc = 0; kLoc < kDimensions->at(dimension-1); kLoc++)
    {
      int64_t modifier = ((kLoc - kCenter->at(dimension-1)) * mDimensions->at(dimension-2));
      int64_t compareOffset = matrixOffset + modifier;
      
      if ((compareOffset < 0) ||
          (compareOffset >= matrix->size()))
      {
        continue;
      }
      
      rKernelWork(matrix, compareOffset, mDimensions,
                  retOffset,
                  kernel, kernelOffset + kLoc, kDimensions, kCenter,
                  dimension-1, ret);
    }
  }
  else
  {
    for (uint64_t kLoc = 0; kLoc < kDimensions->at(0); kLoc++)
    {
      int64_t thisDimOffset = (matrixOffset % mDimensions->at(0)) + (kLoc - kCenter->at(0));
      
      if (( thisDimOffset >= mDimensions->at(0) ) ||
          ( thisDimOffset < 0 ))
      {
        continue;
      }
      
      uint64_t currentOffset = matrixOffset + (kLoc - kCenter->at(0));
      
      pair<double, double> currentKernelPair = kernel->at(kernelOffset+kLoc);
      double k = getRandomInBetween(currentKernelPair.first, currentKernelPair.second);
      
      ret->at(retOffset) += matrix->at(currentOffset) * k;
    }
  }
}

void rArrayedMatrixTraverse(vector<int64_t> *matrix, uint64_t matrixOffset, vector<uint64_t> *mDimensions,
                                             vector<pair<int64_t, int64_t>> *kernel, vector<uint64_t> *kDimensions, vector<uint64_t> *kCenter,
                                             uint8_t dimension, vector<int64_t> *ret)
{
  matrixOffset *= mDimensions->at(dimension-1);
  
  if (dimension > 1)
  {
    for (uint64_t mLoc = 0; mLoc < mDimensions->at(dimension-1); mLoc++)
    {
      rArrayedMatrixTraverse(matrix, matrixOffset + mLoc, mDimensions,
                             kernel, kDimensions, kCenter,
                             dimension-1, ret);
    }
  }
  else
  {
    for (uint64_t mLoc = 0; mLoc < mDimensions->at(dimension-1); mLoc++)
    {
      uint64_t arrayOffset = matrixOffset + mLoc;
      
      rKernelWork(matrix, arrayOffset, mDimensions,
                  arrayOffset,
                  kernel, 0, kDimensions, kCenter,
                  (uint8_t)(mDimensions->size()), ret);
    }
  }
}

vector<int64_t> *matrixConvolution(vector<int64_t> *matrix, vector<uint64_t> *mDimensions,
                                  vector<pair<int64_t, int64_t>> *kernel, vector<uint64_t> *kDimensions,
                                  vector<uint64_t> *kCenter)
{
  vector<int64_t> *ret = new vector<int64_t>();
  ret->resize(matrix->size());
  
  rArrayedMatrixTraverse(matrix, 0, mDimensions,
                         kernel, kDimensions, kCenter,
                         (uint8_t)(mDimensions->size()), ret);
  
  return ret;
  
}


int matrixConvolutionTest()
{
  vector<int64_t> matrix, kernel, *convMatrix;
  vector<uint64_t> mDim, kDim, kCenter;
  vector<pair<int64_t, int64_t>> kDimRange;
  uint64_t mSize = 1, kSize = 1;
  
  uint8_t dimension = 2;
  
  for (uint8_t ix = 0; ix < dimension; ix++)
  {
    mDim.push_back( 4 );
    mSize *= 4;
    kDim.push_back( 3 );
    kSize *= 3;
    kCenter.push_back( 1 );
  }
  
  matrix.resize(mSize);
  kernel.resize(kSize);
  
  cout << "\nCreating matrix\n";
  rModifyAndPrintMatrix(&matrix, &mDim, 0, dimension, randomPattern, 1);
  //cout << "\nCreating kernel\n";
  //rModifyAndPrintMatrix(&kernel, &kDim, 0, dimension, addPattern, 1);
  kernel[kernel.size()/2] = 1;
  cout << "\n";
  
  for (uint64_t ix = 0; ix < kernel.size(); ix++)
  {
    pair<uint64_t, uint64_t> thisPair;
    
    thisPair.first = kernel[ix];
    thisPair.second = kernel[ix];
    
    kDimRange.push_back( thisPair );
  }
  
  convMatrix = matrixConvolution(&matrix, &mDim, &kDimRange, &kDim, &kCenter);
  
  cout << "\nConvoluted matrix\n";
  rModifyAndPrintMatrix(convMatrix, &mDim, 0, dimension, addPattern, 0);
  cout << "\n";
  
  return 0;
}


int executeTest(const char *suiteName, const char *testName, char **args, int nArgs)
{
  ifstream inputStream;
  ofstream outputData;
  ofstream log;
  
  TEST_ENTRY *uut = lookupTest(testName);
  
  openFileStreams(suiteName, testName);
  
  uut->funct(args, nArgs, &inputStream, &outputData, &log);
  
  closeFileStreams();
  
  return 0;
}

int createDataFor(const char *testName, std::vector<uint64_t> *dimensions,
                  std::vector<uint64_t> *seed, std::vector<uint64_t> *pattern)
{
  uint64_t filePosition = 0;
  uint64_t sampleSize = sizeof(double);
  uint64_t duration = dimensions->at(dimensions->size()-1);
  
  char oFile[strlen( testName )+2 + 2 + strlen ( BINARY_FILE )+1];
  
  strcat (oFile, testName);
  strcat (oFile, "_o");
  strcat (oFile, BINARY_FILE);
  
  ofstream outputStream(oFile, ios::out|ios::binary|ios::ate);
  
  for (uint8_t ix = 0; ix < dimensions->size()-1; ix++)
  {
    sampleSize *= dimensions->at(ix);
  }
  
  char *outputData = new char[sampleSize];
  
  outputStream.seekp(0, ios::beg);
  
  for (uint64_t ix = 0; ix < duration; ix++)
  {
    filePosition += sampleSize;
    outputStream.seekp(filePosition, ios::beg);

    /* Create Data */
    
    
    outputStream.write(outputData, sampleSize);
  }
  
  outputStream.close();
  
  return 0;
}

int setupNeuralNetworkData(const char *suiteName, std::vector<uint64_t> *dimensions,
                           std::vector<uint64_t> *seed)
{
  const char *testName = "Neural_Network";
  uint64_t filePosition = 0;
  uint64_t sampleSize = sizeof(double);
  uint64_t duration = dimensions->at(dimensions->size()-1);
  
  char oFile[strlen( suiteName )+2 + strlen( testName )+2 + 2 + strlen ( BINARY_FILE )+1];
  
  strcat (oFile, testName);
  strcat (oFile, "_o");
  strcat (oFile, BINARY_FILE);
  
  ofstream outputStream(oFile, ios::out|ios::binary|ios::ate);
  
  for (uint8_t ix = 0; ix < dimensions->size()-1; ix++)
  {
    sampleSize *= dimensions->at(ix);
  }
  
  char *outputData = new char[sampleSize];
  
  outputStream.seekp(0, ios::beg);
  
  for (uint64_t ix = 0; ix < duration; ix++)
  {
    filePosition += sampleSize;
    outputStream.seekp(filePosition, ios::beg);
    
    //for (seed)
    
    outputStream.write(outputData, sampleSize);
  }
  
  outputStream.close();
  
  return 0;
  
}

int testSegmentation(const char *suiteName)
{
  ifstream inputStream;
  ofstream outputData;
  ofstream log;
  
  uint64_t inputFileSize;
  uint64_t sampleSize = 960 * 600 *sizeof(double);
  
  char *buffer0 = new char[sampleSize];
  char *buffer1 = new char[sampleSize];
  
  char *nextData = buffer0;
  char *inputData = buffer1;
  
  bool pingPong = false;
  
  NeuralNetwork::NeuralNetwork *graphToSegment;
  
  openFileStreams(suiteName, "_segmentation");
  
  Segmentation::ObjectSegmentation<NeuralNetwork::Neuron,NeuralNetwork::Axion> segmentor;
  Optimization::Genetic<NeuralNetwork::NeuralNetwork,NeuralNetwork::Neuron,double> geneticExp;
  vector<vector<NeuralNetwork::Neuron *> *> *objects;
  
  geneticExp.addInput((double *)inputData);
  geneticExp.addOutput((double *)nextData);
  
  geneticExp.initInternals();
  
  inputFileSize = inputStream.tellg();
  inputFileSize -= (inputFileSize % sampleSize);
  
  inputStream.seekg (0, ios::beg);
  inputStream.read(nextData, sampleSize);
  
  for (uint64_t pos = sampleSize; pos < inputFileSize; pos += sampleSize)
  {
    inputStream.seekg (pos, ios::beg);
    
    if (pingPong)
    {
      inputData = buffer0;
      nextData = buffer1;
    }
    else
    {
      inputData = buffer1;
      nextData = buffer0;
    }
    
    if (pos < inputFileSize)
    {
      inputStream.read(nextData, sampleSize);
    }
    else
    {
      break;
    }
    
    geneticExp.optimizeAnwser();
  }
  
  graphToSegment = geneticExp.candidates.min(geneticExp.candidates.treeRoot)->data;
  
  objects = segmentor.getNodeSegments(graphToSegment);
  
  closeFileStreams();
  
  return 0;
}

int testHashTable()
{
  HashTable<uint64_t *,uint64_t> *hashTable;
  std::vector<uint64_t> verify;
  hashTable = new HashTable<uint64_t *,uint64_t>(glbTestSize);
  uint64_t *value;
  
  cout << "\nTesting HashTable.\n";
  
  verify.resize(glbTestSize);
  
  for (int ix = 0; ix < glbTestSize; ix++)
  {
    
    do
    {
      value = new uint64_t(random());
    }
    while (!hashTable->get(*value));
    
    hashTable->insert(value, *value);
    
    verify[ix] = *value;
  }
  
  for (int ix = 0; ix < glbTestSize; ix++)
  {
    if (*hashTable->get(verify[ix]) != verify[ix])
    {
      hashTable->get(verify[ix]);
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
      //return -1;
    }
    
    //if ((ix % 4) == 3) {
    //  hashTable->remove(index);
    //}
  }
  
  for (int ix = 0; ix < glbTestSize; ix++)
  {
    if (*hashTable->get(verify[ix]) != verify[ix])
    {
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
      return -1;
    }
    
    hashTable->remove(verify[ix]);
    
    //if ((index % 4) != 3) {
    //  hashTable->remove(index);
    //}
  }
  
  delete hashTable;
  
  cout << "HashTable:Done\n";
  
  return 0;
}

int testSecureHashTable()
{
  SecureHashTable<uint64_t *,uint64_t> *hashTable;
  std::vector<uint64_t> verify;
  hashTable = new SecureHashTable<uint64_t *,uint64_t>(glbSlowTestSize);
  uint64_t *value;
  
  verify.resize(glbSlowTestSize);
  
  cout << "\nTesting SecureHashTable.\n";
  
  for (uint64_t ix = 0; ix < glbSlowTestSize; ix++)
  {
    do
    {
      value = new uint64_t(random());
    } while (!hashTable->get(*value));
    
    hashTable->insert(value, *value);
    
    verify[ix] = *value;
  }
  
  for (uint64_t ix = 0; ix < glbSlowTestSize; ix++)
  {
    if (*hashTable->get(verify[ix]) != verify[ix])
    {
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
    }
    
    //if ((ix % 4) == 3) {
    //  hashTable->remove(index);
    //}
  }
  
  for (uint64_t ix = 0; ix < glbSlowTestSize; ix++)
  {
    if (*hashTable->get(verify[ix]) != verify[ix])
    {
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
    }
    
    hashTable->remove(verify[ix]);
    
    //if ((index % 4) != 3) {
    //  hashTable->remove(index);
    //}
  }
  
  delete hashTable;
  
  cout << "SecureHashTable:Done\n";
  
  return 0;
}

int testHeap() {
  Heap<uint64_t,uint64_t> *heap;
  heap = new Heap<uint64_t,uint64_t>();
  
  uint64_t tmp, tmp1;
  
  cout << "\nTesting Heap.\n";
  
  for (int ix = 0; ix < glbTestSize; ix++)
  {
    uint64_t value = random();
    heap->push(value, value);
  }
  
  if (heap->at(0) != (tmp = heap->pop()))
  {
    cout << "Heap Error!\n";
  }
  
  for (int ix = 1; ix < glbTestSize; ix++)
  {
    if (heap->at(0) != (tmp1 = heap->pop()))
    {
      cout << "Index ";
      cout << ix;
      cout << " Error!\n";
    }
    
    if (tmp > tmp1)
    {
      cout << "Index ";
      cout << ix;
      cout << " lower than previous!\n";
    }
    
    tmp = tmp1;
  }
  
  cout << "Heap:Done.\n";
  
  return 0;
}


int testLLRBTree()
{
  LLRB_Tree<uint64_t,uint64_t> *rbTree;
  vector<uint64_t> *addList;
  LLRB_TreeNode<uint64_t,uint64_t> *current;
  uint64_t tmp;
  
  cout << "\nTesting LLRB_Tree\n";
  
  rbTree = new LLRB_Tree<uint64_t, uint64_t>();
  addList = new vector<uint64_t>();
  
  
  for (uint64_t iter = 0; iter < 4; iter++)
  {
    /* Add items to the tree */
    for (uint64_t ix = 0; ix < glbTestSize; ix++)
    {
      uint64_t value;
      do
      {
        value = random();
        if (!value)
        {
          continue;
        }
        tmp = rbTree->size();
        rbTree->insert(value, value);
      } while (rbTree->size() == tmp);
      
      addList->push_back(value);
    }
    
    /* Make sure things are in order */
    current = rbTree->min(rbTree->treeRoot);
    tmp = current->key;
    
    cout << rbTree->size();
    cout << ":size\n";
    
    for (uint64_t ix = 0; ix < rbTree->size()-1; ix++)
    {
      current = rbTree->next(current);
      
      if (current->key < tmp)
      {
        cout << "Index/Key:";
        cout << ix;
        cout << "/";
        cout << tmp;
        cout << " not sorted!\n";
      }
      
      tmp = current->key;
      
    }
    
    /* Remove random items */
    for (uint64_t ix = 0; ix < addList->size(); ix++)
    {
      if (random() % 2)
      {
        continue;
      }
      uint64_t curr = addList->at(ix);
      
      if (!curr)
      {
        continue;
      }
      
      tmp = rbTree->size();
      
      rbTree->remove(curr,curr);
      
      if (tmp == rbTree->size())
      {
        cout << "Nothing Removed!\n";
      }
      else
      {
        addList->at(ix) = 0;
      }
    }
    
    current = rbTree->min(rbTree->treeRoot);
    tmp = current->key;
    
    cout << rbTree->size();
    cout << ":size\n";
    
    for (uint64_t ix = 0; ix < rbTree->size()-1; ix++)
    {
      current = rbTree->next(current);
      
      if (current->key < tmp)
      {
        cout << "Index/Key:";
        cout << ix;
        cout << "/";
        cout << tmp;
        cout << " not sorted!\n";
      }
      
      tmp = current->key;
      
    }
  }
  
  for (uint64_t ix = 0; ix < addList->size(); ix++)
  {
    uint64_t curr = addList->at(ix);
    
    if (!curr)
    {
      continue;
    }
    
    tmp = rbTree->size();
    
    rbTree->remove(curr,curr);
    
    if (tmp == rbTree->size())
    {
      cout << "Nothing Removed!\n";
    }
    else
    {
      addList->at(ix) = 0;
    }
  }
  
  cout << "LLRB_Tree:Done\n";
  
  
  
  return 0;
}

/*
 int testLinkedList() {
 
 return 0;
 }
 */

int testArrayList()
{
  ArrayList<uint64_t,uint64_t> *arrayList, *arrayListClone;
  arrayList = new ArrayList<uint64_t,uint64_t>();
  uint64_t *verify = new uint64_t[arrayList->getSize()];
  uint64_t tmp;
  
  cout << "\nTesting ArrayList\n";
  
  for (int ix = 0; ix < arrayList->getSize(); ix++)
  {
    uint64_t value = random();
    arrayList->setIndex(ix, new Comparable<uint64_t,uint64_t>(value, value));
    verify[ix] = value;
  }
  
  for (int ix = 0; ix < arrayList->getSize(); ix++)
  {
    if (arrayList->atIndex(ix)->data != verify[ix])
    {
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
    }
  }
  
  delete arrayList;
  delete verify;
  
  arrayList = new ArrayList<uint64_t,uint64_t>(glbTestSize);
  verify = new uint64_t[arrayList->getSize()];
  
  for (int ix = 0; ix < arrayList->getSize(); ix++)
  {
    uint64_t value = random();
    arrayList->setIndex(ix, new Comparable<uint64_t,uint64_t>(value, value));
    verify[ix] = value;
  }
  
  for (uint64_t ix = 0; ix < arrayList->getSize(); ix++)
  {
    if (arrayList->atIndex(ix)->data != verify[ix])
    {
      cout << "Index ";
      cout << ix;
      cout << " does not match!\n";
    }
  }
  
  delete arrayList;
  delete verify;
  
  arrayList = new ArrayList<uint64_t,uint64_t>(glbTestSize);
  
  for (uint64_t ix = 0; ix < arrayList->getSize(); ix++)
  {
    uint64_t value = random();
    arrayList->setIndex(ix, new Comparable<uint64_t,uint64_t>(value, value));
  }
  
  arrayListClone = arrayList->cloneSort();
  
  arrayList->inPlaceSort();
  
  tmp = arrayList->atIndex(0)->key;
  
  for (uint64_t ix = 1; ix < arrayList->getSize(); ix++)
  {
    
    if (arrayList->atIndex(ix)->key < tmp)
    {
      cout << "Index ";
      cout << ix;
      cout << " not sorted!\n";
    }
    
    if (arrayListClone->atIndex(ix)->key < tmp)
    {
      cout << "Index ";
      cout << ix;
      cout << " not sorted!\n";
    }
    
    if (arrayList->atIndex(ix)->key != arrayListClone->atIndex(ix)->key)
    {
      cout << "Index ";
      cout << ix;
      cout << " not identical!\n";
    }
    
    tmp = arrayList->atIndex(ix)->key;
  }
  
  delete arrayList;
  delete arrayListClone;
  
  cout << "ArrayList:Done\n";
  
  return 0;
}

int testNeuralNetwork(const char *suiteName) {
  NeuralNetwork::NeuralNetwork *NNetwork;
  uint64_t iterations = 0;
  std::vector<double *> *thisInput;
  LLRB_Tree<double *, uint64_t> *thisExpect;
  std::vector<Trust<double> *> *thisOutput;
  double errorRate = 0.0;
  double thisError;
  std::vector<Info *> *hiddenInfo = new std::vector<Info *>();
  
  ifstream inputStream;
  ifstream verifyStream;
  ofstream outputData;
  ofstream log;
  
  uint64_t inputFileSize, verifyFileSize;
  uint64_t inSampleSize = 960 * 600 * sizeof(double);
  uint64_t outSampleSize = 960 * 600 * sizeof(double);
  
  char *verifyData = new char[inSampleSize];
  char *inputData = new char[outSampleSize];
  
  openFileStreams(suiteName, "_neuralNetwork");
  
  inputFileSize = inputStream.tellg();
  inputFileSize -= (inputFileSize % inSampleSize);
  
  inputStream.seekg (0, ios::beg);
  inputStream.read(inputData, inSampleSize);
  
  verifyFileSize = inputStream.tellg();
  verifyFileSize -= (verifyFileSize % outSampleSize);
  
  verifyStream.seekg (0, ios::beg);
  verifyStream.read(inputData, outSampleSize);
  
  
  for (uint32_t ix = 0; ix < inSampleSize; ix++)
  {
    for (uint32_t jx = 0; jx < log2(outSampleSize); jx++)
    {
      Info *info = new Info;
      info->c.inputLayer = 0;
      info->c.inputPosition = ix;
      info->c.layer = 4;
      info->c.position = jx;
      hiddenInfo->push_back(info);
    }
  }
  
  for (uint32_t ix = 0; ix < log2(outSampleSize); ix++)
  {
    for (uint32_t jx = 0; jx < outSampleSize; jx++)
    {
      Info *info = new Info;
      info->c.inputLayer = 4;
      info->c.inputPosition = jx;
      info->c.layer = 7;
      info->c.position = jx;
      hiddenInfo->push_back(info);
    }
  }
  
  cout << "\nTesting NeuralNetwork\n";
  
  thisInput = new std::vector<double *>();
  thisOutput = new std::vector<Trust<double> *>();
  thisExpect = new LLRB_Tree<double *, uint64_t>();
  
  thisInput->resize(glbInputSize);
  thisOutput->resize(glbOutputSize);  
  
  for (int jx = 0; jx < inSampleSize; jx++)
  {
    thisInput->at(jx) = &(((double *)inputData)[jx]);
  }
  
  for (uint64_t jx = 0; jx < outSampleSize; jx++)
  {
    thisOutput->at(jx) = new Trust<double>();
    thisOutput->at(jx)->actual = &(((double *)verifyData)[jx]);
    thisExpect->insert(new double(), jx);
  }
  
  NNetwork = new NeuralNetwork::NeuralNetwork
    (
     thisInput,
     thisOutput,
     thisExpect,
     hiddenInfo,
     (uint32_t)thisOutput->size()
    );
  
  for (uint64_t pos = 0; pos < inputFileSize; pos += inSampleSize)
  {
    inputStream.seekg (pos, ios::beg);
    
    if (pos < inputFileSize)
    {
      inputStream.read(inputData, inSampleSize);
    }
    
    if (pos < verifyFileSize)
    {
      verifyStream.read(verifyData, outSampleSize);
    }
    
    
    NNetwork->calcExpectation(pos);
    
    if (1) // (iterations > (glbIterations-4))
    {
      for (int ix = 0; ix < glbOutputSize; ix++)
      {
        cout << "{";
        cout << (*thisOutput->at(ix)->actual);
        cout << ":";
        cout << (*thisExpect->search(ix));
        cout << "},";
      }
      cout << "\n\n";
    }
    
    for (int ix = 0; ix < glbOutputSize; ix++)
    {
      *(thisOutput->at(ix)->actual) = *(thisInput->at(ix));
    }
    
    
    if (!(iterations % (uint64_t)(2*log2(glbIterations))))
    {
      errorRate = 0.0;
      
      for (int ix = 0; ix < glbOutputSize; ix++)
      {
        double thisActual = *thisOutput->at(ix)->actual;
        thisError = (*(thisExpect->search(ix)) - thisActual) / thisActual;
        errorRate += thisError * thisError;
      }
      
      cout << "Error Rate is ";
      cout << sqrt(errorRate/((double)(glbOutputSize)));
      cout << "\n";
    }
    
    NNetwork->doCorrection();
    
    iterations++;
    
  } while (iterations < glbIterations);
  
  
  //NNetwork->g
  //NNetwork->getMaximumFlow();
  
  delete thisInput;
  
  cout << "NeuralNetwork:Done\n";
  
  return 0;
}

int testNavigation()
{
  Navigation *navigation;
  vector<Coordinate *> locs;
  Coordinate *v, *u;
  Path *tmpPath;
  vector<Path *> *shortPath;
  uint64_t width = 100;
  uint64_t length = 100;
  double distance;
  int neighborHops = 1;
  
  cout << "\nTesting Navigation\n";
  
  navigation = new Navigation();
  locs.resize(width*length);
  
  for (uint64_t ix = 0; ix < width; ix++)
  {
    for (uint64_t jx = 0; jx < length; jx++)
    {
      Coordinate *thisCoordinate = new Coordinate(ix,jx,0);
      
      locs[ix+(jx*length)] = thisCoordinate;
    }
  }
  
  for (uint64_t ix = neighborHops; ix < (width-neighborHops); ix++)
  {
    for (uint64_t jx = neighborHops; jx < (length-neighborHops); jx++)
    {
      u = locs[ix+(jx*width)];
      for (int kx = (-neighborHops); kx <= neighborHops; kx ++)
      {
        for (int lx = (-neighborHops); lx <= neighborHops; lx++)
        {
          if ((jx == (length/2)) && (ix < (width*0.8)) && (ix > (width*0.2)))
          {
            continue;
          }
          
          if ((kx) || (lx))
          {
            v = locs[(ix+kx)+((jx+lx)*width)];
            
            distance = Navigation::calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
            navigation->addEdge(v, u, distance);
          }
        }
      }
    }
  }
  
  /* Width sides */
  
  for (uint64_t ix = 0; ix < neighborHops; ix++)
  {
    for (uint64_t jx = neighborHops; jx < (length-neighborHops); jx++)
    {
      u = locs[ix+(jx*width)];
      for (uint64_t kx = 0; kx < (ix+neighborHops); kx ++)
      {
        for (int lx = (-neighborHops); lx < neighborHops; lx++)
        {
          if ((kx != ix) || (lx))
          {
            v = locs[(kx)+((jx+lx)*width)];
            
            distance = Navigation::calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
            navigation->addEdge(v, u, distance);
          }
        }
      }
    }
  }
  
  for (uint64_t ix = (width-neighborHops); ix < width; ix++)
  {
    for (uint64_t jx = neighborHops; jx < (length-neighborHops); jx++)
    {
      u = locs[ix+(jx*width)];
      for (uint64_t kx = (ix-neighborHops); kx < width; kx ++)
      {
        for (int lx = (-neighborHops); lx < neighborHops; lx++)
        {
          if ((kx != ix) || (lx))
          {
            v = locs[(kx)+((jx+lx)*width)];
            
            distance = Navigation::calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
            navigation->addEdge(v, u, distance);
          }
        }
      }
    }
  }
  
  /* length sides */
  
  for (uint64_t ix = neighborHops; ix < (width-neighborHops); ix++)
  {
    for (uint64_t jx = 0; jx < neighborHops; jx++)
    {
      u = locs[ix+(jx*width)];
      for (int kx = (-neighborHops); kx < neighborHops; kx++)
      {
        for (uint64_t lx = 0; lx < (jx+neighborHops); lx++)
        {
          if ((kx) || (lx != jx))
          {
            v = locs[(ix+kx)+((lx)*width)];
            
            distance = Navigation::calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
            navigation->addEdge(v, u, distance);
          }
        }
      }
    }
  }
  
  for (uint64_t ix = neighborHops; ix < (width-neighborHops); ix++)
  {
    for (uint64_t jx = (length-neighborHops); jx < length; jx++)
    {
      u = locs[ix+(jx*width)];
      for (int kx = (-neighborHops); kx < neighborHops; kx ++)
      {
        for (uint64_t lx = (jx-neighborHops); lx < length; lx++)
        {
          if ((kx) || (lx != jx))
          {
            v = locs[(ix+kx)+((lx)*width)];
            
            distance = Navigation::calcDistance(v->X, v->Y, v->Z, u->X, u->Y, u->Z);
            navigation->addEdge(v, u, distance);
          }
        }
      }
    }
  }
  
  navigation->setStart(locs[(neighborHops)+((neighborHops) * width)]);
  navigation->setTerminal(locs[length*width-1]);
  
  if ((shortPath = navigation->getShortestPath()) != NULL)
  {
    cout << "Path Hops ";
    cout << shortPath->size();
    while ((!shortPath->empty()) && ((tmpPath = shortPath->back()) != NULL))
    {
      u = tmpPath->getBackward();
      v = tmpPath->getForward();
      
      cout << "{";
      cout << u->X;
      cout << ":";
      cout << u->Y;
      cout << ":";
      cout << u->Z;
      cout << "},";
      shortPath->resize(shortPath->size()-1);
    }
    
    cout << "{";
    cout << v->X;
    cout << ":";
    cout << v->Y;
    cout << ":";
    cout << v->Z;
    cout << "},";
  }
  
  cout << "Navigation:Done\n";
  
  
  return 0;
}

int testGeneticFile(const char *suiteName)
{
  ifstream inputStream;
  ofstream outputData;
  ofstream log;
  
  uint64_t inputFileSize;
  uint64_t sampleSize = 960 * 600 * sizeof(double);

  char *buffer0 = new char[sampleSize];
  char *buffer1 = new char[sampleSize];

  char *nextData = buffer0;
  char *inputData = buffer1;
  
  bool pingPong = false;
  
  openFileStreams(suiteName, "_genetic");
  
  Optimization::Genetic<NeuralNetwork::NeuralNetwork,NeuralNetwork::Neuron,double> geneticExp;
  
  geneticExp.addInput((double *)inputData);
  geneticExp.addOutput((double *)nextData);
  
  geneticExp.initInternals();
  
  inputFileSize = inputStream.tellg();
  inputFileSize -= (inputFileSize % sampleSize);
  
  inputStream.seekg (0, ios::beg);
  inputStream.read(nextData, sampleSize);
  
  for (uint64_t pos = sampleSize; pos < inputFileSize; pos += sampleSize)
  {
    inputStream.seekg (pos, ios::beg);
    
    if (pingPong)
    {
      inputData = buffer0;
      nextData = buffer1;
    }
    else
    {
      inputData = buffer1;
      nextData = buffer0;
    }
    
    inputStream.read(nextData, sampleSize);
    
    geneticExp.optimizeAnwser();
  }
  
  closeFileStreams();
  
  return 0;
}

int testGenetic(char **args, int nArgs)
{
  Optimization::Genetic<NeuralNetwork::NeuralNetwork,NeuralNetwork::Neuron,double> *geneticExp;
  uint64_t iterations = 0;
  std::vector<double *> *thisInput, *thisOutput, *thisObjective;
  std::vector<Trust<double> *> *thisTrust;
  double errorRate = 0.0;
  double thisError;
  std::vector<uint64_t> *layers = new std::vector<uint64_t>();
  uint64_t precision = (1 << 16);
  uint64_t hShift = 0, wShift = 0;
  uint64_t width = sqrt(glbOutputSize);
  uint64_t height = glbOutputSize / width;
  
  layers->resize(0);
  
  for (uint64_t ix = 0; ix < 1; ix++)
  {
    layers->push_back(log2(glbOutputSize));
  }
  
  cout << "\nTesting Genetic\n";
  
  thisInput = new std::vector<double *>();
  thisOutput = new std::vector<double *>();
  thisObjective = new std::vector<double *>();
  
  thisInput->resize(glbInputSize);
  thisOutput->resize(glbOutputSize);
  thisObjective->resize(glbOutputSize);
  
  
  for (int jx = 0; jx < glbInputSize; jx++)
  {
    thisInput->at(jx) = new double();
    thisOutput->at(jx) = new double();
    thisObjective->at(jx) = new double();
  }
  
  geneticExp = new Optimization::Genetic<NeuralNetwork::NeuralNetwork,NeuralNetwork::Neuron,double>();
  
  geneticExp->addInput(thisInput);
  geneticExp->addOutput(thisOutput);
  
  geneticExp->initInternals();
  
  
  for (int ix = 0; ix < glbOutputSize; ix++)
  {
    *(thisObjective->at(ix)) = max(((random() % precision) / (precision * 1.0)), 0.05);
    *(thisOutput->at(ix)) = *(thisObjective->at(ix)) + (((rand()/(double)RAND_MAX)*0.1) - 0.05);
  }
  
  do
  {
#if 1
    for (uint64_t jx = 0; jx < thisInput->size(); jx++)
    {
      *(thisInput->at(jx)) = *(thisOutput->at(jx));
    }
#endif
    
#if 1
    for (int ix = 0; ix < glbOutputSize; ix++)
    {
      *(thisOutput->at(ix)) = *(thisObjective->at(((ix+(wShift%width)+(width*(hShift%height))))%glbOutputSize)) + (((rand()/(double)RAND_MAX)*0.1) - 0.05);
    }
#endif
    
    geneticExp->optimizeAnwser();
    
    /* place objective reality for error rate */
    for (int ix = 0; ix < glbOutputSize; ix++)
    {
      *(thisOutput->at(ix)) = *(thisObjective->at(ix));
    }
    
    thisTrust = geneticExp->answer.select(NULL,NULL);
    
    if (iterations > (glbIterations-4))
    {
      for (int ix = 0; ix < glbOutputSize; ix++)
      {
        cout << "{";
        cout << (*thisTrust->at(ix)->actual);
        cout << ":";
        if (thisTrust->at(ix)->prediction == NULL)
        {
          cout << "NONE:NONE";
        }
        else
        {
          cout << (thisTrust->at(ix)->prediction->expectation);
          cout << ":";
          cout << (thisTrust->at(ix)->prediction->confidence);
        }
        cout << "},";
      }
      cout << "\n\n";
    }
    
    
    if (!(iterations % (uint64_t)(2*log2(glbIterations))))
    {
      errorRate = 0.0;
      
      for (int ix = 0; ix < glbOutputSize; ix++)
      {
        if (thisTrust->at(ix)->prediction == NULL)
        {
          continue;
        }
        
        thisError = (*thisTrust->at(ix)->actual - (thisTrust->at(ix)->prediction->expectation));
        errorRate += thisError * thisError;
      }
      
      errorRate = sqrt(errorRate/((double)(glbOutputSize)));
      
      cout << "Error Rate: ";
      cout << errorRate;
      cout << "\n";
      cout << "Size/Iteration: ";
      cout << geneticExp->candidates.size();
      cout << "/";
      cout << iterations;
      cout << "\n";
      
    }
    
    geneticExp->error_rate = errorRate;
    
    iterations++;
    /*hShift++;
    hShift %= 3;
    wShift++;
    wShift %= 3;
    */
  } while (iterations < glbIterations);
  
  
  delete thisInput;
  
  cout << "Genetic:Done\n";
  
  return 0;
}

int testNetworkFlow()
{
  Network<SimpleHub, SimplePipe> *network;
  SimpleHub *start, *terminal;
  const uint64_t xSize = 1920;
  const uint64_t ySize = 1200;
//  const uint64_t tSize = 60;
  const int xDist = log2(xSize);
  const int yDist = log2(ySize);
  const double a = 1, xO = 1.0, yO = 1.0;
  double capacity[(2*xDist)+1][(2*yDist)+1];
  SimpleHub *hubs[glbSlowTestSize][glbSlowTestSize];
  
  network = new Network<SimpleHub, SimplePipe>();
  network->setStart(start = new SimpleHub());
  network->setTerminal(terminal = new SimpleHub);
  
  for (int ix = 0; ix < ((2*xDist)+1); ix++)
  {
    for (int jx = 0; jx < ((2*yDist)+1); jx++)
    {
      capacity[ix][jx] = a * exp(-((pow(ix-xDist, 2)/(2*pow(xO, 2))) + (pow(jx-yDist, 2)/(2*pow(yO, 2)))));
    }
  }
  
  for (int ix = 0; ix < xSize; ix++)
  {
    for (int jx = 0; jx < glbSlowTestSize; jx++)
    {
      hubs[ix][jx] = new SimpleHub();
    }
  }
  
  for (int ix = 0; ix < xSize; ix++)
  {
    for (int jx = 0; jx < glbSlowTestSize; jx++)
    {
      for (int xOff = -xDist; xOff <= xDist; xOff++)
      {
        if ((ix + xOff < 0) || (ix + xOff >= xSize))
        {
          continue;
        }
        for (int yOff = -yDist; yOff <= yDist; yOff++)
        {
          if ((jx + yOff < 0) || (jx + yOff >= xSize))
          {
            continue;
          }
          new Pipe<SimpleHub,SimplePipe>(hubs[ix+xOff][jx+yOff],hubs[ix][jx],capacity[xOff+xDist][yOff+yDist]);
        }
      }
    }
  }
  
  return 0;
  
}


int main(int argc, const char * argv[])
{
  int ret = 0;
  
  cout << "Back-2-Basics\n";
  cout << "Created by Marcus Gilchrist on 9/24/12.\n";
  cout << "Copyright (c) 2012 Marcus Gilchrist. All rights reserved.\n";
  cout << "This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n";
  cout << "This is free software, and you are welcome to redistribute it\n";
  cout << "under certain conditions; type `show c' for details.\n";
  
  ret |= matrixConvolutionTest();
  
  /* Generate data for tests */
  //ret |= setupNeuralNetworkData("reserved");
  
  
  //ret |= testHashTable();
  //ret |= testSecureHashTable();
  //ret |= testArrayList();
  //ret |= testHeap();
  //ret |= testRBTree();
  //ret |= testLLRBTree();
  //ret |= testStack();
  //ret |= testNeuralNetwork("reserved");
  //ret |= testGenetic(NULL, NULL);
  //ret |= testMetaheuristic();
  //ret |= testNavigation();
  //ret |= testNetworkFlow();
  //ret |= testSegmentation(SUITE_NAME);
  
  cout << "Finished Testing:";
  cout << ret;
  cout << "\n";
  
  return ret;
}


