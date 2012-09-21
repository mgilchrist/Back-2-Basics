//
//  Heuristic.h
//  OpenSource
//
//  Created by Marcus Gilchrist on 9/16/12.
//  Copyright (c) 2012 Marcus Gilchrist. All rights reserved.
//

#ifndef OpenSource_Heuristic_h
#define OpenSource_Heuristic_h

template <class HeuristicType>
class Heuristic {
  
private:
  
public:
  Heuristic<HeuristicType>();
  Heuristic<HeuristicType>(Collection::Array<double> *);
  
  virtual void calculateExpectation(void) =0;
  virtual double getExpectation(void) =0;
  virtual void doCorrection(double,double) =0;
  
};

template <class HeuristicType>
Heuristic<HeuristicType>::Heuristic() {
  
}


#endif
