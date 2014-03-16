//
//  MatrixConvolution.cpp
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 2/22/14.
//  Copyright (c) 2014 Marcus Gilchrist. All rights reserved.
//

#include <iostream>
using namespace std;

#include <vector>
#include "MatrixConvolution.h"

namespace Collection
{

int64_t addPattern(int64_t seed, uint64_t iterations)
{
  return seed + iterations;
}

int64_t idxPattern(int64_t seed, uint64_t iterations)
{
  return seed + iterations;
}

int64_t randomPattern(int64_t seed, uint64_t iterations)
{
  return seed + (random() & 0x00FF);
}

int modifyAndPrintMatrix(vector<int64_t> *array, vector<uint64_t> *dim, uint64_t offset,
                          uint8_t dimensions, int64_t (*pattern)(int64_t seed, uint64_t iter), uint64_t iter)
{
  
  if (dimensions > 1)
  {
    for (uint8_t ix = dimensions; ix < dim->size(); ix++)
    {
      cout << " ";
    }
    cout << "{\n";
    
    for (uint64_t ix = 0; ix < dim->at(dimensions - 1); ix++)
    {
      modifyAndPrintMatrix(array, dim, (offset + ix) * dim->at(dimensions - 2), dimensions-1, pattern, iter);
      cout << "\n";
    }
    
    for (uint8_t ix = dimensions; ix < dim->size(); ix++)
    {
      cout << " ";
    }
    
    if (dimensions == dim->size()) {
      cout << "}";
    }
    else
    {
      cout << "},";
    }
  }
  else
  {
    for (uint8_t ix = dimensions; ix < dim->size(); ix++)
    {
      cout << " ";
    }
    cout << "{";
    
    cout << " ";
    
    for (uint64_t ix = 0; ; ix++)
    {
      uint64_t entry = pattern(array->at(ix + offset), iter);
      array->at(ix + offset) = entry;
      
      cout << ix + offset;
      cout << ":";
      cout << entry;
      
      if (ix >= (dim->at(0) - 1))
      {
        break;
      }
      
      cout << ", ";
    }
    
    cout << " },";
  }
  
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
  uint64_t totalKernelSize = 0;
  
  ret->resize(matrix->size());
  
  rArrayedMatrixTraverse(matrix, 0, mDimensions,
                         kernel, kDimensions, kCenter,
                         (uint8_t)(mDimensions->size()), ret);
  
  for (uint64_t ix = 0; ix < kernel->size(); ix++)
  {
    pair<int64_t, int64_t> currentKernelPair = kernel->at(ix);
    totalKernelSize += currentKernelPair.first + currentKernelPair.second;
  }
  
  totalKernelSize /= 2;
  
  for (uint64_t ix = 0; ix < matrix->size(); ix++)
  {
    matrix->at(ix) /= totalKernelSize;
  }
  
  return ret;
  
}

}