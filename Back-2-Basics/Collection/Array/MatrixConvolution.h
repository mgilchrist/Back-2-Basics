//
//  MatrixConvolution.h
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 2/22/14.
//  Copyright (c) 2014 Marcus Gilchrist. All rights reserved.
//

#ifndef Back_2_Basics_MatrixConvolution_h
#define Back_2_Basics_MatrixConvolution_h

#include <vector>

namespace Collection
{

vector<int64_t> *matrixConvolution(vector<int64_t> *matrix, vector<uint64_t> *mDimensions,
                                   vector<pair<int64_t, int64_t>> *kernel, vector<uint64_t> *kDimensions,
                                   vector<uint64_t> *kCenter);


int64_t addPattern(int64_t seed, uint64_t iterations);
int64_t idxPattern(int64_t seed, uint64_t iterations);
int64_t randomPattern(int64_t seed, uint64_t iterations);

int modifyAndPrintMatrix(vector<int64_t> *array, vector<uint64_t> *dim, uint64_t offset,
                         uint8_t dimensions, int64_t (*pattern)(int64_t seed, uint64_t iter), uint64_t iter);
  
}

#endif
