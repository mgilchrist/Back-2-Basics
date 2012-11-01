//
//  Stochastic.h
//  Back-2-Basics
//
//  Created by Marcus Gilchrist on 9/16/12.
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

#ifndef OpenSource_Stochastic_h
#define OpenSource_Stochastic_h

#include "LLRB_Tree.h"
#include "Optimization.h"


template <class HeuristicType, class DataType>
class Stoichastic : public Optimization<HeuristicType,DataType> {
  
private:
  
  Graph::LLRB_Tree<HeuristicType *, DataType> *candidates;
  
protected:
  void rDoEpoch(Graph::LLRB_TreeNode<HeuristicType *,DataType>);
  void doEpoch();
  
public:
  Stoichastic();
};

template <class HeuristicType, class DataType>
void Stoichastic<HeuristicType,DataType>::rDoEpoch(Graph::LLRB_TreeNode<HeuristicType *,DataType> current) {
  
  current->data->calculateExpectation();
  
  if (current->getLeaf(LEFT) != NULL) {
    rDoEpoch(current->getLeaf(LEFT));
  }
  
  if (current->getLeaf(RIGHT) != NULL) {
    rDoEpoch(current->getLeaf(RIGHT));
  }
}

template <class HeuristicType, class DataType>
void Stoichastic<HeuristicType,DataType>::doEpoch() {
  rDoEpoch(candidates->treeNode);
}

#endif
