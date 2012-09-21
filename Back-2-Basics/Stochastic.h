//
//  Stochastic.h
//  OpenSource
//
//  Created by Marcus Gilchrist on 9/16/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#ifndef OpenSource_Stochastic_h
#define OpenSource_Stochastic_h

#include "Optimization.h"

template <class OptimizationType>
class Stoichastic : public Optimization<OptimizationType> {
  
private:
  
public:
  
  Collection::Stack<uint64_t> *selections(Collection::Stack<double *>,uint64_t *,uint64_t);
  
};

#endif
